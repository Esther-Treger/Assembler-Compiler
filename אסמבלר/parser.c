#include "data.h"


void free_instruction(void *i)
{
	free(((instruction *) i)->value);
	free(i);
}


/* encodes a label value into an unsigned integer*/
unsigned int encode_label(int val)
{
	int i = ENC_RELOCATABLE;
	i |= val << 2;
	return i;
}

void add_instruction_to_list(void *value, int is_label, int line, struct list_node **instruction_pointer, int *instruction_count)
{
	instruction *inst = (instruction *) alloc(sizeof(*inst));
	inst->value = value;
	inst->is_label = is_label;
	inst->line = line;
	(*instruction_pointer)->next = linked_list_new_node(inst, free_instruction);
	*instruction_pointer = (*instruction_pointer)->next;
	(*instruction_count)++;
}

int add_num_to_instructions(char *n, struct list_node **instruction_pointer, int *instruction_count)
{
	instruction *inst;
	if (!encode_num(n, instruction_pointer, instruction_count))
		return 0;
	inst = (instruction *) alloc(sizeof(*inst));
	inst->value = (word *) (*instruction_pointer)->value;
	inst->is_label = 0;
	inst->line = 0;
	((word *) inst->value)->field <<= 2;
	(*instruction_pointer)->value = inst;
	(*instruction_pointer)->free = free_instruction;
	return 1;
}

enum parser_error_codes encode_operation(char *op_name, enum symbol OPCODE, int line, struct list_node **params, struct list_node **instruction_pointer, int *instruction_count)
{
	struct list_node *param_pointer = *params;
	word *first = (word *) alloc(sizeof(*first)), *registers = NULL;
	int address_method = 0, methods = 0;

	first->field = 0;
	add_instruction_to_list(first, 0, line, instruction_pointer, instruction_count);

	/* OPCODE */
	first->field |= (OPCODE) << OPBIT_OPCODE_6;

	/* address methods + parameters 1 & 2 */
	if (is_jump_operation(OPCODE)){
		if(!is_valid_label((char *)param_pointer->value))
			return PARSER_EINVALIDLABEL;
		first->field |= (param_pointer->next == NULL ? ADDRESS_DIRECT : ADDRESS_JUMP_WITH_PARAMS) << 2;
		add_instruction_to_list(str_dupl((char *) param_pointer->value), 1, line, instruction_pointer, instruction_count);
		param_pointer = param_pointer->next;
	}
	while (param_pointer != NULL) {
		address_method = determine_address_method(param_pointer->value);
		methods <<= 2;
		methods |= address_method;
		if (registers != NULL && address_method != ADDRESS_DIRECT_REGISTER) {
			add_instruction_to_list(registers, 0, line, instruction_pointer, instruction_count);
			registers = NULL;
		}
		switch (address_method) {
		case ADDRESS_INSTANT:
			if (!add_num_to_instructions(&((char *) param_pointer->value)[1], instruction_pointer, instruction_count)) {
				*params = param_pointer;
				return PARSER_EINVALIDNUMBER;
			}
			break;
		case ADDRESS_ERROR:
			*params = param_pointer;
			return PARSER_EINVALIDNUMBER;
		case ADDRESS_DIRECT:
			if(!is_valid_label((char *)param_pointer->value))
				return PARSER_EINVALIDLABEL;
			add_instruction_to_list(str_dupl(param_pointer->value), 1, line, instruction_pointer, instruction_count);
			break;
		case ADDRESS_DIRECT_REGISTER:
			if (registers == NULL) {
				registers = (word *) alloc(sizeof(*registers));
				registers->field = 0;
			}
			registers->field |= atoi(&((char *) param_pointer->value)[1]) << (param_pointer->next == NULL ? 2 : 8);
			break;
		default:
			break;
		}
		param_pointer = param_pointer->next;
	}
	if (registers != NULL)
		add_instruction_to_list(registers, 0, line, instruction_pointer, instruction_count);
	first->field |= methods << (is_jump_operation(OPCODE) ? OPBIT_PARAM2_10 : OPBIT_DEST_2);

	switch (OPCODE) {
	case OPCODE_LEA:
		if (!(((first->field & (1 << OPBIT_SOURCE_5)) >> OPBIT_SOURCE_5) ^ ((first->field & (1 << OPBIT_SOURCE_4)) >> OPBIT_SOURCE_4)))
			return PARSER_EINVALIDSOURCEPARAM;
	case OPCODE_MOV:
	case OPCODE_ADD:
	case OPCODE_SUB:
	case OPCODE_NOT:
	case OPCODE_CLR:
	case OPCODE_INC:
	case OPCODE_DEC:
	case OPCODE_JMP:
	case OPCODE_BNE:
	case OPCODE_RED:
	case OPCODE_JSR:
		if (!(first->field & ((1 << OPBIT_DEST_3) | (1 << OPBIT_DEST_2))))
			return PARSER_EINVALIDDESTPARAM;
		break;
	default:
		break;
	}

	return PARSER_OK;
}

int encode_num(char *s, struct list_node **data_pointer, int *data_count)
{
	int is_negative;
	word *w;
	if (!is_valid_num(s))
		return 0;
	is_negative = *s == '-';
	w = (word *) alloc(sizeof(*w));
	if (*s == '-' || *s == '+')
		s++;
	w->field = atoi(s) - is_negative;
	if (is_negative)
		w->field = ~w->field;
	(*data_pointer)->next = linked_list_new_node(w, free);
	*data_pointer = (*data_pointer)->next;
	(*data_count)++;
	return 1;
}

enum parser_error_codes encode_string(char *s, struct list_node **data_pointer, int *data_count)
{
	struct list_node *tmp;
	char *end;
	word *w;
	int i = 0;
	if (*s != '"')
		return PARSER_EEXPECTEDQUOTES;
	end = strrchr(s, '"') + 1;
	skip_white_space(end, &i);
	if (s == end-1 || count_non_white_space(end, &i) > 0)
		return PARSER_EUNFINISHEDSTRING;
	for (s++; s != end; s++) {
		if (!isprint(*s))
			return PARSER_EINVALIDCHAR;
		w = (word *) alloc(sizeof(*w));
		w->field = s == end-1 ? '\0' : *s; /* null terminator if we are at the end of the string */
		tmp = linked_list_new_node(w, free);
		if (*data_pointer != NULL)
			(*data_pointer)->next = tmp;
		*data_pointer = tmp;
		(*data_count)++;
	}
	return PARSER_OK;
}

enum parser_error_codes parse_params(char *line, int *i, int param_amount, struct list_node **head)
{
	int count, ii, is_jump_with_params = 0;
	char *param, *jump_end;
	struct list_node *n = NULL, *tmp;
	if (param_amount == PARAM_UNKNOWN)
		return PARSER_OK;
	if (param_amount == PARAM_JUMP) {
		if (strchr(&line[*i], '(') != NULL) {
			is_jump_with_params = 1;
			jump_end = strchr(&line[*i], ')');
			if (jump_end == NULL)
				return PARSER_EJUMPPARAMS;
			jump_end++;
			ii = 0;
			skip_white_space(jump_end, &ii);
			if (count_non_white_space(jump_end, &ii))
				return PARSER_EJUMPPARAMS;
			*(--jump_end) = '\0';
		} else {
			is_jump_with_params = 0;
			param_amount = PARAM_SINGLE;
		}
	}
	while ((count = count_non_white_space(line, i)) > 0 && param_amount != 0) {
		param = dupl_until(&line[(*i)-count], param_amount == PARAM_JUMP ? '(' : ',');
		*i -= count;
		ii = 0;
		if (count_non_white_space(param, &ii) == 0)
			return param_amount == PARAM_JUMP ? PARSER_EJUMPPARAMS : PARSER_EUNEXPECTEDCOMMA;
		if ((count = skip_white_space(param, &ii)) > 0) {
			if (is_jump_with_params || count_non_white_space(param, &ii) > 0)
				return PARSER_EUNEXPECTEDSPACE;
			param[ii-count] = '\0';
		}
		tmp = linked_list_new_node(param, free);
		if (n != NULL)
			(n)->next = tmp;
		n = tmp;
		if (*head == NULL)
			*head = n;
		*i += ii;
		if (line[*i] == (param_amount == PARAM_JUMP ? '(' : ','))
			(*i)++;
		else if (param_amount == PARAM_JUMP)
			return PARSER_EJUMPPARAMS;
		if (skip_white_space(line, i) > 0 && is_jump_with_params)
			return PARSER_EUNEXPECTEDSPACE;
		param_amount--;
	}
	if (param_amount > 0)
		return PARSER_ENOTENOUGHPARAMS;
	else if (param_amount == 0 && count > 0)
		return PARSER_ETOOMANYPARAMS;
	return PARSER_OK;
}

enum address_method determine_address_method(char *s)
{
	if (s[0] == '#')
		return is_valid_num(&s[1]) ? ADDRESS_INSTANT : ADDRESS_ERROR;
	else if (is_register(s))
		return ADDRESS_DIRECT_REGISTER;
	return ADDRESS_DIRECT;
}

int is_valid_num(char *s)
{
	if (*s != '-' && *s != '+' && !isdigit(*s))
		return 0;
	for (s++; *s != '\0' && *s != EOF; s++)
		if (!isdigit(*s))
			return 0;
	return 1;
}

int is_valid_label(char *s)
{

	int count;
	if (!isalpha(*s) || symbols_get(s) != UNKNOWN_SYMBOL)
		return 0; 	
	for (count = 1, s++; *s != '\0' && *s != EOF; s++, count++)
		if (!isalnum(*s)) 
			return 0; 	
	return count <= MAX_LEN_LABEL;
}

int is_valid_space(char c)
{
	return c == ' ' || c == '\t';
}

int is_line_terminator(char c)
{
	return c == '\n' || c == '\0' || c == EOF;
}

int skip_white_space(char line[], int *i)
{
	int count = 0;
	for (; is_valid_space(line[*i]) && !is_line_terminator(line[*i]); (*i)++, count++)
		;
	return count;
}

int count_non_white_space(char line[], int *i)
{
	int count = 0;
	for (; !is_valid_space(line[*i]) && !is_line_terminator(line[*i]); (*i)++, count++)
		;
	return count;
}

int count_until(char *s, char c)
{
	int count;
	for (count = 0; s[count] != c && !is_line_terminator(s[count]); count++)
		;
	return count;
}

char *dupl_until(char *s, char c)
{
	return create_string_copy(s, count_until(s, c));
}

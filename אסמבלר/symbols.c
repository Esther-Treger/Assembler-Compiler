#include "data.h"

struct hash_table *operations, *directives, *registers;
enum param_amount *param_amounts;
int param_amount_size;

void symbols_prepare(void)
{
	/* operation definitions */
	operations = hash_table_new();
	hash_table_set_int(operations, "mov", OPCODE_MOV);
	hash_table_set_int(operations, "cmp", OPCODE_CMP);
	hash_table_set_int(operations, "add", OPCODE_ADD);
	hash_table_set_int(operations, "sub", OPCODE_SUB);
	hash_table_set_int(operations, "not", OPCODE_NOT);
	hash_table_set_int(operations, "clr", OPCODE_CLR);
	hash_table_set_int(operations, "lea", OPCODE_LEA);
	hash_table_set_int(operations, "inc", OPCODE_INC);
	hash_table_set_int(operations, "dec", OPCODE_DEC);
	hash_table_set_int(operations, "jmp", OPCODE_JMP);
	hash_table_set_int(operations, "bne", OPCODE_BNE);
	hash_table_set_int(operations, "red", OPCODE_RED);
	hash_table_set_int(operations, "prn", OPCODE_PRN);
	hash_table_set_int(operations, "jsr", OPCODE_JSR);
	hash_table_set_int(operations, "rts", OPCODE_RTS);
	hash_table_set_int(operations, "stop", OPCODE_STOP);

	/* directive definitions */
	directives = hash_table_new();
	hash_table_set_int(directives, ".data", DIRECTIVE_DATA);
	hash_table_set_int(directives, ".string", DIRECTIVE_STRING);
	hash_table_set_int(directives, ".entry", DIRECTIVE_ENTRY);
	hash_table_set_int(directives, ".extern", DIRECTIVE_EXTERN);

	/* register definitions */
	registers = hash_table_new();
	hash_table_set_int(registers, "r0", REGISTER);
	hash_table_set_int(registers, "r1", REGISTER);
	hash_table_set_int(registers, "r2", REGISTER);
	hash_table_set_int(registers, "r3", REGISTER);
	hash_table_set_int(registers, "r4", REGISTER);
	hash_table_set_int(registers, "r5", REGISTER);
	hash_table_set_int(registers, "r6", REGISTER);
	hash_table_set_int(registers, "r7", REGISTER);

	/* parameter amount definitions */
	param_amount_size = operations->size + directives->size;
	param_amounts = (enum param_amount *) alloc(sizeof(*param_amounts) * param_amount_size);
	param_amounts[OPCODE_JMP] = PARAM_JUMP;
	param_amounts[OPCODE_BNE] = PARAM_JUMP;
	param_amounts[OPCODE_JSR] = PARAM_JUMP;
	param_amounts[OPCODE_MOV] = PARAM_TWO;
	param_amounts[OPCODE_CMP] = PARAM_TWO;
	param_amounts[OPCODE_ADD] = PARAM_TWO;
	param_amounts[OPCODE_SUB] = PARAM_TWO;
	param_amounts[OPCODE_LEA] = PARAM_TWO;
	param_amounts[OPCODE_NOT] = PARAM_SINGLE;
	param_amounts[OPCODE_CLR] = PARAM_SINGLE;
	param_amounts[OPCODE_INC] = PARAM_SINGLE;
	param_amounts[OPCODE_DEC] = PARAM_SINGLE;
	param_amounts[OPCODE_RED] = PARAM_SINGLE;
	param_amounts[OPCODE_PRN] = PARAM_SINGLE;
	param_amounts[OPCODE_RTS] = PARAM_NONE;
	param_amounts[OPCODE_STOP] = PARAM_NONE;
	param_amounts[DIRECTIVE_STRING] = PARAM_UNKNOWN;
	param_amounts[DIRECTIVE_DATA] = PARAM_LIST;
	param_amounts[DIRECTIVE_ENTRY] = PARAM_SINGLE;
	param_amounts[DIRECTIVE_EXTERN] = PARAM_SINGLE;
}

void symbols_free(void)
{
	hash_table_free(operations);
	hash_table_free(directives);
	hash_table_free(registers);
	free(param_amounts);
}

enum symbol symbols_get(char *symbol_name)
{
	int *symbol_code  = hash_table_get_int(operations, symbol_name);
	if (symbol_code  == NULL)
		symbol_code  = hash_table_get_int(directives, symbol_name);
	if (symbol_code  == NULL)
		symbol_code  = hash_table_get_int(registers, symbol_name);
	return symbol_code  == NULL ? UNKNOWN_SYMBOL : *symbol_code ;
}

enum param_amount symbols_get_param_amount(enum symbol op)
{
	if (op < 0 || op >= param_amount_size)
		return PARAM_UNKNOWN;
	return param_amounts[op];
}

int is_operation(char *s)
{
	return hash_table_get_int(operations, s) != NULL;
}

int is_directive(char *s)
{
	return hash_table_get_int(directives, s) != NULL;
}

int is_register(char *s)
{
	return hash_table_get_int(registers, s) != NULL;
}

int is_jump_operation(enum symbol o)
{
	return o == OPCODE_JMP || o == OPCODE_BNE || o == OPCODE_JSR;
}

int is_data_directive(enum symbol o)
{
	return o == DIRECTIVE_DATA || o == DIRECTIVE_STRING;
}

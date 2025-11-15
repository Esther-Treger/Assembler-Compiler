run: data.h assembler.c errors.c first_run.c hash_table.c linked_list.c parser.c macro.c second_run.c utils.c symbols.c 
	gcc -g -ansi -Wall -pedantic data.h assembler.c errors.c first_run.c hash_table.c linked_list.c parser.c macro.c second_run.c utils.c symbols.c    -o run

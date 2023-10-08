PROGRAM      := asmAlign
PROGRAM_ARGS := test.asm

CC           := gcc
CCFLAGS      := -Wall -Wextra -pedantic -g -std=c90


all : elf

elf :
	$(CC) $(CCFLAGS) $(PROGRAM).c -o $(PROGRAM)

valgrind : elf
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
	 ./$(PROGRAM) $(PROGRAM_ARGS)

clean : 
	rm $(PROGRAM)


.PHONY: clean all

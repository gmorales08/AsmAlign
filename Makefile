PROGRAM     := asmAlign

CC          := gcc
CCFLAGS     := -Wall -Wextra -pedantic -g

all : elf

elf : $(PROGRAM)

$(PROGRAM) :
	$(CC) $(CCFLAGS) $(PROGRAM).c -o $(PROGRAM)

.PHONY: clean all

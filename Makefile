PROGRAM     := asmAlign

CC          := gcc
CCFLAGS     := -Wall -Wextra -pedantic -g -std=c90

all : clean elf

elf : $(PROGRAM)

$(PROGRAM) :
	$(CC) $(CCFLAGS) $(PROGRAM).c -o $(PROGRAM)

clean: 
	rm $(PROGRAM)

.PHONY: clean all

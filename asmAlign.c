#include <stdio.h>
#include <stdlib.h>
/*#include <string.h>*/

int main(int argc, char *argv[]) {
    int n;
    int value;
    int instructionSize = 5; /* -i */
    int separation      = 1; /* -s */ 
    int upperCase       = 0; /* -u */
    

    if (argc > 4) {
        printf("Usage: asmAlign [options] <input file>\n");
        printf("  -i<number>  (The length of the longest instruction)\n");
        printf("              (The default number is 5. Min. 1, Max. 9.)\n");
        printf("  -s<number>  (Space between instruction and parameters)\n");
        printf("              (The default number is 1. Min. 1, Max. 9.) \n");
        printf("  -u          (Write the instructions in upper case)\n");
        return 0;
    }

    for (n = 1; n < argc; n++) {
        if (argv[n][0] == '-') {
            switch (argv[n][1]) {
                case 'i':
                    value = argv[n][2] - '0'; 
                    if (value < 1 || value > 9) {
                        value = 5;
                    }
                    instructionSize = value;
                    break;
                case 's':
                    value = argv[n][2] - '0';
                    if (value < 1 || value > 9) {
                        value = 1;
                    }
                    separation = value;
                    break;
                case 'u':
                    upperCase = 1;
                    break;
                default:
                    printf("Error: unrecognized command-line option -%c\n",
                            argv[n][1]);
            }
        } 
    }

    printf("Parameters used: i=%d s=%d u=%d\n", instructionSize, separation,
            upperCase);

    return 0;
}

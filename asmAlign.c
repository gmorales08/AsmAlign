#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <errno.h>*/

#define BUFSIZE 80

int main(int argc, char *argv[]) {
    int  n;
    int  value;
    int  instructionSize = 5; /* -i */
    int  separation      = 1; /* -s */ 
    int  upperCase       = 0; /* -u */
    FILE *file;
    FILE *fileOld;
    char *fileName;
    char fileNameOld[80];
    char buffer[BUFSIZE];
    
    if (argc < 2) {
        printf("Error: You must specify a file\n");
        return 1;
    } else {
        fileName = argv[1];
    }

    file = fopen(fileName, "r+");
    if (!file) {
        /*printf("Error: File %s could not be opened\n", fileName);*/
        perror(fileName);
        return 1;
    }

    if (argc > 5) {
        printf("Usage: asmAlign [options] <input file>\n");
        printf("  -i<number>  (The length of the longest instruction)\n");
        printf("              (The default number is 5. Min. 1, Max. 9.)\n");
        printf("  -s<number>  (Space between instruction and parameters)\n");
        printf("              (The default number is 1. Min. 1, Max. 9.) \n");
        printf("  -u          (Write the instructions in upper case)\n");
        return 1;
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

    printf("File: %s. Parameters used: i=%d s=%d u=%d\n", fileName,
            instructionSize, separation, upperCase);

    /* Backup the old file */
    strcpy(fileNameOld, fileName);
    strcat(fileNameOld, "_old");
    
    fileOld = fopen(fileNameOld, "w+");
    if (!fileOld) {
        /*printf("Error: Failed to backup the file\n");*/
        perror(fileNameOld);
        return 1;
    }    
    
    n = 0;
    do {
        if (fgets(buffer, BUFSIZE, file)) {
            fputs(buffer, fileOld);
        } 
        n++;
    } while (!feof(file));
    fclose(fileOld);
    printf("%d lines readed from %s\n", n - 1, fileName);
    printf("A backup file %s has been created\n", fileNameOld);

    return 0;
}

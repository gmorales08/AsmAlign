#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int  i;
    int  j;
    int  k;
    int  value;
    int  instructionSize = 4; /* -i */
    int  separation      = 1; /* -s */
    int  upperCase       = 0; /* -u */
    int  trim            = 0;
    int  iReading;
    int  iReaded;             /* Boolean values for use while writting in */
    int  sReaded;             /* the new file */
    FILE *file;
    FILE *fileAlign;
    char *fileName;
    char fileNameAlign[80];

    if (argc < 2) {
        printf("Error: You must specify a file\n");
        return 1;
    } else {
        fileName = argv[1];
    }

    file = fopen(fileName, "r");
    if (!file) {
        perror(fileName);
        return 1;
    }

    if (argc > 5) {
        printf("Usage: asmAlign [options] <input file>\n");
        printf("  -i<number>  (The length of the longest instruction)\n");
        printf("              (The default number is 4. Min. 1, Max. 9.)\n");
        printf("  -s<number>  (Space between instruction and parameters)\n");
        printf("              (The default number is 1. Min. 1, Max. 9.) \n");
        printf("  -u          (Write the instructions in upper case)\n");
        printf("  -t          (Remove white spaces between the parameters)\n");
        return 1;
    }

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'i':
                    value = argv[i][2] - '0';
                    if (value < 1 || value > 9) {
                        value = 5;
                    }
                    instructionSize = value;
                    break;
                case 's':
                    value = argv[i][2] - '0';
                    if (value < 1 || value > 9) {
                        value = 1;
                    }
                    separation = value;
                    break;
                case 'u':
                    upperCase = 1;
                    break;
                case 't':
                    trim = 1;
                    break;
                default:
                    printf("Error: unrecognized command-line option -%c\n",
                            argv[i][1]);
            }
        }
    }

    printf("File: %s. Parameters used: i=%d s=%d u=%d\n", fileName,
            instructionSize, separation, upperCase);

    /* Create the new file */
    strcpy(fileNameAlign, fileName);
    strcat(fileNameAlign, "_align");

    fileAlign = fopen(fileNameAlign, "w+");
    if (!fileAlign) {
        perror(fileNameAlign);
        return 1;
    }

    i        = 0;
    k        = instructionSize;
    iReaded  = 0;
    iReading = 0;
    sReaded  = 0;
    printf("Reading the file...\n");
    do {
        value = fgetc(file);
        if ((char) value == '\n') {
            i++;
            k        = instructionSize;
            iReaded  = 0;
            iReading = 0;
            sReaded  = 0;
            fputc((char) value, fileAlign);
        } else if ((char) value == ' ' || (char) value == '\t') {
            if (iReading == 1) {
                iReaded  = 1;
                iReading = 0;
                for (j = 0; j < separation; j++) {
                    fputc(' ', fileAlign);
                }
                sReaded = 1;
            } else if (iReaded == 1 && sReaded == 0) {
                for (j = 0; j < separation; j++) {
                    fputc(' ', fileAlign);
                }
                sReaded = 1;
            } else if (iReaded == 1 && sReaded == 1 && k <= 0 && trim == 0) {
                if (iReading == 1) {
                    fputc(' ', fileAlign);
                }
            } else if (iReaded == 1 && sReaded == 1 && k <= 0 && trim == 1) {
                /* Remove white spaces */
            } else if (k <= 0 || (iReaded == 0 && sReaded == 0)) {
                fputc(value, fileAlign);
            }
        } else if (value != EOF){
            /* Instruction read */
            if (iReaded == 0) {
                iReading = 1;
                if (upperCase) {
                    if ((char) value >= 'a' && (char) value <= 'z') {
                        value = value - 32;
                    }
                }
            /* Parameters read */
            } else if (sReaded == 1){
                while (k > 0) {
                    fputc(' ', fileAlign);
                    k--;
                }
            }
            fputc((char) value, fileAlign);
            k--;
        }
    } while (!feof(file));
    fclose(file);
    fclose(fileAlign);
    printf("%d lines readed from %s\n", i, fileName);
    /*printf("A backup file %s has been created\n", fileNameOld);*/

    return 0;
}

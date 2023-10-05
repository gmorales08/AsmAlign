#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LSIZE 80  /* Maximum Line Size */
#define FNSIZE 80 /* Maximum File Name Size */

int main(int argc, char *argv[]) {
    int  i;
    int  lines;               /* Count the lines readed */
    int  cIndex;              /* Index of character in a line */
    int  lSize;               /* Size of the line */
    int  instructionSize = 4; /* -i */
    int  separation      = 1; /* -s */
    int  upperCase       = 0; /* -u */
    int  trim            = 0; /* -t */
    int  iReading;            /* Boolean values for use while writting in */
    int  iReaded;             /* the new file */
    int  sReaded;
    int  iRemaining;          /* Chars remaining to complete the instruction */ 
    char value;               /* For read characters in args and file */
    FILE *file;
    FILE *fileAlign;
    char *fileName;
    char fileNameAlign[FNSIZE];
    char fileNameOld[FNSIZE];
    char line[LSIZE + 1];

    /* Check if file name is provided */
    if (argc < 2) {
        printf("Error: You must specify a file\n");
        return 1;
    } else if (strcmp(argv[1], "--usage")) {
        fileName = argv[1];
        /* Open the original file */
        file = fopen(fileName, "r");
        if (!file) {
            perror(fileName);
            return 1;
        }
    }
  
    /* Show usage */
    if (argc > 5 || !strcmp(argv[1], "--usage")) {
        printf("Usage: asmAlign [options] <input file>\n");
        printf("  -i<number>  (The length of the longest instruction)\n");
        printf("              (The default number is 4. Min. 1, Max. 9.)\n");
        printf("  -s<number>  (Space between instruction and parameters)\n");
        printf("              (The default number is 1. Min. 1, Max. 9.) \n");
        printf("  -u          (Write the instructions in upper case)\n");
        printf("  -t          (Remove white spaces between the parameters)\n");
        printf("  --usage     (Print usage information)\n");
        printf("\n");
        printf("Prebuilt options by architecture:\n");
        printf("  --z80       (-i4 -s1)\n");
        return 1;
    }

    /* Read the arguments */
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'i':
                    value = argv[i][2] - '0';
                    if (value < 1 || value > 9) {
                        value = 4;
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
                case '-':
                    if (!strcmp(argv[i], "--z80")) {
                        instructionSize = 4;
                        separation = 1;
                    }
                    break;
                default:
                    printf("Error: unrecognized command-line option -%c\n",
                            argv[i][1]);
            }
        }
    }
    printf("File: %s. Parameters used: i=%d s=%d u=%d t=%d\n", fileName,
            instructionSize, separation, upperCase, trim);

    /* Create the new file */
    strcpy(fileNameAlign, fileName);
    strcat(fileNameAlign, "_align");

    fileAlign = fopen(fileNameAlign, "w+");
    if (!fileAlign) {
        perror(fileNameAlign);
        return 1;
    }

    /* Read the file and make the changes */
    lines      = 0;
    iRemaining = instructionSize;
    iReaded    = 0;
    iReading   = 0;
    sReaded    = 0;
    printf("Reading the file...\n"); 
    do {
        if (fgets(line, LSIZE + 1, file)) { /* +1 because line contains \n */
            lSize = strlen(line);
            /* if the line ends with ':' its a label and is printed directly */
            if (line[lSize - 2] != ':') {
                /* Read the characters of the line */
                cIndex = 0;
                do {
                    value = line[cIndex];
                    if (value == '\n') {
                        iRemaining = instructionSize;        
                        iReaded    = 0;
                        iReading   = 0;
                        sReaded    = 0;
                        fputc(value, fileAlign);
                    } else if (value == ' ' || value == '\t') {
                        /* Reading the first space of the separation */
                        if (iReading == 1) {
                            iReaded  = 1;
                            iReading = 0;
                            for (i = 0; i < separation; i++) {
                                fputc(' ', fileAlign);
                            }
                            sReaded = 1;
                        /* Reading the first space of the separation */
                        } else if (iReaded == 1 && sReaded == 0) {
                            for (i = 0; i < separation; i++) {
                                fputc(' ', fileAlign);
                            }
                            sReaded = 1;
                        /* Reading the args and not trimming spaces */
                        } else if (iReaded == 1 && sReaded == 1 && 
                                   iRemaining <= 0 && trim == 0) {
                            if (iReading == 1) {
                                fputc(' ', fileAlign);
                            }
                        /* Reading the args and trimming spaces */
                        } else if (iReaded == 1 && sReaded == 1 &&
                                   iRemaining <= 0 && trim == 1) {
                            /* Remove white spaces */
                        /* Reading the space before instruction */
                        } else if (iRemaining == instructionSize && 
                                   iReaded == 0 && sReaded == 0) {
                            fputc(value, fileAlign);
                        }
                    } else if (value != EOF) {
                        /* Instruction read */
                        if (iReaded == 0) {
                            iReading = 1;
                            if (upperCase) {
                                if (value >= 'a' && value <= 'z') {
                                    value = value - 32;
                                }
                            }
                        /* Parameters read */
                        } else if (sReaded == 1) {
                            while (iRemaining > 0) {
                                fputc(' ', fileAlign);
                                iRemaining--;
                            }
                        }
                        fputc(value, fileAlign);
                        iRemaining--;
                    }
                    cIndex++;
                } while ((int) value != EOF && cIndex < lSize);
            /* The line readed is a label */
            } else {
                fputs(line, fileAlign);
            } 
        }
        lines++;
    } while (!feof(file));

    fclose(file);
    fclose(fileAlign);
    /* Rename the file */
    strcpy(fileNameOld, fileName);
    strcat(fileNameOld, "_old");
    if (rename(fileName, fileNameOld) == 1) {
        perror(fileName);
    }
    if (rename(fileNameAlign, fileName) == 0) {
        printf("A backup file %s with the original content has been created\n",
               fileNameOld);
    }
    printf("%d lines readed from %s\n", lines, fileName); 
    
    return 0;
}

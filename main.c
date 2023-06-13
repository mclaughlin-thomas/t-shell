#include "main.h"

char *read_validate_line(void);

int main(int argumentCount, char **argumentVector){
    char *initPrompt = "t-shell> $ ";
    char *userInputLine;

    (void)argumentCount; (void)argumentVector;(void)initPrompt;

    userInputLine = read_validate_line();

    free(userInputLine);
    return (0);
}

char *read_validate_line(void){
    int initBufferSize = 1024; //seems as if this is the norm
    char *curBuffer = malloc(sizeof(char) * initBufferSize);
    if (!curBuffer) { //curBuffer was unable to allocate the memory if it is null
        fprintf(stderr, "There was an allocation error you fool!\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int curChar;
    while (1) {
        // Read character from command and return ASCII value of said character
        curChar = getchar();

        // If end of file is reached, it will be replaced with a null character, and the current buffer returned.
        if (curChar == EOF || curChar == '\n') {
            curBuffer[i] = '\0';
            return curBuffer;
        } else {
            curBuffer[i] = curChar;
        }
        i++;

        // If we have exceeded the buffer, reallocate.
        if (i >= initBufferSize) {
            initBufferSize += initBufferSize;
            curBuffer = realloc(curBuffer, initBufferSize);

            if (!curBuffer) { //curBuffer was unable to allocate the memory if it is null
                fprintf(stderr, "There was an allocation error you fool!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
    
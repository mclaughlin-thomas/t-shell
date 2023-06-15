#include "main.h"

char *read_validate_line(void);

//parse the line into a list of arguments
char **parse_tokenize_line(char *userInputLine);

int main(int argc, char **argv){    
    char *userInputLine;
    char **tokenizedLine;

    (void)argc; (void)argv;

    printf("t-shell> $ ");
    userInputLine = read_validate_line();
    tokenizedLine = parse_tokenize_line(userInputLine);

    free(userInputLine);
    free(tokenizedLine);

    return (0);
}

char *read_validate_line(void){
    int initBufferSize = 1024; //seems as if this is the norm
    char *curBuffer = malloc(sizeof(char) * initBufferSize);
    if (!curBuffer) { //curBuffer was unable to al`locate the memory if it is null
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
char **parse_tokenize_line(char *userInputLine){
    int initBufferSize = 64; // Seems to be what everyone else is doing...
    char **tokenCollection = malloc(sizeof(char ) * initBufferSize );
    char *curToken;

    int i = 0;

    if (!tokenCollection) {
        fprintf(stderr, "There was an allocation error you fool!\n");
        exit(EXIT_FAILURE);
    }
    curToken = strtok(userInputLine, " \t\n\r\a"); // We tokenize the line based on whitespace
    while(curToken != NULL){
        tokenCollection[i] = curToken;
        i++;

        //checking to see if the current token count is past our current buffer size
        if (i >= initBufferSize) {
            initBufferSize += initBufferSize;
            curToken = realloc(tokenCollection, initBufferSize * sizeof(char*));
            if (!tokenCollection) {
                fprintf(stderr, "There was an allocation error you fool!\n");
                exit(EXIT_FAILURE);
            }
        }
        curToken = strtok(NULL, " \t\n\r\a");
    }

    tokenCollection[i] = NULL;
    return tokenCollection;
}

int lsh_launch(char **args){
    pid_t pid;
    pid_t childPid;
    int status;

    pid = fork();

    if (pid == 0) { // is current process the child process?
        if (execvp(args[0], args) == -1) { // did we get an error attempting to execute the new program?
            fprintf(stderr, "There was an error you fool!\n");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) { //should not be less than 0, indicates process error
        // Error forking
        fprintf(stderr, "There was an error you fool!\n");
    } 
    else {
        do { // this is the parents process
            childPid = waitpid(pid, &status, WUNTRACED); // child process to change state
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

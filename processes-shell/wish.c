#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    
    if(argc == 2) {
        char* line = NULL;
        size_t len = 0;
        ssize_t nread;

        FILE* fp = fopen(argv[1], "r");
        if(fp == NULL) {
            char error_message[30] = "An error has occured\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

        // printf("%s\n", "wish> ");
        while((nread = getline(&line, &len, fp)) != -1) {
            
            if(nread > 0 && line[nread - 1] == '\n') {
                line[--nread] = '\0';
            }

            if(strcmp(line, "exit") == 0) {
                exit(0);
            }
            else {
                char* strVector[10];
                char* token;
                int index = 0;

                while(index < 10 && (token = strsep(&line, " \t")) != NULL) {
                    strVector[index++] = token;
                }

                if(strcmp(strVector[0], "cd") == 0 && index > 1 && index < 3) {
                    printf("%s\n", "this is correct");
                }
                else {
                    char error_message[30] = "An error has occurred\n";
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
            }
            
            // printf("%s\n", "wish> ");

        }
        free(line);
    }
    
    return 0;
}
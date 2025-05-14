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
            error();
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
                char* duplic = line;
                int index = 0;

                while(index < 10 && (token = strsep(&duplic, " \t")) != NULL) {
                    strVector[index++] = token;
                }

                if(index == 0) continue;

                if(strcmp(strVector[0], "cd") == 0) {

                    if(index != 2) {

                        char error_message[128];
                        int len = snprintf(error_message, sizeof(error_message),
                            "cd: %s: No such file or directory\n", strVector[1]);
                        write(STDERR_FILENO, error_message, (size_t)len);
                    }
                    else if(chdir(strVector[1]) == -1) {

                        char error_message[128];
                        int len = snprintf(error_message, sizeof(error_message),
                            "cd: %s: No such file or directory\n", strVector[1]);
                        write(STDERR_FILENO, error_message, (size_t)len);
                    }
                }
                else if(strcmp(strVector[0], "ls") == 0) {

                    if(index != 2) {

                        char error_message[128];
                        int len = snprintf(error_message, sizeof(error_message),
                            "ls: %s: No such file or directory\n", strVector[1]);
                        write(STDERR_FILENO, error_message, (size_t)len);
                    }
                    else {
                        if(access(strVector[1], X_OK) == 1) {

                            printf("%s\n", "this is correct");
                        }
                        else {

                            char error_message[128];
                            int len = snprintf(error_message, sizeof(error_message),
                            "ls: %s: No such file or directory\n", strVector[1]);
                            write(STDERR_FILENO, error_message, (size_t)len);
                        }
                    }
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

void getTokens(FILE* input) {
    
    while(1) {
        if(input == stdin) printf("%s", "wish> ");

        char* line = NULL;
        size_t len = 0;
        ssize_t nread;
        int index = 0;

        if(getline(&line, &len, input) == EOF) exit(0);

        unsigned long long lineSize = strlen(line);
        char* newLine = (char*)malloc(sizeof(char) * lineSize * 6);
        int shift = 0;

        

    }
}

void error() {
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}


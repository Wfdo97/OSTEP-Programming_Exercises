#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* arrayOfTokens[100];
int idx = 0;
int childPIds[100];
int numOfChilds = 0;

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
                int indx = 0;

                while(indx < 10 && (token = strsep(&duplic, " \t")) != NULL) {
                    strVector[indx++] = token;
                }

                if(indx == 0) continue;

                if(strcmp(strVector[0], "cd") == 0) {

                    if(indx != 2) {

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

                    if(indx != 2) {

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

        int temp = idx;
        idx = 0;
        char* line = NULL;
        size_t len = 0;
        ssize_t nread;

        if(getline(&line, &len, input) == EOF) exit(0);

        unsigned long long lineSize = strlen(line);
        char* newLine = (char*)malloc(sizeof(char) * lineSize * 6);
        int shift = 0;

        if(strcmp(line, "&\n") == 0) continue;

        for(int i = 0; i < lineSize; i++) {
            if(line[i] == '>' || line[i] == '&' || line[i] == '|') {
                newLine[i + shift] = ' ';
                shift++;
                newLine[i + shift] = line[i];
                i++;
                newLine[i + shift] = ' ';
                shift++;
            }
            newLine[i + shift] = line[i];
        }

        char* token;
        while((token = strsep(&newLine, " \t")) != NULL) {
            
            if(strcmp(token, "&") == 0) {
                arrayOfTokens[idx] = NULL;
                idx++;
            }
            if(memcmp(token, "\0", 1)) {
                arrayOfTokens[idx] = token;
                idx++; 
            }
        }

        for(int i = idx; i < temp; i++) {
            arrayOfTokens[i] = NULL;
        }

        execCommands();
        free(newLine);
        for(int i = 0; i < numOfChilds; i++) {
            int stat;
            waitpid(childPIds[i], &stat, 0);
        }
    }
}

void error() {
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}


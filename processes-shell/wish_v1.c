#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_PATH 4096 

char* arrayOfTokens[100];
int idx = 0;
int childPIds[100];
int numOfChilds = 0;
int numOfPaths = 2;
char* paths[100] = {"/bin/", "/usr/bin/"};

void getTokens(FILE*);
void execCommand();
int redirection(char* arr[], int start);
void error();

int main(int argc, char* argv[]) {

    if(argc == 1) getTokens(stdin);
    else if(argc == 2) {

        FILE* fp = fopen(argv[1], "r");
        if(fp == NULL) {
            error();
            exit(1);
        }
        else {
            getTokens(fp);
        }
    }
    else {
        error();
        exit(1);
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

        if(!strcmp(line, "&\n")) continue;

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
            
            if(!strcmp(token, "&")) {
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

        execCommand();
        free(newLine);
        for(int i = 0; i < numOfChilds; i++) {
            int stat;
            waitpid(childPIds[i], &stat, 0);
        }
    }
}

void execCommand() {
    for(int i = 0; i < idx; i++) {
        if(!strcmp(arrayOfTokens[i], "exit")) {
            if(arrayOfTokens[i + 1] == NULL) {
                exit(0);
            }
            else {
                i++;
                error();
            }
        }
        else if(!strcmp(arrayOfTokens[i], "cd")) {
            if(chdir(arrayOfTokens[++i])) {
                error();
            }

            while(arrayOfTokens[i] != NULL) {
                i++;
            }
            i++;
        }
        else if(!strcmp(arrayOfTokens[i], "path")) {
            numOfPaths = 0;
            for(int j = 1; j < idx; j++) {

                char* temp = realpath(arrayOfTokens[j], NULL);
                if(temp != NULL) paths[j - 1] = temp;
                else error();

                numOfPaths++;
            }
            i += numOfPaths;
        }
        else {
            int pid = fork();
            childPIds[numOfChilds++] = pid;

            if(pid == -1) {
                error();
                exit(1);
            }
            if(pid == 0) {
                if(redirection(arrayOfTokens, i) != -1) {
                    for(int j = 0; j < numOfPaths; j++) {
                        char* exe = malloc(sizeof(char) * MAX_PATH);
                        strcpy(exe, paths[j]);
                        strcat(exe, "/");
                        strcat(exe, arrayOfTokens[i]);
                        if(!access(exe, X_OK)) {
                            execv(exe, arrayOfTokens + i);
                        }
                    }
                    error();
                    exit(1);
                }
            }
            else {
                while(arrayOfTokens[i] != NULL) {
                    i++;
                }
                i++;
                
                if(i < idx) continue;
            }
        }
    }
}

int redirection(char* arr[], int start) {
    for(int i = start; arr[i] != NULL; i++) {
        if(!strcmp(arr[i], ">") && i != start) {
            arr[i] = NULL;
            if(arr[i + 1] == NULL || arr[i + 2] != NULL) {
                error();
                return -1;
            }
            else {
                int fd = open(arr[i + 1], O_WRONLY | O_CREAT, 0777);
                dup2(fd, STDOUT_FILENO);
                arr[i + 1] = NULL;
                close(fd);
            }
            return i;
        }
    }
    return 0;
}

void error() {
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}


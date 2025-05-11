#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    if(argc > 2) {
        printf("%s", "wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("%s", "cannot open this file\n");
        exit(1);
    }

    char* line = NULL;
    ssize_t nread;
    size_t len = 0;

    while((nread = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
    }

    return 0;
}

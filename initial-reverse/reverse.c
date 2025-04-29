#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    if(argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>");
        exit(1);
    }

    /*====== Read File ======*/
    FILE* file = fopen(argv[1], "r");
    if(file == NULL) {
        fprintf(stderr, "error: cannot open file 'input.txt'");
        exit(1);
    }

    int capacity = 16;
    int count = 0;
    char **strings = malloc(capacity * sizeof(*strings));
    if(strings == NULL) {
        printf("malloc failed"); 
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;

    while(getline(&line, &len, file) != -1) {
        if(count == capacity) {
            capacity *= 2;
            char **temp = realloc(strings, capacity * sizeof(*strings));
            if(temp == NULL) {
                perror("realloc didn't work properly");
                break;
            }
            strings = temp;
        }

        strings[count] = malloc(strlen(line) + 1);
        if(strings[count] == NULL) {
            perror("malloc didn't work properly");
            break;
        }
        strcpy(strings[count], line);
        count++;
    }

    free(line);
    fclose(file);

    /*====== Condition 1:  ======*/
    for(int i = count - 1; i >= 0; i--) {
        printf("%s", strings[i]);
        free(strings[i]);
    }
    free(strings);
    return 0;
}
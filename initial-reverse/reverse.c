#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char* argv[]) {

    int count = 0;
    int capacity = 16;
    char **strings = malloc(capacity * sizeof(*strings));
    if(strings == NULL) {
        printf("malloc failed"); 
        exit(1);
    }

    if(argc == 3) {
        char* input = strrchr(argv[1], '/');
        char* output = strrchr(argv[2], '/');

        input = input ? input + 1 : argv[1];
        output = output ? output + 1 : argv[2];
        
        if(strcmp(input,output) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
    }

    if(argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    else if(argc == 1) {
        char buff[100];
        while(fgets(buff, sizeof(buff), stdin)) {
            if(count == capacity) {
                capacity *= 2;
                char **temp = realloc(strings, capacity * sizeof(*strings));
                if(temp == NULL) {
                    perror("realloc didn't work properly");
                    break;
                }
                strings = temp;
            }

            strings[count] = malloc(strlen(buff) + 1);
            if(strings[count] == NULL) {
                perror("malloc didn't work properly");
                break;
            }
            strcpy(strings[count], buff);
            count++;
        }
    }
    else if(argc > 1) {
        /*====== Read File ======*/
        FILE* file1 = fopen(argv[1], "r");
        if(file1 == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        char *line = NULL;
        size_t len = 0;

        while(getline(&line, &len, file1) != -1) {
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
        fclose(file1);
    }

    
    if(argc == 3) {
        FILE* file2 = fopen(argv[2], "w");
        if(file2 == NULL) {
            fprintf(stderr, "error: cannot open file 'output.txt'");
            exit(1);
        }

        for(int i = count - 1; i >= 0; i--) {
            fprintf(file2, "%s", strings[i]);
            free(strings[i]);
        }
    }
    else {
        for(int i = count - 1; i >= 0; i--) {
            printf("%s", strings[i]);
            free(strings[i]);
        }
    }

    free(strings);
    return 0;
}
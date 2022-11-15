#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int fd[2];
    // fd[1] = write
    // fd[0] = read;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe Failed");
        return -1;
    }

    char input_string[100];
    pid_t p;

    scanf("%s", input_string);

    p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed");
        return -1;
    }
    else if (p == 0) {
        write(fd[1], input_string, strlen(input_string));
        close(fd[1]);
    } else if (p > 0) {
        char output_string[100];
        read(fd[0], output_string, 100);
        for (int i = 0; output_string[i] != '\0'; i++) {
            if (output_string[i] > 90) {
                output_string[i] -= 32; 
            } else {
                output_string[i] += 32;
            }
        }
        printf("%s\n", output_string);
    }

    return 0;
}
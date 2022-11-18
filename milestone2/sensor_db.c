#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "config.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/wait.h>
#include "sensor_db.h"

int cnt = 0;
FILE* logfile;

FILE* open_db(char* filename, bool append) {
    logfile = open_log();
    if (logfile == NULL) {
        fprintf(stderr, "Failed To Open gateway.log");
        return NULL;
    } 
    
    FILE* data;
    if (append) {
        data = fopen(filename, "a");
    } else {
        data = fopen(filename, "w");
    }
    
    if (data == NULL) {
        insert_to_log("Failed To Open data.csv");
    } else {
        insert_to_log("Successfully Opened data.csv");
    }
    
    return data;
}

int insert_sensor(FILE* f, sensor_id_t id, sensor_value_t value) {
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    if (fprintf(f, "%d, %f, %s", id, value, asctime(info)) < 0) {
        insert_to_log("Failed To Insert Data Into data.csv");
        return -1;
    } else {
        insert_to_log("Successfully Inserted Data Into data.csv");
    }
    return 0;
}

int close_db(FILE * f) {
    if (fclose(f) != 0) {
        insert_to_log("Failed To Close data.csv");
        return -1;
    } else {
        insert_to_log("Successfully Closed data.csv");
    }

    if (close_log() != 0) {
        insert_to_log("Failed To Close gateway.log");
        return -1;
    }
    return 0;
}

void insert_to_log(char* message) {
    cnt++;
    int fd[2];
    // fd[1] = write
    // fd[0] = read;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe Failed");
    }

    pid_t p;

    p = fork();

    if (p < 0) {
        fprintf(stderr, "Fork Failed");
    }
    else if (p == 0) {
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        waitpid(-1, NULL, 0);
    } else if (p > 0) {
        read(fd[0], message, 100);
        close(fd[0]);
        child_process(message);
        exit(0);
    }
    return;
}


FILE * open_log() {
    return fopen("gateway.log", "a");
}

int close_log() {
    return fclose(logfile);
}

void child_process(char* message) {
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    fprintf(logfile, "%d: %s    -   %s\n", cnt, asctime(info), message);
}


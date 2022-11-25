/**
 * \author Nur Alda-Onggar
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include "config.h"
#include "lib/tcpsock.h"

#define PORT 5678
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting

struct my_thread_info {
    int bytes;
    int result;
    sensor_data_t data;
};

int opened_conn_cnt;

void* connection(void* client_void) {
    
    struct my_thread_info* info;
    info = malloc(sizeof(struct my_thread_info));
    tcpsock_t* client = (tcpsock_t*) client_void;

    do {
        // read sensor ID
        info->bytes = sizeof((info->data).id);
        info->result = tcp_receive(client, (void *) &(info->data).id, &(info->bytes));
        // read temperature
        info->bytes = sizeof((info->data).value);
        info->result = tcp_receive(client, (void *) &(info->data).value, &(info->bytes));
        // read timestamp
        info->bytes = sizeof((info->data).ts);
        info->result = tcp_receive(client, (void *) &(info->data).ts, &(info->bytes));
        if ((info->result == TCP_NO_ERROR) && info->bytes) {
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", (info->data).id, (info->data).value,
                (long int) (info->data).ts);
        }
    } while (info->result == TCP_NO_ERROR);
    
    if (info->result == TCP_CONNECTION_CLOSED) {
        opened_conn_cnt--;
        printf("Peer has closed connection\n");
    } else {
        printf("Error occured on connection to peer\n");
    }
    
    tcp_close(&client);
    free(info);

    return NULL;
}

int main(void) {
    tcpsock_t *server, *client;
    sensor_data_t data;
    int bytes, result, rc;
    pthread_t tid[MAX_CONN];

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        printf("Incoming new client connection\n");
        opened_conn_cnt++;
        rc = pthread_create(&tid[opened_conn_cnt], NULL, connection, (void*) client);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    } while (opened_conn_cnt < MAX_CONN);
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");

    for (int t = 0; t < MAX_CONN; t++) {
        pthread_cancel(tid[t]);;
    }

    return 0;
}

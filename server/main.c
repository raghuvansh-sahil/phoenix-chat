#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "server.h"
#include "message.h"

volatile sig_atomic_t server_running = 1;

static void handle_sigint() {
    log_event("INFO", "SIGINT received. Preparing to shut down server.");
    server_running = 0;
}

static void setup_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("[SERVER] sigaction");
        exit(EXIT_FAILURE);
    }
}

int main() {
    setup_signal_handler();

    int listening_socket;
    get_listening_socket(&listening_socket);
    
    run_server(listening_socket, &server_running);

    close(listening_socket); 

    return 0;
}
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "ced.h"

/**
 * @brief Handler function for client connections of the echo server
 * @param client The client to handle
 */
void echo_client_handler(ced_server_client_p client) {
    ced_log("Currently managing %d clients\n", client->server->clients->size);
    ced_log("(fd %d) %s:%d\n", client->fd, client->host, client->port);

    ced_buffer_p buffer = ced_buffer_new_handle(client->fd, 0);

    if (buffer != NULL) {
        if (ced_buffer_cmp_str(buffer, "exit") == 0) {
            ced_server_stop(client->server);
        } else if (ced_buffer_cmp_str(buffer, "query") == 0) {
            char res[1024];
            sprintf(res, "Currently managing %d clients\n", client->server->clients->size);
            write(client->fd, res, strlen(res));
        } else {
            int x = rand() % 10;
            sleep(x);
            write(client->fd, buffer->data, buffer->size);
        }

        ced_buffer_free(buffer);
    }

    // all done with the client
    ced_server_client_finish(client);
}

/**
 * @brief Entry point for the echo server
 * @return 0 on success, 1 on failure
 */
int main() {
    ced_server_p server = ced_server_new(NULL, 3001, echo_client_handler);

    if (ced_server_start(server) != CED_SUCCESS) {
        return EXIT_FAILURE;
    }

    ced_server_free(server);

    return EXIT_SUCCESS;
}

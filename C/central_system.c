#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

#define MAX_JSON_SIZE 4096

static struct lws *wsi;
static int connection_established = 0;

struct per_session_data__ocpp {
    int message_count;
};

static int callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    struct per_session_data__ocpp *session_data = (struct per_session_data__ocpp *)user;

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("Client connected\n");
            session_data->message_count = 0;  // Initialize message count
            break;

        case LWS_CALLBACK_RECEIVE:
            if (len > MAX_JSON_SIZE) {
                fprintf(stderr, "Received message too large\n");
                return -1;
            }

            // Ensure null-termination, safely if message is too long
            char received_message[MAX_JSON_SIZE];
            strncpy(received_message, (const char *)in, sizeof(received_message) - 1);
            received_message[sizeof(received_message) - 1] = '\0';

            // Increment the message count for this session
            session_data->message_count++;

            printf("Received message %d from client: %s\n", session_data->message_count, received_message);

            // Send a response back to the client (echo)
            lws_write(wsi, in, len, LWS_WRITE_TEXT);
            break;

        default:
            break;
    }
    return 0;
}

int main() {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);

    static struct lws_protocols protocols[] = {
        {
            "ocpp2.0.1",             // Protocol name
            callback,                // Callback function for handling WebSocket events
            sizeof(struct per_session_data__ocpp),  // Size of per-session data
            MAX_JSON_SIZE,           // Receive buffer size (maximum message size)
        },
        { NULL, NULL, 0, 0 }  // End of the list
    };

    info.iface = "192.168.68.84";
    info.port = 9000;  // Port to listen on
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "libwebsockets context creation failed\n");
        return -1;
    }

    printf("WebSocket server started and listening on port 9000...\n");

    while (1) {
        lws_service(context, 50);  // 50ms timeout
    }

    lws_context_destroy(context);

    return 0;
}

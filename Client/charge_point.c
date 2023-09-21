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
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connection established\n");
            connection_established = 1;
            session_data->message_count = 0;  // Initialize message count
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
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

            printf("Received message %d from server: %s\n", session_data->message_count, received_message);
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, "Connection failed\n");
            return -1;

        default:
            break;
    }
    return 0;
}

int main() {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);

    info.port = CONTEXT_PORT_NO_LISTEN;

    static struct lws_protocols protocols[] = {
        {
            "ocpp2.0.1",
            callback,
            sizeof(struct per_session_data__ocpp),
            MAX_JSON_SIZE,
        },
        { NULL, NULL, 0, 0 }  // End of the list
    };

    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    struct lws_client_connect_info ccinfo;
    memset(&ccinfo, 0, sizeof ccinfo);

    ccinfo.context = lws_create_context(&info); // Initialize context here
    ccinfo.address = "ws://192.168.68.84:9000/CP_1";
    ccinfo.port = 9000;
    ccinfo.path = "/CP_1";
    ccinfo.host = ccinfo.address;
    ccinfo.origin = ccinfo.address;
    ccinfo.protocol = "ocpp2.0.1";
    ccinfo.pwsi = &wsi;

    if (!ccinfo.context) { // Check if context initialization failed
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }

    wsi = lws_client_connect_via_info(&ccinfo);
    if (!wsi) {
        fprintf(stderr, "Failed to connect to server\n");
        lws_context_destroy(ccinfo.context); // Cleanup context on failure
        return -1;
    }

    while (lws_service(ccinfo.context, 0) >= 0 && !connection_established) {
        printf("Waiting for connection...\n");
    }

    if (connection_established) {
        // Send OCPP messages here using lws_write
        const char *message = "{\"Payload\": \"YourPayloadData\"}";
        lws_write(wsi, (unsigned char *)message, strlen(message), LWS_WRITE_TEXT);
    }

    lws_context_destroy(ccinfo.context); // Cleanup context

    return 0;
}

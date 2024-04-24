#include <microhttpd.h>
#include <libwebsockets.h>
#include <sqlite3.h>

#define PORT 8888
#define MAX_MESSAGE_SIZE 1024

struct session_data {

};

// WebSocket protocol callbacks
static int websocket_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            // Handle WebSocket connection established
            break;
        case LWS_CALLBACK_RECEIVE:
            // Handle message received from client
            // Broadcast message to all connected clients
            lws_broadcast(wsi, (unsigned char *)in, len, LWS_WRITE_TEXT);
            break;
        default:
            break;
    }
    return 0;
}

int create_new_chat_room_handler(void *cls, struct MHD_Connection *connection,
                                 const char *url, const char *method,
                                 const char *version, const char *upload_data,
                                 size_t *upload_data_size, void **con_cls) {
    // Serve the HTML file for creating a new chat room
    const char *file_path = "/frontend/html/new-chat.html";
    struct MHD_Response *response;
    int ret;

    FILE *file = fopen(file_path, "rb");
    if (!file) {
        // Handle error: file not found
        return MHD_HTTP_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    fclose(file);

    buffer[file_size] = '\0';
    response = MHD_create_response_from_buffer(file_size, buffer, MHD_RESPMEM_MUST_FREE);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int main() {
    struct MHD_Daemon *http_daemon;
    sqlite3 *db;
    struct lws_context *ws_context;

    // Start HTTP server
    http_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                                    &create_new_chat_room_handler, NULL, MHD_OPTION_END);
    if (NULL == http_daemon)
        return 1;

    // Initialize SQLite database
    int rc = sqlite3_open("chat.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Set up libwebsockets context creation info
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = PORT;
    info.protocols = protocols; // Custom protocols if needed
    // Set up other context creation parameters
    // Initialize libwebsockets context
    ws_context = lws_create_context(&info);
    if (ws_context == NULL) {
        fprintf(stderr, "libwebsockets init failed\n");
        return 1;
    }

    // Main loop
    while (1) {
        // Handle WebSocket events
        lws_service(ws_context, 50);

        // Handle other background tasks or events
    }

    // Cleanup
    MHD_stop_daemon(http_daemon);
    sqlite3_close(db);
    lws_context_destroy(ws_context);
    return 0;
}

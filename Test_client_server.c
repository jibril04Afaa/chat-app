#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

GtkWidget *name_entry;
GtkWidget *message_entry;
GtkWidget *message_display;
int sockfd;
char *server_address;
int server_port;

void connect_to_server(GtkWidget *widget, gpointer data);

void *receive_messages(void *data) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message_display));
    while (1) {
        char msg[1024];
        ssize_t bytes_received = recv(sockfd, msg, sizeof(msg), 0);
        if (bytes_received > 0) {
            msg[bytes_received] = '\0';
            gtk_text_buffer_insert_at_cursor(buffer, msg, -1);
            gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
        } else if (bytes_received == 0) {
            break;
        } else {
            perror("Error receiving message");
        }
    }
    return NULL;
}

void send_message(GtkWidget *widget, gpointer data) {
    const gchar *message = gtk_entry_get_text(GTK_ENTRY(message_entry));
    if (strlen(message) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Please enter a message");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    send(sockfd, message, strlen(message), 0);
    gtk_entry_set_text(GTK_ENTRY(message_entry), ""); // Clear the message entry after sending
}

void connect_to_server(GtkWidget *widget, gpointer data) {
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    if (strlen(name) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Please enter your name");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_address);
    serv_addr.sin_port = htons(server_port);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    send(sockfd, name, strlen(name), 0);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Client");
    gtk_widget_set_size_request(window, 600, 300);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    message_display = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(message_display), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), message_display, TRUE, TRUE, 0);

    message_entry = gtk_entry_new();
    GdkColor color;
    gdk_color_parse("orange", &color);
    gtk_widget_modify_base(message_entry, GTK_STATE_NORMAL, &color);
    gtk_entry_set_max_length(GTK_ENTRY(message_entry), 100);
    gtk_box_pack_start(GTK_BOX(vbox), message_entry, FALSE, FALSE, 0);

    GtkWidget *send_button = gtk_button_new_with_label("Send");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), window);
    gtk_box_pack_start(GTK_BOX(vbox), send_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    pthread_t thread;
    if (pthread_create(&thread, NULL, receive_messages, NULL) != 0) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s [server_address] [server_port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_address = argv[1];
    server_port = atoi(argv[2]);

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Enter Your Name");
    gtk_widget_set_size_request(window, 600, 300);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    name_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), name_entry, FALSE, FALSE, 0);

    GtkWidget *connect_button = gtk_button_new_with_label("Connect");
    g_signal_connect(connect_button, "clicked", G_CALLBACK(connect_to_server), window);
    gtk_box_pack_start(GTK_BOX(vbox), connect_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}




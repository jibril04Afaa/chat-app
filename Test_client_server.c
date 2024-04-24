#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

GtkWidget *name_entry;
GtkWidget *message_entry;
GtkWidget *message_display;
int sockfd;
char *server_address;
int server_port;

void connect_to_server(GtkWidget *widget, gpointer data);

void send_message(GtkWidget *widget, gpointer data) {
    const gchar *message = gtk_entry_get_text(GTK_ENTRY(message_entry));
    gtk_entry_set_text(GTK_ENTRY(message_entry), "");

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

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Client - ");
    gtk_widget_set_size_request(window, 600, 300);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 10); // Increased spacing between widgets
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10); // Added border width
    gtk_container_add(GTK_CONTAINER(window), vbox);

    message_display = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), message_display, TRUE, TRUE, 0);

    // Modified text entry fields
    message_entry = gtk_entry_new();
    GdkColor color;
    gdk_color_parse("orange", &color); // Parse the color string to GdkColor
    gtk_widget_modify_base(message_entry, GTK_STATE_NORMAL, &color); // Modify the background color
    gtk_entry_set_max_length(GTK_ENTRY(message_entry), 100); // Set max length of text entry
    gtk_entry_set_width_chars(GTK_ENTRY(message_entry), 40); // Set width of text entry
    gtk_box_pack_start(GTK_BOX(vbox), message_entry, FALSE, FALSE, 0);

    name_entry = gtk_entry_new();
    gtk_widget_modify_base(name_entry, GTK_STATE_NORMAL, &color); // Modify the background color
    gtk_entry_set_max_length(GTK_ENTRY(name_entry), 30); // Set max length of text entry
    gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 20); // Set width of text entry
    gtk_box_pack_start(GTK_BOX(vbox), name_entry, FALSE, FALSE, 0);

    GtkWidget *send_
     = gtk_button_new_with_label("Send");
    // Connect the send_button click signal to the send_message function
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), window);
    gtk_box_pack_start(GTK_BOX(vbox), send_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

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
    gtk_widget_set_size_request(window, 600, 300); // Reduced window size

    GtkWidget *vbox = gtk_vbox_new(FALSE, 10); // Increased spacing between widgets
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10); // Added border width
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Modified text entry fields
    name_entry = gtk_entry_new();
    GdkColor color;
    gdk_color_parse("orange", &color); // Parse the color string to GdkColor
    gtk_widget_modify_base(name_entry, GTK_STATE_NORMAL, &color); // Modify the background color
    gtk_entry_set_max_length(GTK_ENTRY(name_entry), 30); // Set max length of text entry
    gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 20); // Set width of text entry
    gtk_box_pack_start(GTK_BOX(vbox), name_entry, FALSE, FALSE, 0);

    GtkWidget *connect_button = gtk_button_new_with_label("Connect");
    // Connect the connect_button click signal to the connect_to_server function
    g_signal_connect(connect_button, "clicked", G_CALLBACK(connect_to_server), window);
    gtk_box_pack_start(GTK_BOX(vbox), connect_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


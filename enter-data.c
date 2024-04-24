#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>


int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window; // initialize window variable
    GtkWidget *label; // intro text

    GtkWidget *server_address_label;
    GtkWidget *server_address_text_field;
    GtkWidget *port_number_label;
    GtkWidget *port_number_text_field;
    GtkWidget *name;
    GtkWidget *name_field

    GtkWidget *continue_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);


    // default settings
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 650);
    gtk_window_set_title(GTK_WINDOW(window), "Enter some data");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Container to hold widgets
    GtkWidget *widget_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), widget_container);

    // label
    label = gtk_label_new("Enter some data to get started");
    gtk_box_pack_start(GTK_BOX(widget_container), label, FALSE, FALSE, 0);

    // server address
    server_address_label = gtk_label_new("Server Address: ");
    gtk_box_pack_start(GTK_BOX(widget_container), server_address_label, FALSE, FALSE, 0);

    server_address_text_field = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(widget_container), server_address_text_field, FALSE, FALSE, 0);

    // port number
    port_number_label = gtk_label_new("Port Number: ");
    gtk_box_pack_start(GTK_BOX(widget_container), port_number_label, FALSE, FALSE, 0);

    port_number_text_field = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(widget_container), port_number_text_field, FALSE, FALSE, 0);


    // name
    name = gtk_label_new("Name: ");
    gtk_box_pack_start(GTK_BOX(widget_container), name, FALSE, FALSE, 0);

    name_field = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(widget_container), name_field, FALSE, FALSE, 0);


    // continue button - creates a new chat window every time "continue is clicked"
    // void continue() {

    // }

    continue_button = gtk_button_new_with_label("Click To Continue");
    gtk_box_pack_start(GTK_BOX(widget_container), continue_button, FALSE, FALSE, 0);

    // render the window
    gtk_widget_show_all(window);

    // GTK main loop
    gtk_main();

    return 0;
}
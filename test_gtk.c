#include <gtk/gtk.h>

GtkWidget *text_view;

void start_server(GtkWidget *widget, gpointer data) {
    // Start server code here
    g_print("Server started\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Server");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    text_view = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Start Server");
    g_signal_connect(button, "clicked", G_CALLBACK(start_server), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>


int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window; // initialize window variable
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show_all(window);

    // set a default size
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 900);

    // set the default position to be centered
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // creates a callback to close the window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // render the window
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
} 


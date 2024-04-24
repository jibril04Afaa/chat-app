#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>


int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window; // initialize window variable
    GtkWidget *start_button; // initialize start button variable

    GtkWidget *grid_container; // grid container to hold start button



    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // new grid container created
    grid_container = gtk_grid_new();

    // add grid to window
    gtk_container_add(GTK_CONTAINER(window), grid_container);

    // set a default size
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 650);

    // set window title
    gtk_window_set_title(GTK_WINDOW(window), "Chat App");

    // set the default position to be centered
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // creates a callback to close the window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    
    start_button = gtk_button_new_with_label("Click to Start");

    // resize start button
    gtk_widget_set_size_request(start_button, 300, 120);

    // position the start button
    // Make the rows and columns of the grid container homogeneous
    gtk_grid_set_row_homogeneous(GTK_GRID(grid_container), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid_container), TRUE);

    // Attach the button to the center of the grid container
    gtk_grid_attach(GTK_GRID(grid_container), start_button, 0, 0, 1, 1);

    // Align the button within the grid cell
    gtk_widget_set_halign(start_button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(start_button, GTK_ALIGN_CENTER);
    

    void start() {
        system("./enter-data");
    }

    // start button starts the chat when clicked
    g_signal_connect(start_button, "clicked", G_CALLBACK(start), NULL);

    // change button color
    // GdkRGBA text_color;
    // gdk_rgba_parse(&text_color, "green");
    // gtk_widget_override_color(start_button, GTK_STATE_FLAG_NORMAL, &text_color);

    // // add button to container
    // gtk_container_add(GTK_CONTAINER(container), start_button);


    // // add container to the window
    // gtk_container_add(GTK_CONTAINER(window), container);

    // render the window
    gtk_widget_show_all(window);

    // GTK main loop
    gtk_main();



    return 0;
} 


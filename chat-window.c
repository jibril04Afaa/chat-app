#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>


int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window; // initialize window variable
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 

    GtkWidget *chat_text_field; // messages are entered here

    GtkWidget *send_btn;

    int sockfd; // socket file descriptor


    // default settings
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 650);
    gtk_window_set_title(GTK_WINDOW(window), "Chatting Process");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Container to hold widgets
    GtkWidget *widget_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), widget_container);

    // Enter text Text Field
    chat_text_field = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(widget_container), chat_text_field, FALSE, FALSE, 0);

    /*Send button - when send is clicked, it should show the 
    chat in a separate text field above 
    */ 
    void sendFunc() {
        printf("Message sent");
    }

    send_btn = gtk_button_new_with_label("Send");
    
    // event listener for send button
    g_signal_connect(send_btn, "clicked", G_CALLBACK(sendFunc), NULL);

    gtk_box_pack_start(GTK_BOX(widget_container), send_btn, FALSE, FALSE, 0);



    // render the window
    gtk_widget_show_all(window);

    // GTK main loop
    gtk_main();

    return 0;
}
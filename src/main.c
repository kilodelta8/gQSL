#include <gtk/gtk.h>
#include "../include/gui.h"

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    window = create_main_window(app);

    char callsign[20];
    char date[20];
    char time[20];

    get_log_entry_fields(callsign, date, time);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.gqsl", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
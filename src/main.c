#include <gtk/gtk.h>
#include "include/gui.h"
#include "./include/log_manager.h"

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    window = create_main_window(app);

    // Load log entries from file
    if (load_log_entries_from_file("logs.json") != 0) {
        g_print("Error loading log entries.\n");
    }

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.gqsl", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Save logs when the application closes.
    g_application_add_main_quit_handler(G_APPLICATION(app), (GCallback)save_logs, NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static void save_logs(GtkApplication *app, gpointer user_data) {
    save_log_entries_to_file("logs.json");
    g_print("Logs saved.\n");
}
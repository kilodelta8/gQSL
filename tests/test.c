#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *entry;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Test GTK4 App");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    entry = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry), "test");
    gtk_window_set_child(GTK_WINDOW(window), entry);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.test", G_APPLICATION_DEFAULT_FLAGS); // Corrected line
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
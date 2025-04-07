#include <gtk/gtk.h>
#include <glib.h>
// #include <glib-object.h> // No longer needed
#include <gio/gio.h>
#include <stdlib.h>
#include <string.h> // For strerror
#include <errno.h> // For errno

// Adjust include paths based on your project structure
#include "../include/gui.h"
#include "../include/log_manager.h" // Includes LogEntry definition
#include "../include/preferences.h"
#include "../include/adif_handler.h"

// --- Global Variables ---
static char *log_file_path = NULL;

// --- Function Prototypes ---
static const char* get_log_file_path(void);
static void save_logs(void);
static void activate(GtkApplication *app);
static void startup(GApplication *application);
static void apply_dark_mode_preference(void);
static void on_dark_mode_changed(GSettings *settings, gchar *key, gpointer user_data);


// --- Helper Functions ---
static const char* get_log_file_path(void) {
    // (Implementation remains the same)
    if (!log_file_path) {
        const char *data_dir = g_get_user_data_dir();
        if (!data_dir) {
            g_warning("Could not determine user data directory. Using relative path 'logs.json'.");
            log_file_path = g_strdup("logs.json");
        } else {
            log_file_path = g_build_filename(data_dir, "gQSL", "logs.json", NULL);
            char *dir_name = g_path_get_dirname(log_file_path);
            if (dir_name) {
                if (g_mkdir_with_parents(dir_name, 0755) != 0 && errno != EEXIST) {
                     g_warning("Could not create directory '%s': %s", dir_name, strerror(errno));
                }
                g_free(dir_name);
            } else {
                g_warning("Could not get directory name from path: %s", log_file_path);
            }
        }
         g_print("Using log file path: %s\n", log_file_path ? log_file_path : "(null)");
    }
    return log_file_path ? log_file_path : "logs.json";
}

// --- Application Callbacks ---
static void save_logs(void) {
    // (Implementation remains the same)
    const char *path = get_log_file_path();
    if (save_log_entries_to_file(path) != 0) {
         g_warning("Failed to save logs to %s", path);
    } else { g_print("Logs saved to %s.\n", path); }
    prefs_shutdown();
}
static void apply_dark_mode_preference(void) {
    // (Implementation remains the same)
    gboolean use_dark = prefs_get_dark_mode();
    GtkSettings *gtk_settings = gtk_settings_get_default();
    if (gtk_settings) {
        g_object_set(gtk_settings, "gtk-application-prefer-dark-theme", use_dark, NULL);
         g_message("Applied dark mode preference: %s", use_dark ? "TRUE" : "FALSE");
    }
}
static void on_dark_mode_changed(GSettings *settings, gchar *key, gpointer user_data) {
    // (Implementation remains the same)
    (void)settings; (void)key; (void)user_data;
    g_message("Dark mode setting changed, applying...");
    apply_dark_mode_preference();
}

static void activate(GtkApplication *app) {
    // (Implementation remains the same)
    GtkWindow *window;
    const char *path = get_log_file_path();
    if (load_log_entries_from_file(path) == 0) {
        g_print("Successfully processed log file: %s (Count: %d)\n", path, get_log_entry_count());
    } else {
        g_warning("Error loading log entries from %s. Continuing with empty log.", path);
    }
    window = gtk_application_get_active_window(app);
    if (window == NULL) {
         GtkWidget *new_widget = create_main_window(app);
         window = GTK_WINDOW(new_widget);
    }
    // NOTE: Initial list population happens inside create_main_window
    gtk_window_present(window);
}

// Runs once when the application first starts
static void startup(GApplication *application) {
    GtkApplication *app = GTK_APPLICATION(application);
    g_message("Application Startup");
    prefs_init();
    apply_dark_mode_preference();
    prefs_connect_dark_mode_changed(G_CALLBACK(on_dark_mode_changed), NULL);

    // --- Load CSS ---
    GtkCssProvider *provider = gtk_css_provider_new();
    // Simply call load_from_path, don't check void return value
    gtk_css_provider_load_from_path(provider, "data/style.css");
    // We could add a check for the fallback path here if needed, but
    // robust path handling (GResource) is better long term.
    // Report errors via signal connection if necessary.
    // g_signal_connect (provider, "parsing-error", G_CALLBACK (on_css_parsing_error), NULL);

    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    // ---------------

    const GActionEntry app_entries[] = {
        { .name = "import", .activate = action_import_activated },
        { .name = "export", .activate = action_export_activated }
    };
    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);
}


// --- Main Entry Point ---
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // GBoxed Registration REMOVED

    app = gtk_application_new("org.example.gqsl", G_APPLICATION_DEFAULT_FLAGS);
    if (!app) { g_critical("Failed to create GtkApplication."); return 1; }

    g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_signal_connect(app, "shutdown", G_CALLBACK(save_logs), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    g_free(log_file_path);
    log_file_path = NULL;

    return status;
}

/* Final newline added below */

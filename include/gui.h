#ifndef GUI_H
#define GUI_H

#include <glib.h>
#include <gtk/gtk.h>

// LogEntry struct is defined in log_manager.h

// --- Function Prototypes ---

GtkWidget *create_main_window(GtkApplication *app);
void clear_log_entry_fields(void);
void update_log_list_view(void);

// Action Handlers (defined in gui.c, called from main.c's action map)
void action_import_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data);
void action_export_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data);

#endif // GUI_H

/* Final newline added below */

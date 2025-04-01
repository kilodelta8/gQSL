#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h> // Add this line

GtkWidget *create_main_window(GtkApplication *app);
void clear_log_entry_fields(void);
void get_log_entry_fields(char *callsign, char *date, char *time);

#endif // GUI_H
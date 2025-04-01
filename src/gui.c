// gui.c

#include "./include/gui.h"
#include <gtk/gtk.h>
#include <gtk/gtkentry.h> // Include GtkEntry header
#include <string.h>

GtkWidget *callsign_entry;
GtkWidget *date_entry;
GtkWidget *time_entry;

GtkWidget *create_main_window(GtkApplication *app) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *callsign_label, *date_label, *time_label;
  GtkWidget *add_button;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "gQSL");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  callsign_label = gtk_label_new("Callsign:");
  gtk_grid_attach(GTK_GRID(grid), callsign_label, 0, 0, 1, 1);
  callsign_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), callsign_entry, 1, 0, 1, 1);

  date_label = gtk_label_new("Date:");
  gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);
  date_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), date_entry, 1, 1, 1, 1);

  time_label = gtk_label_new("Time:");
  gtk_grid_attach(GTK_GRID(grid), time_label, 0, 2, 1, 1);
  time_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), time_entry, 1, 2, 1, 1);

  add_button = gtk_button_new_with_label("Add Log");
  gtk_grid_attach(GTK_GRID(grid), add_button, 1, 3, 1, 1);

  return window;
}

void clear_log_entry_fields(void) {
  gtk_editable_set_text(GTK_EDITABLE(callsign_entry), "");
  gtk_editable_set_text(GTK_EDITABLE(date_entry), "");
  gtk_editable_set_text(GTK_EDITABLE(time_entry), "");
}

void get_log_entry_fields(char *callsign, char *date, char *time) {
  strcpy(callsign, gtk_editable_get_text(GTK_EDITABLE(callsign_entry)));
  strcpy(date, gtk_editable_get_text(GTK_EDITABLE(date_entry)));
  strcpy(time, gtk_editable_get_text(GTK_EDITABLE(time_entry)));
}
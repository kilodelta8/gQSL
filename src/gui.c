// gui.c

#include "./include/gui.h"
#include <gtk/gtk.h>
#include <gtk/gtkentry.h> // Include GtkEntry header
#include <string.h>

GtkWidget *callsign_entry;
GtkWidget *date_entry;
GtkWidget *time_entry;
GtkWidget *stationCallsign_entry;
GtkWidget *band_entry;
GtkWidget *frequency_entry;
GtkWidget *mode_entry;
GtkWidget *signalReport_entry;

GtkWidget *create_main_window(GtkApplication *app) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *callsign_label, *date_label, *time_label, *stationCallsign_label, *band_label, *frequency_label, *mode_label, *signalReport_label;
  GtkWidget *add_button;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "gQSL");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  stationCallsign_label = gtk_label_new("Station Callsign:"); // This should be a set variable, when the user first sets up the software
  gtk_grid_attach(GTK_GRID(grid), stationCallsign_label, 0, 0, 1, 1); // and it should be their own call sign.
  stationCallsign_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), stationCallsign_entry, 1, 0, 1, 1);

  callsign_label = gtk_label_new("Callsign:");
  gtk_grid_attach(GTK_GRID(grid), callsign_label, 0, 1, 1, 1);
  callsign_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), callsign_entry, 1, 1, 1, 1);

  date_label = gtk_label_new("Date:");
  gtk_grid_attach(GTK_GRID(grid), date_label, 0, 2, 1, 1);
  date_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), date_entry, 1, 2, 1, 1);

  band_label = gtk_label_new("Band:");
  gtk_grid_attach(GTK_GRID(grid), band_label, 0, 3, 1, 1);
  band_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), band_entry, 1, 3, 1, 1);

  frequency_label = gtk_label_new("Frequency:");
  gtk_grid_attach(GTK_GRID(grid), frequency_label, 2, 0, 1, 1);
  frequency_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), frequency_entry, 3, 0, 1, 1);

  mode_label = gtk_label_new("Mode:");
  gtk_grid_attach(GTK_GRID(grid), mode_label, 2, 1, 1, 1);
  mode_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), mode_entry, 3, 1, 1, 1);

  signalReport_label = gtk_label_new("Signal Report:");
  gtk_grid_attach(GTK_GRID(grid), signalReport_label, 2, 2, 1, 1);
  signalReport_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), signalReport_entry, 3, 2, 1, 1);

  time_label = gtk_label_new("Time:");
  gtk_grid_attach(GTK_GRID(grid), time_label, 2, 3, 1, 1);
  time_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), time_entry, 3, 3, 1, 1);

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
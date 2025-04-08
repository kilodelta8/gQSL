#include "../include/gui.h"
#include "../include/log_manager.h"
#include "../include/preferences.h"
#include "../include/adif_handler.h"
#include "../include/log_entry_object.h" // Include the new header
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <gio/gio.h>
// Removed: #include <glib-object.h>

/* --- Global Widget Pointers --- */
GtkWidget *callsign_entry; GtkWidget *date_entry; GtkWidget *time_entry;
GtkWidget *freq_entry; GtkWidget *mode_entry; GtkWidget *rst_sent_entry;
GtkWidget *rst_rcvd_entry; GtkWidget *band_entry; GtkWidget *my_call_entry;
GtkWidget *gridsquare_entry;
GtkWidget *main_stack;

/* --- Static Variables for List View --- */
static GListStore *log_list_store = NULL;
static GtkWidget *log_column_view = NULL;

/* --- Callback Prototypes --- */
static void add_log_entry_callback(void);
static void show_entry_form_callback(void);
static void show_list_view_callback(void);
static void setup_label_cell(GtkListItemFactory *factory, GtkListItem *list_item);
static void bind_callsign_cell(GtkListItemFactory *factory, GtkListItem *list_item);
static void bind_date_cell(GtkListItemFactory *factory, GtkListItem *list_item);
static void bind_time_cell(GtkListItemFactory *factory, GtkListItem *list_item);
static void bind_freq_cell(GtkListItemFactory *factory, GtkListItem *list_item);
static void bind_mode_cell(GtkListItemFactory *factory, GtkListItem *list_item);
static GtkFileFilter* create_adif_file_filter(void);
static void import_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data);
static void export_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data);
static GtkColumnViewColumn* create_label_column(const char *title, GCallback bind_callback);


/* --- List View Factory Callbacks --- */
static void setup_label_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_label_new(NULL); gtk_label_set_xalign(GTK_LABEL(l),0.0); gtk_widget_add_css_class(l,"log_cell"); gtk_list_item_set_child(list_item,l);}
static void bind_callsign_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->call);else gtk_label_set_text(GTK_LABEL(l),"");}
static void bind_date_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->qso_date);else gtk_label_set_text(GTK_LABEL(l),"");}
static void bind_time_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->time_on);else gtk_label_set_text(GTK_LABEL(l),"");}
static void bind_freq_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e){char s[20];snprintf(s,sizeof(s),"%.3f",e->freq);gtk_label_set_text(GTK_LABEL(l),s);}else{gtk_label_set_text(GTK_LABEL(l),"");}}
static void bind_mode_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->mode);else gtk_label_set_text(GTK_LABEL(l),"");}


/* --- Other Callbacks --- */
static void show_entry_form_callback(void) {
    if (main_stack) {
        clear_log_entry_fields();
        GDateTime *now_utc_time=g_date_time_new_now_utc(); char *time_str=g_date_time_format(now_utc_time,"%H%M%S");
        if (time_str && time_entry) gtk_editable_set_text(GTK_EDITABLE(time_entry), time_str);
        g_free(time_str); g_date_time_unref(now_utc_time);
        GDateTime *now_utc_date=g_date_time_new_now_utc(); char *date_str=g_date_time_format(now_utc_date,"%Y%m%d");
        if (date_str && date_entry) gtk_editable_set_text(GTK_EDITABLE(date_entry), date_str);
        g_free(date_str); g_date_time_unref(now_utc_date);
        gchar *my_call = prefs_get_my_callsign();
        if (my_call && my_call_entry) gtk_editable_set_text(GTK_EDITABLE(my_call_entry), my_call);
        g_free(my_call);
        gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "entry_form");
        g_print("Switched to Entry Form\n");
    }
}
static void show_list_view_callback(void) {
     if (main_stack) {
        gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "list_view");
        g_print("Switched to List View\n");
    }
}
static void add_log_entry_callback(void) {
    const char *callsign_text = gtk_editable_get_text(GTK_EDITABLE(callsign_entry));
    if (callsign_text == NULL || strlen(callsign_text) == 0) { g_warning("Callsign cannot be empty."); return; }
    LogEntry *entry_data = create_log_entry(
        gtk_editable_get_text(GTK_EDITABLE(date_entry)), gtk_editable_get_text(GTK_EDITABLE(time_entry)),
        callsign_text, atof(gtk_editable_get_text(GTK_EDITABLE(freq_entry))),
        gtk_editable_get_text(GTK_EDITABLE(mode_entry)), gtk_editable_get_text(GTK_EDITABLE(rst_sent_entry)),
        gtk_editable_get_text(GTK_EDITABLE(rst_rcvd_entry)), gtk_editable_get_text(GTK_EDITABLE(band_entry)),
        gtk_editable_get_text(GTK_EDITABLE(my_call_entry)), gtk_editable_get_text(GTK_EDITABLE(gridsquare_entry))
    );
    if (entry_data) {
        if (add_log_entry(entry_data)) {
             g_print("Log Entry Added to manager: %s\n", entry_data->call);
            if (log_list_store) {
                 LogEntryObject *entry_obj = log_entry_object_new(entry_data);
                 g_list_store_append(log_list_store, entry_obj);
                 g_object_unref(entry_obj);
                 g_print("Log Entry Object Added to UI model: %s\n", entry_data->call);
            }
            show_list_view_callback();
        } else { g_warning("Failed to add log entry to internal list."); }
    } else { g_warning("Failed to create log entry struct."); }
}


/* --- Action Handler Callbacks & Dialog Response Handlers --- */
static GtkFileFilter* create_adif_file_filter(void) {
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "ADIF Files (*.adi)");
    gtk_file_filter_add_pattern(filter, "*.adi"); gtk_file_filter_add_pattern(filter, "*.ADI");
    return filter;
 }
static void import_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data){
     (void)user_data;
     if (response_id == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        GFile *file = gtk_file_chooser_get_file(chooser); // Deprecated
        if (file) {
            char *filepath = g_file_get_path(file);
            if (filepath) {
                g_message("Importing ADIF from: %s", filepath);
                if (adif_import_from_file(filepath)) {
                    g_message("ADIF Import finished. Updating list view...");
                    update_log_list_view(); // Refresh the entire UI list
                } else { g_warning("ADIF Import failed for file: %s", filepath); }
                g_free(filepath);
            }
            g_object_unref(file);
        }
    } else { g_message("ADIF Import cancelled by user."); }
    gtk_window_destroy(GTK_WINDOW(dialog));
 }
static void export_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data){
    (void)user_data;
     if (response_id == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        GFile *file = gtk_file_chooser_get_file(chooser); // Deprecated
        if (file) {
            char *filepath = g_file_get_path(file);
            if (filepath) {
                if (!g_str_has_suffix(filepath, ".adi") && !g_str_has_suffix(filepath, ".ADI")) {
                    char * new_filepath = g_strconcat(filepath, ".adi", NULL);
                    g_free(filepath); filepath = new_filepath;
                }
                g_message("Exporting ADIF to: %s", filepath);
                 if (adif_export_to_file(filepath)) { g_message("ADIF Export successful."); }
                 else { g_warning("ADIF Export failed for file: %s", filepath); }
                g_free(filepath);
            }
            g_object_unref(file);
        }
    } else { g_message("ADIF Export cancelled by user."); }
    gtk_window_destroy(GTK_WINDOW(dialog));
}
void action_import_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){
    (void)action; (void)parameter;
    GtkApplication *app = GTK_APPLICATION(user_data);
    GtkWindow *parent_window = gtk_application_get_active_window(app);
    if (!parent_window) { g_warning("Cannot show import dialog: main window not available."); return; }
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Import ADIF File", parent_window, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = create_adif_file_filter();
    gtk_file_chooser_add_filter(chooser, filter); g_object_unref(filter); // Deprecated
    g_signal_connect(dialog, "response", G_CALLBACK(import_dialog_response), NULL);
    gtk_widget_show(dialog); // Deprecated
 }
void action_export_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){
    (void)action; (void)parameter;
    GtkApplication *app = GTK_APPLICATION(user_data);
    GtkWindow *parent_window = gtk_application_get_active_window(app);
    if (!parent_window) { g_warning("Cannot show export dialog: main window not available."); return; }
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Export ADIF File", parent_window, GTK_FILE_CHOOSER_ACTION_SAVE,
                                                    "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    GtkFileFilter *filter = create_adif_file_filter();
    gtk_file_chooser_add_filter(chooser, filter); g_object_unref(filter); // Deprecated
    gtk_file_chooser_set_current_name(chooser, "gqsl_export.adi"); // Deprecated
    gtk_file_chooser_set_create_folders(chooser, TRUE); // Deprecated
    g_signal_connect(dialog, "response", G_CALLBACK(export_dialog_response), NULL);
    gtk_widget_show(dialog); // Deprecated
 }


/* --- Public Functions --- */
void update_log_list_view(void) {
     if (!log_list_store) { g_warning("List store not initialized, cannot update view."); return; }
    g_list_store_remove_all(log_list_store);
    int count = get_log_entry_count();
    g_print("Updating list view store with %d entries...\n", count);
    for (int i = 0; i < count; i++) {
        LogEntry *entry_data = get_log_entry(i);
        if (entry_data) {
            LogEntryObject *entry_obj = log_entry_object_new(entry_data);
            g_list_store_append(log_list_store, entry_obj);
            g_object_unref(entry_obj);
         }
    }
     g_print("List view store update complete.\n");
}

// Helper function to create columns
static GtkColumnViewColumn* create_label_column(const char *title, GCallback bind_callback) {
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_label_cell), NULL);
    g_signal_connect(factory, "bind", bind_callback, NULL);
    GtkColumnViewColumn *column = gtk_column_view_column_new(title, factory);
    if (!column) {
         g_warning("!!! Failed to create GtkColumnViewColumn object for title '%s' !!!", title);
         return NULL;
    }
    gtk_column_view_column_set_expand(column, TRUE);
    return column;
}

// Function to create the main application window and its contents
GtkWidget *create_main_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "gQSL Log");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    // Header Bar
    GtkWidget *header_bar = gtk_header_bar_new(); gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header_bar), TRUE); gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
    GtkWidget *new_log_button = gtk_button_new_from_icon_name("list-add-symbolic"); gtk_widget_set_tooltip_text(new_log_button, "New Log Entry"); g_signal_connect(new_log_button, "clicked", G_CALLBACK(show_entry_form_callback), NULL); gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), new_log_button);
    GtkWidget *menu_button = gtk_menu_button_new(); gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "open-menu-symbolic"); gtk_widget_set_tooltip_text(menu_button, "Menu"); GMenu *menu_model = g_menu_new(); g_menu_append(menu_model, "Import ADIF File...", "app.import"); g_menu_append(menu_model, "Export ADIF File...", "app.export"); GtkWidget *popover = gtk_popover_menu_new_from_model(G_MENU_MODEL(menu_model)); gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover); g_object_unref(menu_model); gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menu_button);

    // Stack
    main_stack = gtk_stack_new(); gtk_stack_set_transition_type(GTK_STACK(main_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT); gtk_window_set_child(GTK_WINDOW(window), main_stack);

    // View 1: Contact List
    g_type_ensure(GQSL_TYPE_LOG_ENTRY_OBJECT); // Ensure type is registered
    log_list_store = g_list_store_new(GQSL_TYPE_LOG_ENTRY_OBJECT); // Use wrapper type
    GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(log_list_store)); gtk_single_selection_set_autoselect(selection_model, FALSE);
    log_column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection_model)); gtk_widget_set_vexpand(log_column_view, TRUE); gtk_widget_add_css_class(log_column_view, "log_list");
    GtkColumnViewColumn *col;
    col = create_label_column("Callsign", G_CALLBACK(bind_callsign_cell)); if(col)gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Date", G_CALLBACK(bind_date_cell)); if(col)gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Time", G_CALLBACK(bind_time_cell)); if(col)gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Frequency", G_CALLBACK(bind_freq_cell)); if(col)gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Mode", G_CALLBACK(bind_mode_cell)); if(col)gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(); gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), log_column_view); gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_stack_add_named(GTK_STACK(main_stack), scrolled_window, "list_view");
    update_log_list_view(); // Initial population

    // View 2: Entry Form Grid
    GtkWidget *entry_grid = gtk_grid_new();
    gtk_widget_set_margin_start(entry_grid, 10); gtk_widget_set_margin_end(entry_grid, 10); gtk_widget_set_margin_top(entry_grid, 10); gtk_widget_set_margin_bottom(entry_grid, 10); gtk_grid_set_row_spacing(GTK_GRID(entry_grid), 6); gtk_grid_set_column_spacing(GTK_GRID(entry_grid), 6);
    GtkWidget *callsign_label=gtk_label_new("Callsign:"); gtk_widget_set_halign(callsign_label, GTK_ALIGN_END); callsign_entry=gtk_entry_new(); gtk_widget_set_hexpand(callsign_entry, TRUE);
    GtkWidget *date_label=gtk_label_new("Date (YYYYMMDD):"); gtk_widget_set_halign(date_label, GTK_ALIGN_END); date_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(date_entry),"YYYYMMDD");
    GtkWidget *time_label=gtk_label_new("Time (HHMMSS):");   gtk_widget_set_halign(time_label, GTK_ALIGN_END); time_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(time_entry),"HHMMSS or HHMM");
    GtkWidget *freq_label=gtk_label_new("Frequency (MHz):"); gtk_widget_set_halign(freq_label, GTK_ALIGN_END); freq_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(freq_entry),"e.g., 14.230");
    GtkWidget *mode_label=gtk_label_new("Mode:");            gtk_widget_set_halign(mode_label, GTK_ALIGN_END); mode_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(mode_entry),"e.g., SSB, CW, FT8");
    GtkWidget *rst_sent_label=gtk_label_new("RST Sent:");    gtk_widget_set_halign(rst_sent_label, GTK_ALIGN_END); rst_sent_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(rst_sent_entry),"e.g., 599");
    GtkWidget *rst_rcvd_label=gtk_label_new("RST Received:"); gtk_widget_set_halign(rst_rcvd_label, GTK_ALIGN_END); rst_rcvd_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(rst_rcvd_entry),"e.g., 59");
    GtkWidget *band_label=gtk_label_new("Band:");            gtk_widget_set_halign(band_label, GTK_ALIGN_END); band_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(band_entry),"e.g., 20m, 40m");
    GtkWidget *my_call_label=gtk_label_new("My Callsign:");   gtk_widget_set_halign(my_call_label, GTK_ALIGN_END); my_call_entry=gtk_entry_new();
    GtkWidget *gridsquare_label=gtk_label_new("Grid Square:"); gtk_widget_set_halign(gridsquare_label, GTK_ALIGN_END); gridsquare_entry=gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(gridsquare_entry),"e.g., FN31");
    GtkWidget *add_button=gtk_button_new_with_label("Add Log Entry"); gtk_widget_set_hexpand(add_button, TRUE); g_signal_connect(add_button, "clicked", G_CALLBACK(add_log_entry_callback), NULL);
    gtk_grid_attach(GTK_GRID(entry_grid), callsign_label, 0,0,1,1); gtk_grid_attach(GTK_GRID(entry_grid), callsign_entry, 1,0,1,1); gtk_grid_attach(GTK_GRID(entry_grid), freq_label, 2,0,1,1); gtk_grid_attach(GTK_GRID(entry_grid), freq_entry, 3,0,1,1);
    gtk_grid_attach(GTK_GRID(entry_grid), date_label, 0,1,1,1); gtk_grid_attach(GTK_GRID(entry_grid), date_entry, 1,1,1,1); gtk_grid_attach(GTK_GRID(entry_grid), mode_label, 2,1,1,1); gtk_grid_attach(GTK_GRID(entry_grid), mode_entry, 3,1,1,1);
    gtk_grid_attach(GTK_GRID(entry_grid), time_label, 0,2,1,1); gtk_grid_attach(GTK_GRID(entry_grid), time_entry, 1,2,1,1); gtk_grid_attach(GTK_GRID(entry_grid), band_label, 2,2,1,1); gtk_grid_attach(GTK_GRID(entry_grid), band_entry, 3,2,1,1);
    gtk_grid_attach(GTK_GRID(entry_grid), rst_sent_label, 0,3,1,1); gtk_grid_attach(GTK_GRID(entry_grid), rst_sent_entry, 1,3,1,1); gtk_grid_attach(GTK_GRID(entry_grid), rst_rcvd_label, 2,3,1,1); gtk_grid_attach(GTK_GRID(entry_grid), rst_rcvd_entry, 3,3,1,1);
    gtk_grid_attach(GTK_GRID(entry_grid), my_call_label, 0,4,1,1); gtk_grid_attach(GTK_GRID(entry_grid), my_call_entry, 1,4,1,1); gtk_grid_attach(GTK_GRID(entry_grid), gridsquare_label, 2,4,1,1); gtk_grid_attach(GTK_GRID(entry_grid), gridsquare_entry, 3,4,1,1);
    gtk_grid_attach(GTK_GRID(entry_grid), add_button, 0,5,4,1);
    gtk_stack_add_named(GTK_STACK(main_stack), entry_grid, "entry_form");

    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "list_view");
    return window;
}

void clear_log_entry_fields(void) { /* ... */ }
void get_log_entry_fields(char *callsign, char *date, char *time) { /* ... */ }

/* Final newline added below */

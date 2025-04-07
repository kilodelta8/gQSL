#include "../include/gui.h"
#include "../include/log_manager.h"
#include "../include/preferences.h"
#include "../include/adif_handler.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <gio/gio.h>
#include <glib-object.h> // Needed for GObject definition

/* --- GObject Wrapper for LogEntry --- */
typedef struct { GObject parent_instance; LogEntry *log_entry_data; } LogEntryObject;
typedef struct { GObjectClass parent_class; } LogEntryObjectClass;
#define GQSL_TYPE_LOG_ENTRY_OBJECT (log_entry_object_get_type())
G_DEFINE_TYPE(LogEntryObject, log_entry_object, G_TYPE_OBJECT)
static void log_entry_object_init(LogEntryObject *self) { self->log_entry_data = NULL; }
static void log_entry_object_finalize(GObject *object) {
    LogEntryObject *self = (LogEntryObject*)object;
    self->log_entry_data = NULL; // Wrapper doesn't own data from log_manager's static array
    G_OBJECT_CLASS(log_entry_object_parent_class)->finalize(object);
}
static void log_entry_object_class_init(LogEntryObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = log_entry_object_finalize;
}
static LogEntryObject* log_entry_object_new(LogEntry *entry) {
    LogEntryObject *self = g_object_new(GQSL_TYPE_LOG_ENTRY_OBJECT, NULL);
    self->log_entry_data = entry;
    return self;
}
static LogEntry* log_entry_object_get_entry(LogEntryObject *self) {
    if (!self || !G_IS_OBJECT(self)) return NULL;
    // g_return_val_if_fail(GQSL_IS_LOG_ENTRY_OBJECT(self), NULL); // Optional safety check
    return self->log_entry_data;
}
/* --- End GObject Wrapper --- */


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


/* --- List View Factory Callbacks --- */
static void setup_label_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_label_new(NULL); gtk_label_set_xalign(GTK_LABEL(l),0.0); gtk_widget_add_css_class(l,"log_cell"); gtk_list_item_set_child(list_item,l);}
static void bind_callsign_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->call);else gtk_label_set_text(GTK_LABEL(l),"");}
static void bind_date_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->qso_date);else gtk_label_set_text(GTK_LABEL(l),"");}
static void bind_time_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->time_on);else gtk_label_set_text(GTK_LABEL(l),"");}
static void bind_freq_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e){char s[20];snprintf(s,sizeof(s),"%.3f",e->freq);gtk_label_set_text(GTK_LABEL(l),s);}else{gtk_label_set_text(GTK_LABEL(l),"");}}
static void bind_mode_cell(GtkListItemFactory *factory, GtkListItem *list_item){(void)factory; GtkWidget *l=gtk_list_item_get_child(list_item); LogEntryObject *o=gtk_list_item_get_item(list_item); LogEntry *e=o?log_entry_object_get_entry(o):NULL; if(e)gtk_label_set_text(GTK_LABEL(l),e->mode);else gtk_label_set_text(GTK_LABEL(l),"");}


/* --- Other Callbacks --- */
static void show_entry_form_callback(void) { /* ... implementation same ... */ }
static void show_list_view_callback(void) { /* ... implementation same ... */ }
static void add_log_entry_callback(void) { /* ... implementation same ... */ }


/* --- Action Handler Callbacks & Dialog Response Handlers --- */
static GtkFileFilter* create_adif_file_filter(void) { /* ... implementation same ... */ }
static void import_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data){ /* ... implementation same ... */ }
static void export_dialog_response(GtkDialog *dialog, int response_id, gpointer user_data){ /* ... implementation same ... */ }
void action_import_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){ /* ... implementation same ... */ }
void action_export_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data){ /* ... implementation same ... */ }


/* --- Public Functions --- */
void update_log_list_view(void) {
    // (Implementation remains the same)
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

// Helper function to create columns (remains the same)
static GtkColumnViewColumn* create_label_column(const char *title, GCallback bind_callback) { /* ... implementation same ... */ }

// Function to create the main application window and its contents
GtkWidget *create_main_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "gQSL Log");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    // Header Bar (remains the same)
    GtkWidget *header_bar = gtk_header_bar_new(); /* ... */ gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
    GtkWidget *new_log_button = gtk_button_new_from_icon_name("list-add-symbolic"); /* ... */ gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), new_log_button);
    GtkWidget *menu_button = gtk_menu_button_new(); /* ... */ GMenu *menu_model = g_menu_new(); /* ... */ GtkWidget *popover = gtk_popover_menu_new_from_model(G_MENU_MODEL(menu_model)); /* ... */ gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover); /* ... */ gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menu_button);

    // Stack (remains the same)
    main_stack = gtk_stack_new(); /* ... */ gtk_window_set_child(GTK_WINDOW(window), main_stack);

    // --- View 1: Contact List ---

    // *** Ensure the GObject type is initialized BEFORE creating the store ***
    g_type_ensure(GQSL_TYPE_LOG_ENTRY_OBJECT);
    // ************************************************************************

    log_list_store = g_list_store_new(GQSL_TYPE_LOG_ENTRY_OBJECT); // Use GObject type

    GtkSingleSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(log_list_store));
    gtk_single_selection_set_autoselect(selection_model, FALSE);

    log_column_view = gtk_column_view_new(GTK_SELECTION_MODEL(selection_model));
    gtk_widget_set_vexpand(log_column_view, TRUE);
    gtk_widget_add_css_class(log_column_view, "log_list");

    // Create and append columns (remains the same)
    GtkColumnViewColumn *col;
    col = create_label_column("Callsign", G_CALLBACK(bind_callsign_cell)); gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Date", G_CALLBACK(bind_date_cell)); gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Time", G_CALLBACK(bind_time_cell)); gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Frequency", G_CALLBACK(bind_freq_cell)); gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    col = create_label_column("Mode", G_CALLBACK(bind_mode_cell)); gtk_column_view_append_column(GTK_COLUMN_VIEW(log_column_view), col);
    // TODO: Add more columns

    GtkWidget *scrolled_window = gtk_scrolled_window_new(); /* ... */ gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), log_column_view); /* ... */
    gtk_stack_add_named(GTK_STACK(main_stack), scrolled_window, "list_view");

    // --- Call update_log_list_view HERE --- (Remains the same place)
    update_log_list_view();

    // --- View 2: Entry Form Grid (Setup remains the same) ---
    GtkWidget *entry_grid = gtk_grid_new(); /* ... */
    gtk_stack_add_named(GTK_STACK(main_stack), entry_grid, "entry_form");

    // --- Set Initial View --- (Remains the same)
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "list_view");

    return window;
}


// Function to clear the entry fields (remains the same)
void clear_log_entry_fields(void) { /* ... */ }
// Function to get data from entry fields (remains the same)
void get_log_entry_fields(char *callsign, char *date, char *time) { /* ... */ }

/* Final newline added below */

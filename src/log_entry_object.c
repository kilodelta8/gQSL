#include "../include/log_entry_object.h" // Adjust path if needed

// --- GObject Implementation ---

// Instance struct definition
struct _LogEntryObject {
    GObject parent_instance;
    LogEntry *log_entry_data; // Pointer to the actual LogEntry struct
};

// G_DEFINE_TYPE macro defines necessary functions like _get_type, _class_init, _init
// and the parent class pointer (log_entry_object_parent_class)
G_DEFINE_TYPE(LogEntryObject, log_entry_object, G_TYPE_OBJECT)

// Initialization function for the object instance
static void log_entry_object_init(LogEntryObject *self) {
    self->log_entry_data = NULL;
}

// Finalization function (called when the object is destroyed)
static void log_entry_object_finalize(GObject *object) {
    LogEntryObject *self = (LogEntryObject*)object;

    // Since log_manager owns the actual LogEntry structs in its static array,
    // the wrapper object should NOT free log_entry_data here.
    g_print("Finalizing LogEntryObject wrapper for entry: %s\n",
            self->log_entry_data ? self->log_entry_data->call : "(null)");
    self->log_entry_data = NULL;

    // Always chain up to the parent class finalize method
    G_OBJECT_CLASS(log_entry_object_parent_class)->finalize(object);
}

// Class initialization function (sets up the class)
static void log_entry_object_class_init(LogEntryObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = log_entry_object_finalize;
}

// Public constructor
LogEntryObject* log_entry_object_new(LogEntry *entry) {
    LogEntryObject *self = g_object_new(GQSL_TYPE_LOG_ENTRY_OBJECT, NULL);
    self->log_entry_data = entry;
    return self;
}

// Public getter
LogEntry* log_entry_object_get_entry(LogEntryObject *self) {
    // Use generated IS_ macro for safety check
    g_return_val_if_fail(GQSL_IS_LOG_ENTRY_OBJECT(self), NULL);
    return self->log_entry_data;
}

/* Final newline added below */
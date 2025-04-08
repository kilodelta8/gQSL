#ifndef GQSL_LOG_ENTRY_OBJECT_H
#define GQSL_LOG_ENTRY_OBJECT_H

#include <glib-object.h>
#include "./log_manager.h" // For LogEntry definition

// --- Type Declaration ---

// Macro to get the GType for our object
#define GQSL_TYPE_LOG_ENTRY_OBJECT (log_entry_object_get_type())

// Declare the object struct (details are private in .c)
typedef struct _LogEntryObject LogEntryObject;

// GObject macros declare helper functions/macros like GQSL_IS_LOG_ENTRY_OBJECT() etc.
G_DECLARE_FINAL_TYPE(LogEntryObject, log_entry_object, GQSL, LOG_ENTRY_OBJECT, GObject)


// --- Public Function Prototypes ---

// Constructor: Creates a new wrapper object holding the LogEntry pointer.
LogEntryObject* log_entry_object_new(LogEntry *entry);

// Getter: Retrieves the LogEntry pointer from the wrapper object.
LogEntry* log_entry_object_get_entry(LogEntryObject *self);


#endif // GQSL_LOG_ENTRY_OBJECT_H

/* Final newline added below */

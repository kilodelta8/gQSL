#include "../include/log_manager.h" // Adjust path if needed
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>     // For g_warning, g_strdup, g_free etc.
#include <errno.h>    // For errno with file operations

#define MAX_LOG_ENTRIES 100 // Example static limit - consider dynamic allocation later

// Internal storage for log entries
static LogEntry *log_entries[MAX_LOG_ENTRIES];
static int log_entry_count = 0;

// Helper function to safely get string from JSON object
static const char* json_object_get_string_safe(json_t *obj, const char *key) {
    json_t *value = json_object_get(obj, key);
    if (json_is_string(value)) {
        return json_string_value(value);
    }
    g_warning("JSON load: Expected string for key '%s', but got different type or NULL.", key);
    return ""; // Return empty string on error/mismatch
}

// Helper function to safely get float from JSON object
static float json_object_get_real_safe(json_t *obj, const char *key) {
    json_t *value = json_object_get(obj, key);
    if (json_is_real(value) || json_is_integer(value)) { // Accept integer as float too
        return json_number_value(value);
    }
     g_warning("JSON load: Expected number for key '%s', but got different type or NULL.", key);
    return 0.0f; // Return 0.0 on error/mismatch
}


LogEntry *create_log_entry(const char *qso_date, const char *time_on, const char *call,
                           float freq, const char *mode, const char *rst_sent,
                           const char *rst_rcvd, const char *band,
                           const char *my_call, const char *gridsquare)
{
    // Note: No size check here, assuming caller checks or add_log_entry handles limit
    LogEntry *entry = (LogEntry *)g_malloc(sizeof(LogEntry)); // Use g_malloc for consistency
    if (!entry) {
         g_warning("Failed to allocate memory for LogEntry.");
         return NULL;
    }

    // Use g_strlcpy for safer string copying
    g_strlcpy(entry->qso_date, qso_date ? qso_date : "", sizeof(entry->qso_date));
    g_strlcpy(entry->time_on, time_on ? time_on : "", sizeof(entry->time_on));
    g_strlcpy(entry->call, call ? call : "", sizeof(entry->call));
    entry->freq = freq;
    g_strlcpy(entry->mode, mode ? mode : "", sizeof(entry->mode));
    g_strlcpy(entry->rst_sent, rst_sent ? rst_sent : "", sizeof(entry->rst_sent));
    g_strlcpy(entry->rst_rcvd, rst_rcvd ? rst_rcvd : "", sizeof(entry->rst_rcvd));
    g_strlcpy(entry->band, band ? band : "", sizeof(entry->band));
    g_strlcpy(entry->my_call, my_call ? my_call : "", sizeof(entry->my_call));
    g_strlcpy(entry->gridsquare, gridsquare ? gridsquare : "", sizeof(entry->gridsquare));

    return entry;
}

void free_log_entry(LogEntry *entry) {
    if (entry) {
        g_free(entry); // Use g_free with g_malloc
    }
}

// Clears all log entries from memory
void clear_all_log_entries(void) {
    for (int i = 0; i < log_entry_count; i++) {
        if (log_entries[i]) {
            free_log_entry(log_entries[i]);
            log_entries[i] = NULL;
        }
    }
    log_entry_count = 0;
    g_print("Cleared all in-memory log entries.\n");
}


gboolean add_log_entry(LogEntry *entry) {
    if (!entry) {
        g_warning("Attempted to add NULL log entry.");
        return FALSE;
    }
    if (log_entry_count >= MAX_LOG_ENTRIES) {
        g_warning("Log entry list is full (Max: %d). Cannot add new entry.", MAX_LOG_ENTRIES);
        free_log_entry(entry); // Free the entry that couldn't be added
        return FALSE;
    }
    log_entries[log_entry_count++] = entry; // Takes ownership
    return TRUE;
}

gboolean remove_log_entry(LogEntry *entry_to_remove) {
     if (!entry_to_remove) return FALSE;

     int found_index = -1;
     for (int i = 0; i < log_entry_count; i++) {
         if (log_entries[i] == entry_to_remove) {
             found_index = i;
             break;
         }
     }

     if (found_index != -1) {
         // Free the memory for the found entry
         free_log_entry(log_entries[found_index]);

         // Shift subsequent entries down
         for (int i = found_index; i < log_entry_count - 1; i++) {
             log_entries[i] = log_entries[i + 1];
         }

         // Null out the last pointer and decrement count
         log_entries[log_entry_count - 1] = NULL;
         log_entry_count--;
         g_print("Removed log entry at index %d.\n", found_index);
         return TRUE;
     } else {
         g_warning("Attempted to remove log entry that was not found in the list.");
         return FALSE;
     }
}


LogEntry *get_log_entry(int index) {
    if (index >= 0 && index < log_entry_count) {
        return log_entries[index];
    }
    g_warning("Attempted to get log entry with invalid index: %d (Count: %d)", index, log_entry_count);
    return NULL;
}

int get_log_entry_count(void) {
    return log_entry_count;
}

int load_log_entries_from_file(const char *filename) {
    if (!filename || filename[0] == '\0') {
        g_warning("Load error: Invalid filename provided.");
        return 1;
    }

    // Clear existing entries before loading
    clear_all_log_entries();

    json_error_t error;
    // Use JSON_DECODE_ANY to allow reading empty files without error
    json_t *root = json_load_file(filename, JSON_DECODE_ANY, &error);

    if (!root) {
        // Check if it's just a file-not-found error, which might be okay on first launch
        if (errno == ENOENT) {
             g_print("Log file '%s' not found. Starting with empty log.\n", filename);
             return 0; // Not necessarily an error if file doesn't exist yet
        } else {
            g_warning("Failed to load or parse '%s': %s (Line: %d, Col: %d, Pos: %d)",
                      filename, error.text, error.line, error.column, error.position);
            return 1; // Indicate a loading/parsing error
        }
    }

    // Handle case where file exists but is empty or contains non-array JSON (e.g., null)
    if (!json_is_array(root)) {
         if (json_is_null(root)) {
            g_print("Log file '%s' contained null. Starting with empty log.\n", filename);
            json_decref(root);
            return 0; // Empty log is fine
         } else {
            g_warning("JSON load error: Root element in '%s' is not an array.", filename);
            json_decref(root);
            return 2; // Indicate wrong JSON structure
         }
    }

    size_t index;
    json_t *value;
    g_print("Loading log entries from '%s'...\n", filename);
    json_array_foreach(root, index, value) {
        if (log_entry_count >= MAX_LOG_ENTRIES) {
             g_warning("Reached maximum log entries (%d) while loading. Stopping load.", MAX_LOG_ENTRIES);
             break; // Stop loading if we hit the limit
        }
        if (!json_is_object(value)) {
             g_warning("JSON load warning: Skipping non-object item at index %zu.", index);
            continue;
        } // Brace for `if (!json_is_object)` - was correct before

        // Use g_malloc0 to zero-initialize the struct, preventing issues with strcpy if JSON fields are missing
        LogEntry *entry = (LogEntry *)g_malloc0(sizeof(LogEntry));
        if (!entry) {
             g_warning("JSON load error: Failed to allocate memory for entry at index %zu.", index);
            continue;
        } // <-- *** Added missing closing brace here ***

        // Use helper functions for safer extraction
        g_strlcpy(entry->qso_date, json_object_get_string_safe(value, "qso_date"), sizeof(entry->qso_date));
        g_strlcpy(entry->time_on, json_object_get_string_safe(value, "time_on"), sizeof(entry->time_on));
        g_strlcpy(entry->call, json_object_get_string_safe(value, "call"), sizeof(entry->call));
        entry->freq = json_object_get_real_safe(value, "freq");
        g_strlcpy(entry->mode, json_object_get_string_safe(value, "mode"), sizeof(entry->mode));
        g_strlcpy(entry->rst_sent, json_object_get_string_safe(value, "rst_sent"), sizeof(entry->rst_sent));
        g_strlcpy(entry->rst_rcvd, json_object_get_string_safe(value, "rst_rcvd"), sizeof(entry->rst_rcvd));
        g_strlcpy(entry->band, json_object_get_string_safe(value, "band"), sizeof(entry->band));
        g_strlcpy(entry->my_call, json_object_get_string_safe(value, "my_call"), sizeof(entry->my_call));
        g_strlcpy(entry->gridsquare, json_object_get_string_safe(value, "gridsquare"), sizeof(entry->gridsquare));

        // Add the successfully parsed entry (manage directly)
        log_entries[log_entry_count++] = entry;
    } // End of json_array_foreach loop

    json_decref(root);
    g_print("Loaded %d log entries.\n", log_entry_count);
    return 0; // Success
}

int save_log_entries_to_file(const char *filename) {
     if (!filename || filename[0] == '\0') {
        g_warning("Save error: Invalid filename provided.");
        return -1;
    }

    json_t *root = json_array();
    if (!root) {
        g_warning("Save error: Failed to create JSON array.");
        return -1;
    }

    for (int i = 0; i < log_entry_count; i++) {
        if (!log_entries[i]) continue; // Should not happen, but safety check

        json_t *entry_obj = json_object();
        if (!entry_obj) {
            g_warning("Save error: Failed to create JSON object for entry %d.", i);
            continue; // Skip this entry if object creation fails
        }

        // Use json_object_set_new which handles reference counting for new JSON values
        json_object_set_new(entry_obj, "qso_date", json_string(log_entries[i]->qso_date));
        json_object_set_new(entry_obj, "time_on", json_string(log_entries[i]->time_on));
        json_object_set_new(entry_obj, "call", json_string(log_entries[i]->call));
        json_object_set_new(entry_obj, "freq", json_real(log_entries[i]->freq));
        json_object_set_new(entry_obj, "mode", json_string(log_entries[i]->mode));
        json_object_set_new(entry_obj, "rst_sent", json_string(log_entries[i]->rst_sent));
        json_object_set_new(entry_obj, "rst_rcvd", json_string(log_entries[i]->rst_rcvd));
        json_object_set_new(entry_obj, "band", json_string(log_entries[i]->band));
        json_object_set_new(entry_obj, "my_call", json_string(log_entries[i]->my_call));
        json_object_set_new(entry_obj, "gridsquare", json_string(log_entries[i]->gridsquare));

        if (json_array_append_new(root, entry_obj) != 0) {
             g_warning("Save error: Failed to append JSON object for entry %d to array.", i);
             json_decref(entry_obj); // Decref the object if append failed
        }
    }

    g_print("Saving %d log entries to '%s'...\n", log_entry_count, filename);
    // Use JSON_ENSURE_ASCII disabled for broader compatibility, and JSON_SORT_KEYS for consistent output
    int dump_result = json_dump_file(root, filename, JSON_INDENT(4) | JSON_ENSURE_ASCII);

    json_decref(root); // Decrement root array ref count

    if (dump_result != 0) {
        g_warning("Failed to write JSON log file '%s': %s", filename, strerror(errno));
        return -1; // Indicate save error
    }

    g_print("Successfully saved logs.\n");
    return 0; // Success
}

// Placeholder - Implement search logic based on specific needs (e.g., search by callsign)
LogEntry **search_log_entries(const char *search_term, int *result_count) {
    // g_warning("Search function not yet implemented.");
    // Remove unused parameter warnings if not implementing yet
    (void)search_term;
    (void)result_count;
    if (result_count) *result_count = 0;
    return NULL;
}

/* Final newline added below */

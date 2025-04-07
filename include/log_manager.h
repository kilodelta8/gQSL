#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <glib.h> // For gboolean
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- LogEntry Definition --- (Defined ONLY here)
typedef struct {
    char qso_date[12]; // YYYYMMDD
    char time_on[7];   // HHMMSS
    char call[20];
    float freq;
    char mode[10];
    char rst_sent[4];
    char rst_rcvd[4];
    char band[10];
    char my_call[20];
    char gridsquare[10];
    // Add other fields as needed
} LogEntry;

// --- Function Prototypes ---

LogEntry* create_log_entry(const char *qso_date, const char *time_on, const char *call,
                           float freq, const char *mode, const char *rst_sent,
                           const char *rst_rcvd, const char *band,
                           const char *my_call, const char *gridsquare);
void free_log_entry(LogEntry *entry);
gboolean add_log_entry(LogEntry *entry);
gboolean remove_log_entry(LogEntry *entry_to_remove);
LogEntry* get_log_entry(int index);
int get_log_entry_count(void);
int save_log_entries_to_file(const char *filename);
int load_log_entries_from_file(const char *filename);
LogEntry** search_log_entries(const char *search_term, int *result_count); // Keep prototype params
void clear_all_log_entries(void);

#endif // LOG_MANAGER_H

/* Final newline added below */

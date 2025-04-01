#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    // Add more fields later
} LogEntry;



LogEntry* create_log_entry(const char *qso_date, const char *time_on, const char *call, float freq, const char *mode, const char *rst_sent, const char *rst_rcvd, const char *band, const char *my_call, const char *gridsquare);
void free_log_entry(LogEntry *entry);
void add_log_entry(LogEntry *entry);
void remove_log_entry(LogEntry *entry);
LogEntry* get_log_entry(int index);
int get_log_entry_count(void);
void save_log_entries_to_file(const char *filename);
int load_log_entries_from_file(const char *filename); // added return value for error checking
LogEntry** search_log_entries(const char *search_term, int *result_count);


#endif // LOG_MANAGER_H
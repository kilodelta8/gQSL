#include "./include/log_manager.h"
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_ENTRIES 100

static LogEntry *log_entries[MAX_LOG_ENTRIES];
static int log_entry_count = 0;

LogEntry *create_log_entry(const char *qso_date, const char *time_on, const char *call, float freq, const char *mode, const char *rst_sent, const char *rst_rcvd, const char *band, const char *my_call, const char *gridsquare) {
    if (log_entry_count >= MAX_LOG_ENTRIES) {
        return NULL;
    }
    LogEntry *entry = (LogEntry *)malloc(sizeof(LogEntry));
    if (entry) {
        strcpy(entry->qso_date, qso_date);
        strcpy(entry->time_on, time_on);
        strcpy(entry->call, call);
        entry->freq = freq;
        strcpy(entry->mode, mode);
        strcpy(entry->rst_sent, rst_sent);
        strcpy(entry->rst_rcvd, rst_rcvd);
        strcpy(entry->band, band);
        strcpy(entry->my_call, my_call);
        strcpy(entry->gridsquare, gridsquare);
    }
    return entry;
}

void free_log_entry(LogEntry *entry) {
    if (entry) {
        free(entry);
    }
}

void add_log_entry(LogEntry *entry) {
    if (log_entry_count < MAX_LOG_ENTRIES && entry) {
        log_entries[log_entry_count++] = entry;
    }
}

void remove_log_entry(LogEntry *entry) {
    // Implement removal logic here
    // ...
}

LogEntry *get_log_entry(int index) {
    if (index >= 0 && index < log_entry_count) {
        return log_entries[index];
    }
    return NULL;
}

int get_log_entry_count(void) {
    return log_entry_count;
}

int load_log_entries_from_file(const char *filename) {
    json_error_t error;
    json_t *root = json_load_file(filename, 0, &error);
    if (!root) {
        return 1; // Error loading JSON
    }

    if (!json_is_array(root)) {
        json_decref(root);
        return 2; // JSON is not an array
    }

    size_t index;
    json_t *value;
    json_array_foreach(root, index, value) {
        if (!json_is_object(value)) {
            continue;
        }

        LogEntry *entry = (LogEntry *)malloc(sizeof(LogEntry));
        if (!entry) {
            continue;
        }

        strcpy(entry->qso_date, json_string_value(json_object_get(value, "qso_date")));
        strcpy(entry->time_on, json_string_value(json_object_get(value, "time_on")));
        strcpy(entry->call, json_string_value(json_object_get(value, "call")));
        entry->freq = json_real_value(json_object_get(value, "freq"));
        strcpy(entry->mode, json_string_value(json_object_get(value, "mode")));
        strcpy(entry->rst_sent, json_string_value(json_object_get(value, "rst_sent")));
        strcpy(entry->rst_rcvd, json_string_value(json_object_get(value, "rst_rcvd")));
        strcpy(entry->band, json_string_value(json_object_get(value, "band")));
        strcpy(entry->my_call, json_string_value(json_object_get(value, "my_call")));
        strcpy(entry->gridsquare, json_string_value(json_object_get(value, "gridsquare")));

        log_entries[log_entry_count++] = entry;
    }

    json_decref(root);
    return 0; // Success
}

void save_log_entries_to_file(const char *filename) {
    json_t *root = json_array();
    for (int i = 0; i < log_entry_count; i++) {
        json_t *entry = json_object();
        json_object_set_new(entry, "qso_date", json_string(log_entries[i]->qso_date));
        json_object_set_new(entry, "time_on", json_string(log_entries[i]->time_on));
        json_object_set_new(entry, "call", json_string(log_entries[i]->call));
        json_object_set_new(entry, "freq", json_real(log_entries[i]->freq));
        json_object_set_new(entry, "mode", json_string(log_entries[i]->mode));
        json_object_set_new(entry, "rst_sent", json_string(log_entries[i]->rst_sent));
        json_object_set_new(entry, "rst_rcvd", json_string(log_entries[i]->rst_rcvd));
        json_object_set_new(entry, "band", json_string(log_entries[i]->band));
        json_object_set_new(entry, "my_call", json_string(log_entries[i]->my_call));
        json_object_set_new(entry, "gridsquare", json_string(log_entries[i]->gridsquare));
        json_array_append_new(root, entry);
    }

    json_dump_file(root, filename, JSON_INDENT(4));
    json_decref(root);
}

LogEntry **search_log_entries(const char *search_term, int *result_count) {
    // Implement search logic here
    // ...
    return NULL;
}
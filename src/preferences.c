#include "../include/preferences.h" // Adjust path if needed
#include <gio/gio.h> // For GSettings

// Schema ID must match the one in the XML file and meson.build
#define GQSL_SETTINGS_SCHEMA "org.example.gqsl"

static GSettings *settings = NULL;

void prefs_init(void) {
    if (settings == NULL) {
        settings = g_settings_new(GQSL_SETTINGS_SCHEMA);
        if (!settings) {
            g_critical("Failed to initialize GSettings with schema '%s'. Ensure schema is installed correctly.", GQSL_SETTINGS_SCHEMA);
        } else {
            g_message("GSettings initialized successfully for schema '%s'.", GQSL_SETTINGS_SCHEMA);
        }
    }
}

void prefs_shutdown(void) {
    if (settings != NULL) {
        g_object_unref(settings);
        settings = NULL;
        g_message("GSettings shut down.");
    }
}

// --- Getters ---

gboolean prefs_get_dark_mode(void) {
    if (!settings) return FALSE;
    return g_settings_get_boolean(settings, "dark-mode");
}

gboolean prefs_get_contest_mode(void) {
    if (!settings) return FALSE;
    return g_settings_get_boolean(settings, "contest-mode");
}

// String getters return new memory (must be g_free'd)
gchar* prefs_get_my_callsign(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-callsign");
}

gchar* prefs_get_my_name(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-name");
}

gchar* prefs_get_my_gridsquare(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-gridsquare");
}

gdouble prefs_get_my_latitude(void) {
    if (!settings) return 0.0;
    return g_settings_get_double(settings, "my-latitude");
}

gdouble prefs_get_my_longitude(void) {
    if (!settings) return 0.0;
    return g_settings_get_double(settings, "my-longitude");
}

gint prefs_get_my_cq_zone(void) {
    if (!settings) return 0;
    return g_settings_get_int(settings, "my-cq-zone");
}

gint prefs_get_my_itu_zone(void) {
    if (!settings) return 0;
    return g_settings_get_int(settings, "my-itu-zone");
}

gint prefs_get_my_dxcc(void) {
    if (!settings) return 0;
    return g_settings_get_int(settings, "my-dxcc");
}

gchar* prefs_get_my_country(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-country");
}

gchar* prefs_get_my_state(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-state");
}

gchar* prefs_get_my_county(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-county");
}

gchar* prefs_get_my_rig(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-rig");
}

gchar* prefs_get_my_antenna(void) {
    if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-antenna");
}

gchar* prefs_get_my_timezone(void) {
     if (!settings) return g_strdup("");
    return g_settings_get_string(settings, "my-timezone");
}

gchar* prefs_get_contest_band(void) {
    if (!settings) return g_strdup("20m"); // Default from schema
    return g_settings_get_string(settings, "contest-band");
}


// --- Setters ---

void prefs_set_dark_mode(gboolean value) {
    if (!settings) return;
    g_settings_set_boolean(settings, "dark-mode", value);
}

void prefs_set_contest_mode(gboolean value) {
     if (!settings) return;
    g_settings_set_boolean(settings, "contest-mode", value);
}

void prefs_set_my_callsign(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-callsign", value);
}

void prefs_set_my_name(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-name", value);
}

void prefs_set_my_gridsquare(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-gridsquare", value);
}

void prefs_set_my_latitude(gdouble value) {
    if (!settings) return;
    // Add range clamping if desired, although schema provides some validation
    g_settings_set_double(settings, "my-latitude", value);
}

void prefs_set_my_longitude(gdouble value) {
    if (!settings) return;
    g_settings_set_double(settings, "my-longitude", value);
}

void prefs_set_my_cq_zone(gint value) {
    if (!settings) return;
    // Add range check if needed (0-40)
    g_settings_set_int(settings, "my-cq-zone", value);
}

void prefs_set_my_itu_zone(gint value) {
    if (!settings) return;
     // Add range check if needed (0-90)
    g_settings_set_int(settings, "my-itu-zone", value);
}

void prefs_set_my_dxcc(gint value) {
    if (!settings) return;
    g_settings_set_int(settings, "my-dxcc", value);
}

void prefs_set_my_country(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-country", value);
}

void prefs_set_my_state(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-state", value);
}

void prefs_set_my_county(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-county", value);
}

void prefs_set_my_rig(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "my-rig", value);
}

void prefs_set_my_antenna(const char *value) {
     if (!settings || !value) return;
    g_settings_set_string(settings, "my-antenna", value);
}

void prefs_set_my_timezone(const char *value) {
     if (!settings || !value) return;
    g_settings_set_string(settings, "my-timezone", value);
}

void prefs_set_contest_band(const char *value) {
    if (!settings || !value) return;
    g_settings_set_string(settings, "contest-band", value);
}

// --- Signal Connection ---

gulong prefs_connect_dark_mode_changed(GCallback callback, gpointer user_data) {
    if (!settings) return 0;
    return g_signal_connect(settings, "changed::dark-mode", callback, user_data);
}

void prefs_disconnect_dark_mode_changed(gulong handler_id) {
    if (!settings || handler_id == 0) return;
    g_signal_handler_disconnect(settings, handler_id);
}

/* Final newline added below */

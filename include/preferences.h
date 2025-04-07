#ifndef GQSL_PREFERENCES_H
#define GQSL_PREFERENCES_H

#include <glib.h>
#include <gtk/gtk.h>

// Initialize the preferences system
void prefs_init(void);

// Cleanup the preferences system
void prefs_shutdown(void);

// --- Getters ---
gboolean prefs_get_dark_mode(void);
gboolean prefs_get_contest_mode(void);
gchar* prefs_get_my_callsign(void); // Free with g_free()
gchar* prefs_get_my_name(void);     // Free with g_free()
gchar* prefs_get_my_gridsquare(void); // Free with g_free()
gdouble prefs_get_my_latitude(void);
gdouble prefs_get_my_longitude(void);
gint prefs_get_my_cq_zone(void);
gint prefs_get_my_itu_zone(void);
gint prefs_get_my_dxcc(void);
gchar* prefs_get_my_country(void);  // Free with g_free()
gchar* prefs_get_my_state(void);    // Free with g_free()
gchar* prefs_get_my_county(void);   // Free with g_free()
gchar* prefs_get_my_rig(void);      // Free with g_free()
gchar* prefs_get_my_antenna(void);  // Free with g_free()
gchar* prefs_get_my_timezone(void); // Free with g_free()
gchar* prefs_get_contest_band(void);// Free with g_free()

// --- Setters ---
void prefs_set_dark_mode(gboolean value);
void prefs_set_contest_mode(gboolean value);
void prefs_set_my_callsign(const char *value);
void prefs_set_my_name(const char *value);
void prefs_set_my_gridsquare(const char *value);
void prefs_set_my_latitude(gdouble value);
void prefs_set_my_longitude(gdouble value);
void prefs_set_my_cq_zone(gint value);
void prefs_set_my_itu_zone(gint value);
void prefs_set_my_dxcc(gint value);
void prefs_set_my_country(const char *value);
void prefs_set_my_state(const char *value);
void prefs_set_my_county(const char *value);
void prefs_set_my_rig(const char *value);
void prefs_set_my_antenna(const char *value);
void prefs_set_my_timezone(const char *value);
void prefs_set_contest_band(const char *value);

// --- Signal Connection ---
gulong prefs_connect_dark_mode_changed(GCallback callback, gpointer user_data);
void prefs_disconnect_dark_mode_changed(gulong handler_id);

#endif // GQSL_PREFERENCES_H

/* Final newline added below */

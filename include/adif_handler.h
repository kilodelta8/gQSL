#ifndef GQSL_ADIF_HANDLER_H
#define GQSL_ADIF_HANDLER_H

#include <glib.h> // For gboolean

// Exports the current logbook entries to the specified ADIF file.
// Uses data from log_manager.
// Args:
//   filename: The full path to the output .adi file.
// Returns:
//   TRUE on success, FALSE on failure.
gboolean adif_export_to_file(const char *filename);

// Imports logbook entries from the specified ADIF file.
// Adds imported entries to the log_manager.
// Args:
//   filename: The full path to the input .adi file.
// Returns:
//   TRUE on success (even if some records failed), FALSE on critical failure (e.g., cannot open file).
//   Note: Partial success might occur; check console logs for warnings.
gboolean adif_import_from_file(const char *filename);


#endif // GQSL_ADIF_HANDLER_H

/* Final newline added below */

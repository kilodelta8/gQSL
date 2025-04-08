project('gQSL', 'c',
  version : '0.1.0',
  default_options : ['warning_level=3'])

# --- Dependencies ---
gtk4_dep = dependency('gtk4', version : '>=4.0')
jansson_dep = dependency('jansson')
gio_dep = dependency('gio-2.0') # Needed for GSettings

if not jansson_dep.found()
  error('Jansson library not found.')
endif

# --- Find Schema Compiler Program ---
glib_compile_schemas_prog = find_program('glib-compile-schemas', required : true)

# --- Define Schema Source File ---
gschema_source_dir = meson.current_source_dir() / 'data'
gschema_file = files('data/org.example.gqsl.gschema.xml')

# --- Custom Target for Build-time Schema Compilation ---
gschema_compiled_target = custom_target('gqsl-gschemas-compiled',
  input : gschema_file,
  output : 'gschemas.compiled',
  command : [glib_compile_schemas_prog, '--targetdir', '@OUTDIR@', gschema_source_dir],
  build_by_default : true
)

# --- Source Files ---
src_files = [
  'src/main.c',
  'src/log_manager.c',
  'src/adif_handler.c',
  'src/gui.c',
  'src/preferences.c',
  'src/log_entry_object.c', # <-- Added
  'platform/macos/platform_mac.c',
]

# --- Executable ---
executable('gqsl', src_files,
  dependencies : [ gtk4_dep, jansson_dep, gio_dep ],
  install : true
)

# --- Installation Steps ---
pkgdatadir = join_paths(get_option('prefix'), get_option('datadir'), meson.project_name())
gschema_dir = join_paths(pkgdatadir, 'glib-2.0', 'schemas')
install_data(gschema_file, install_dir: gschema_dir)
meson.add_install_script('glib-compile-schemas', gschema_dir)

# Final newline added below
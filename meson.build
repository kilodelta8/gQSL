project('gQSL', 'c',
  version : '0.1.0',
  default_options : ['warning_level=3'])

# --- Dependencies ---
gtk4_dep = dependency('gtk4', version : '>=4.0')
jansson_dep = dependency('jansson')
gio_dep = dependency('gio-2.0')

if not jansson_dep.found()
  error('Jansson library not found.')
endif

# --- Find Schema Compiler Program ---
glib_compile_schemas_prog = find_program('glib-compile-schemas', required : true)

# --- Define Schema Source File ---
# Ensure this path is correct relative to the project root (where meson.build is)
gschema_source_dir = meson.current_source_dir() / 'data'
gschema_file = files('data/org.example.gqsl.gschema.xml')

# --- Custom Target for Build-time Schema Compilation ---
# This target generates gschemas.compiled inside the build directory
gschema_compiled_target = custom_target('gqsl-gschemas-compiled',
  input : gschema_file,
  # Output file name (will be placed in the build directory's root)
  output : 'gschemas.compiled',
  # Command to run: prog --targetdir <output_dir> <source_dir>
  # @OUTDIR@ is the directory Meson creates for the output (build/)
  # gschema_source_dir is the directory containing the input xml
  command : [glib_compile_schemas_prog, '--targetdir', '@OUTDIR@', gschema_source_dir],
  build_by_default : true # Ensure this runs during `ninja build`
)
# -----------------------------------------------------


# --- Source Files ---
src_files = [
  'src/main.c',
  'src/log_manager.c',
  'src/adif_handler.c',
  'src/gui.c',
  'src/preferences.c',
  'platform/macos/platform_mac.c',
]


# --- Executable ---
executable('gqsl', src_files,
  dependencies : [
      gtk4_dep,
      jansson_dep,
      gio_dep
  ],
  # NOTE: We don't explicitly depend on gschema_compiled_target here,
  # as it's not linked. build_by_default ensures it runs.
  install : true
)


# --- Installation Steps (Still needed for system-wide install) ---
pkgdatadir = join_paths(get_option('prefix'), get_option('datadir'), meson.project_name())
gschema_dir = join_paths(pkgdatadir, 'glib-2.0', 'schemas')

# Install the SOURCE schema file to the target system directory
install_data(gschema_file, install_dir: gschema_dir)

# Run glib-compile-schemas during the actual install process in the target system directory
meson.add_install_script('glib-compile-schemas', gschema_dir)

# Final newline added below

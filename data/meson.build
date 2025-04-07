# data/meson.build

gnome = import('gnome')

# Define the source file relative to THIS directory
gschema_file = files('org.example.gqsl.gschema.xml')

# Compile the schema - runs in the context of the 'data' subdir
gschema_compiled = gnome.compile_schemas(
  depend_files: gschema_file,
  build_by_default: true
  # No target needed, Meson handles the output name (gschemas.compiled)
)

# NOTE: Installation of the schema is handled in the main meson.build
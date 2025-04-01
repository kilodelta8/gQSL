project('gQSL', 'c',
    version : '0.1.0',
    default_options : ['warning_level=3'])

gtk4_dep = dependency('gtk4', version : '>=4.0')

jansson_dep = dependency('jansson')

if not jansson_dep.found()
    error('Jansson library not found.')
endif

src_files = [
    'src/main.c',
    'src/log_manager.c',
    'src/adif_handler.c',
    'src/gui.c',
    'src/preferences.c',
    'platform/macos/platform.c',
]

executable('gqsl', src_files, dependencies : [gtk4_dep, jansson_dep], install : true) # Corrected line
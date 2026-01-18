project(
  'mullvad-exclude-suid',
  'c',
  meson_version: '>=1.1',
  version: '0.1.0',
  default_options: ['warning_level=3'],
)

if get_option('mullvad_exclude_search_strategy') == 'option'
  mullvad_exclude_path = get_option('mullvad_exclude_abs_path')
  if mullvad_exclude_path == ''
    error(
      '\'mullvad_exclude_abs_path\' must not be empty with \'option\'',
      'mullvad_exclude_search_strategy!'
    )
  elif mullvad_exclude_path[0] != '/'
    error(
      '\'mullvad_exclude_abs_path\' must be absolute path with \'option\'',
      'mullvad_exclude_search_strategy!'
    )
  endif
else
  mullvad_exclude_path = find_program('mullvad-exclude').full_path()
endif

cfg = configuration_data()
cfg.set_quoted('MULLVAD_EXCLUDE_PATH', mullvad_exclude_path)

configure_file(
  configuration: cfg,
  output: 'config.h',
)

exe = executable(
  'mullvad-exclude-suid',
  'src/mullvad-exclude-suid.c',
  install: true,
)

if get_option('root_on_install')
  if meson.version().version_compare('>=1.1.0')
    meson.add_install_script('install_scripts/root_on_install.py', dry_run: true)
  else
    meson.add_install_script('install_scripts/root_on_install.py')
  endif
endif

if get_option('set_setuid_on_install')
  if meson.version().version_compare('>=1.1.0')
    meson.add_install_script('install_scripts/setuid_root_on_install.py', dry_run: true)
  else
    meson.add_install_script('install_scripts/setuid_root_on_install.py')
  endif
endif

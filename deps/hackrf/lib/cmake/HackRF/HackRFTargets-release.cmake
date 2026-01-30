#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HackRF::hackrf" for configuration "Release"
set_property(TARGET HackRF::hackrf APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(HackRF::hackrf PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhackrf.0.9.2.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libhackrf.0.dylib"
  )

list(APPEND _cmake_import_check_targets HackRF::hackrf )
list(APPEND _cmake_import_check_files_for_HackRF::hackrf "${_IMPORT_PREFIX}/lib/libhackrf.0.9.2.dylib" )

# Import target "HackRF::hackrf_static" for configuration "Release"
set_property(TARGET HackRF::hackrf_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(HackRF::hackrf_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhackrf.a"
  )

list(APPEND _cmake_import_check_targets HackRF::hackrf_static )
list(APPEND _cmake_import_check_files_for_HackRF::hackrf_static "${_IMPORT_PREFIX}/lib/libhackrf.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HydraSDR::hydrasdr" for configuration "Release"
set_property(TARGET HydraSDR::hydrasdr APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(HydraSDR::hydrasdr PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhydrasdr.1.0.3.dylib"
  IMPORTED_SONAME_RELEASE "/usr/local/lib/libhydrasdr.0.dylib"
  )

list(APPEND _cmake_import_check_targets HydraSDR::hydrasdr )
list(APPEND _cmake_import_check_files_for_HydraSDR::hydrasdr "${_IMPORT_PREFIX}/lib/libhydrasdr.1.0.3.dylib" )

# Import target "HydraSDR::hydrasdr_static" for configuration "Release"
set_property(TARGET HydraSDR::hydrasdr_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(HydraSDR::hydrasdr_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhydrasdr.a"
  )

list(APPEND _cmake_import_check_targets HydraSDR::hydrasdr_static )
list(APPEND _cmake_import_check_files_for_HydraSDR::hydrasdr_static "${_IMPORT_PREFIX}/lib/libhydrasdr.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

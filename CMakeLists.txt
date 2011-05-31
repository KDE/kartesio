project(kartesio)
 
find_package(KDE4 REQUIRED)
find_package( Qt4 REQUIRED )
add_definitions(${KDE4_DEFINITIONS} ${QT_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES}  ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

include(KDE4Defaults)
INCLUDE( ${QT_USE_FILE} )

# detect oxygen icon dir at configure time based on KDEDIRS - there may be different package installation locations
execute_process(COMMAND "${KDE4_KDECONFIG_EXECUTABLE}" --path icon OUTPUT_VARIABLE _dir ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
file(TO_CMAKE_PATH "${_dir}" __dir)
find_path(KDE4_ICON_DIR oxygen PATHS
    ${__dir}
)
message(STATUS "using oxygen application icons from ${KDE4_ICON_DIR}")


add_subdirectory(src)
add_subdirectory(icons)
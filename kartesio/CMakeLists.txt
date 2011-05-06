project(kartesio)
 
find_package(KDE4 REQUIRED)
find_package( Qt4 REQUIRED )
add_definitions(${KDE4_DEFINITIONS} ${QT_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES}  ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

INCLUDE( ${QT_USE_FILE} )

add_subdirectory(src)
#add_subdirectory(icons)
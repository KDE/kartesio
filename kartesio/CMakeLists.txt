project(kartesio)
 
find_package(KDE4 REQUIRED)
find_package( Qt4 REQUIRED )
add_definitions(${KDE4_DEFINITIONS} ${QT_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES}  ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

INCLUDE( ${QT_USE_FILE} )
 
set(kartesio_SRCS 
  main.cpp
  mainwindow.cpp
  calculations.cpp
)

SET(kartesio_MOC_HDRS mainwindow.h)
kde4_add_ui_files(kartesio_SRCS mainwindow.ui )

QT4_WRAP_CPP ( kartesio_MOC_SRCS ${kartesio_MOC_HDRS} )
QT4_AUTOMOC ( ${kartesio_SRCS} ${kartesio_MOC_SRCS} )


SET(kartesio_EXECUTABLE
                ${kartesio_SRCS}
                ${kartesio_MOC_HDRS}
                ${kartesio_UI_HDRS}
)

kde4_add_executable(kartesio ${kartesio_EXECUTABLE})
#kde4_add_app_icon(italc_SRCS "resources/*.png")

target_link_libraries(kartesio ${KDE4_KDEUI_LIBS} ${QT_QTSCRIPT_LIBRARIES} kdeeduui)
 
install(TARGETS kartesio DESTINATION ${BIN_INSTALL_DIR})
install(FILES kartesioui.rc DESTINATION  ${DATA_INSTALL_DIR}/kartesio)
install(FILES kartesio.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR})
#kde4_install_icons(${ICON_INSTALL_DIR})
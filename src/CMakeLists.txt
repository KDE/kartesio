 
set(kartesio_SRCS 
  main.cpp
  mainwindow.cpp
  calculations.cpp
)

SET(kartesio_MOC_HDRS mainwindow.h)
kde4_add_ui_files(kartesio_SRCS mainwindow.ui )

QT4_AUTOMOC ( ${kartesio_SRCS} ${kartesio_MOC_SRCS} )
QT4_WRAP_CPP ( kartesio_MOC_SRCS ${kartesio_MOC_HDRS} )


SET(kartesio_EXECUTABLE
                ${kartesio_SRCS}
                ${kartesio_MOC_HDRS}
                ${kartesio_UI_HDRS}
)

kde4_add_app_icon(kartesio_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-kartesio.png")
kde4_add_executable(kartesio ${kartesio_EXECUTABLE})

#target_link_libraries(kartesio ${KDE4_KDEUI_LIBS} ${QT_QTSCRIPT_LIBRARIES} kdeeduui)
#target_link_libraries(kartesio ${KDE4_KDEUI_LIBS} ${QT_QTSCRIPT_LIBRARIES})
target_link_libraries(kartesio ${KDE4_KDEUI_LIBS} ${QT_QTSCRIPT_LIBRARIES} ${KDE4_KIO_LIBS} zorbaneural)
 
install(TARGETS kartesio DESTINATION ${BIN_INSTALL_DIR})
install(FILES kartesioui.rc DESTINATION  ${DATA_INSTALL_DIR}/kartesio)
install(FILES kartesio.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR})
kde4_install_icons(${ICON_INSTALL_DIR})

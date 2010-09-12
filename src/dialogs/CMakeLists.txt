SET( DIALOGS_CPPS
	aboutdialog.cpp
	settingsdialog.cpp
	listwidgetitemwithid.cpp
        playlistdialog.cpp
)

SET( DIALOGS_MOC_HDRS
	aboutdialog.h
	settingsdialog.h
        playlistdialog.h
)

SET( DIALOGS_UIS
	uis/settingsdialog.ui
        uis/playlistdialog.ui
        uis/aboutDialog.ui
)

INCLUDE_DIRECTORIES( "." ".." ${CMAKE_BINARY_DIR}/src/dialogs/ "../widgets" )
QT4_WRAP_UI( DIALOGS_UI_HDRS ${DIALOGS_UIS} )
QT4_WRAP_CPP( DIALOGS_MOC_SRCS ${DIALOGS_MOC_HDRS} )
ADD_LIBRARY( dialogs STATIC ${DIALOGS_UI_HDRS} ${DIALOGS_MOC_SRCS} ${DIALOGS_CPPS} )

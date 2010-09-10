SET( DIALOGS_CPPS
	aboutdialog.cpp
	settingsdialog.cpp
	listwidgetitemwithid.cpp
)

SET( DIALOGS_MOC_HDRS
	aboutdialog.h
	settingsdialog.h
)

SET( DIALOGS_UIS
	uis/settingsdialog.ui
        uis/aboutDialog.ui
)

INCLUDE_DIRECTORIES( "." ".." ${CMAKE_BINARY_DIR}/src/dialogs/ "../widgets" )
QT4_WRAP_UI( DIALOGS_UI_HDRS ${DIALOGS_UIS} )
QT4_WRAP_CPP( DIALOGS_MOC_SRCS ${DIALOGS_MOC_HDRS} )
ADD_LIBRARY( dialogs STATIC ${DIALOGS_UI_HDRS} ${DIALOGS_MOC_SRCS} ${DIALOGS_CPPS} )

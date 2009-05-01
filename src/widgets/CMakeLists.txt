SET( WIDGETS_CPPS
	blinkbutton.cpp
	glowbutton.cpp
	playlist.cpp
	playlistviewitem.cpp
	songlistview.cpp
	timedisplay.cpp
	timer.cpp
	vumeter.cpp
)

SET( WIDGETS_MOC_HDRS
	blinkbutton.h
	glowbutton.h
	playlistviewitem.h
	songlistview.h
	timedisplay.h
	timer.h
	vumeter.h
)

SET( WIDGETS_UIS
	timerSettings.ui
)

INCLUDE_DIRECTORIES( "." ".." ${CMAKE_BINARY_DIR}/src/widgets/ )
QT4_WRAP_UI( WIDGETS_UI_HDRS ${WIDGETS_UIS} )
QT4_WRAP_CPP( WIDGETS_MOC_SRCS ${WIDGETS_MOC_HDRS} )
ADD_LIBRARY( widgets STATIC ${WIDGETS_UI_HDRS} ${WIDGETS_MOC_SRCS} ${WIDGETS_CPPS} )

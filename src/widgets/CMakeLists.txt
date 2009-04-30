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

SET( WIDGETS_UIS
	timerSettings.ui
)

QT4_WRAP_UI( WIDGETS_UI_HDRS ${WIDGETS_UIS} )
ADD_LIBRARY( widgets STATIC ${WIDGETS_UI_HDRS} ${WIDGETS_CPPS} )

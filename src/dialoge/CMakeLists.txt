SET( DIALOGE_CPPS
	chanprefdlg.cpp
	cuelistdlg.cpp
	playlistmanager.cpp
	prefdlg.cpp
)

SET( DIALOGE_MOC_HDRS
	chanprefdlg.h
	cuelistdlg.h
	playlistmanager.h
	prefdlg.h
)

SET( DIALOGE_UIS
	kanalpreferences.ui
	ch_add_dlg.ui
	ch_del_dlg.ui
	ch_move_dlg.ui
	cuelist.ui
	help_playlist_dlg.ui
	playlistng.ui
	preferencesdlg.ui	
)

INCLUDE_DIRECTORIES( "." ".." ${CMAKE_BINARY_DIR}/src/dialoge/ "../widgets" )
QT4_WRAP_UI( DIALOGE_UI_HDRS ${DIALOGE_UIS} )
QT4_WRAP_CPP( DIALOGE_MOC_SRCS ${DIALOGE_MOC_HDRS} )
ADD_LIBRARY( dialoge STATIC ${DIALOGE_UI_HDRS} ${DIALOGE_MOC_SRCS} ${DIALOGE_CPPS} )

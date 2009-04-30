SET( DIALOGE_CPPS
	chanprefdlg.cpp
	cuelistdlg.cpp
	playlistmanager.cpp
	prefdlg.cpp
)

SET( DIALOGE_UIS
	kanalpreferences.ui
	aboutDialog.ui
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
ADD_LIBRARY( dialoge STATIC ${DIALOGE_UI_HDRS} ${DIALOGE_CPPS} )

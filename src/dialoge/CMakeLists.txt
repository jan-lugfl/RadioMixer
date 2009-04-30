SET( DIALOGE_CPPS
	chanprefdlg.cpp
	cuelistdlg.cpp
	playlistmanager.cpp
	prefdlg.cpp
)

SET( DIALOGE_UIS
	aboutDialog.ui
	ch_add_dlg.ui
	ch_del_dlg.ui
	ch_move_dlg.ui
	cuelist.ui
	help_playlist_dlg.ui
	kanalpreferences.ui
	playlistng.ui
	preferencesdlg.ui	
)

QT4_WRAP_UI( DIALOGE_UI_HDRS ${DIALOGE_UIS} )
ADD_LIBRARY( dialoge STATIC ${DIALOGE_CPPS} ${DIALOGE_UI_HDRS} )

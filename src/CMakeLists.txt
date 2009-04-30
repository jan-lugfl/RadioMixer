ADD_SUBDIRECTORY( widgets )
ADD_SUBDIRECTORY( dialoge )

SET( SRC_CPPS
	channelbufmngr.cpp
	metainfo.cpp
	mixerchannelgui.cpp
	mixerguiplayer.cpp
	playerchannelalsamix.cpp
	playerchannelstd.cpp
	radiomixer.cpp
	soundplayeralsa.cpp
	filedecoder.cpp
	metatag.cpp
	mixerguialsamix.cpp
	mpgdecoder.cpp
	playerchannelfile.cpp
	playlistitem.cpp
	soundplayerjack.cpp
	mainformdlg.cpp
	mipucom.cpp
	mixerguijackport.cpp
	oggdecoder.cpp
	playerchanneljackport.cpp
	playlistitemsongdb.cpp
	soundplayer.cpp
)

SET( SRC_UIS
	mainform.ui
)

INCLUDE_DIRECTORIES( "." "widgets" "dialoge" ${CMAKE_BINARY_DIR}/src/widgets ${CMAKE_BINARY_DIR}/src/dialoge ${CMAKE_BINARY_DIR}/src )
QT4_WRAP_UI( SRC_UI_HDRS ${SRC_UIS} )
ADD_EXECUTABLE(radiomixer ${SRC_CPPS} ${SRC_UI_HDRS} ${RADIOMIXER_RC_SRCS} )
ADD_DEPENDENCIES( radiomixer dialoge widgets )

ADD_SUBDIRECTORY( widgets dialoge )

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
	ringbuffer.cpp
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

QT4_WRAP_UI( SRC_UI_HDRS ${SRC_UIS} )
ADD_EXECUTABLE(radiomixer ${SRC_CPPS} ${RADIOMIXER_RC_SRCS} )

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
	mainformdlg.cpp
	soundplayerjack.cpp
	mipucom.cpp
	mixerguijackport.cpp
	oggdecoder.cpp
	playerchanneljackport.cpp
	playlistitemsongdb.cpp
	soundplayer.cpp
)

SET( SRC_MOC_HDRS
	metainfo.h
	mixerchannelgui.h
	mixerguiplayer.h
	playerchannelalsamix.h
	playerchannelstd.h
	playerchannelfile.h
	playlistitem.h
	filedecoder.h
	mixerguialsamix.h
	mpgdecoder.h
	soundplayerjack.h
	mipucom.h
	mixerguijackport.h
	oggdecoder.h
	playerchanneljackport.h
	playlistitemsongdb.h
)

SET( SRC_UIS
	mainform.ui
)

INCLUDE_DIRECTORIES( "." "widgets" "dialoge" ${CMAKE_BINARY_DIR}/src/widgets ${CMAKE_BINARY_DIR}/src/dialoge ${CMAKE_BINARY_DIR}/src )
QT4_WRAP_CPP( SRC_MOC_SRCS ${SRC_MOC_HDRS} )
QT4_WRAP_UI( SRC_UI_HDRS ${SRC_UIS} )
ADD_EXECUTABLE(radiomixer ${SRC_CPPS} ${SRC_UI_HDRS} ${SRC_MOC_SRCS} ${RADIOMIXER_RC_SRCS} )

TARGET_LINK_LIBRARIES(radiomixer ${QT_LIBRARIES} ${ALSA_LIBRARIES} ${JACKD_LIBRARIES} ${MAD_LIBRARIES} ${OGG_LIBRARIES} ${SAMPLERATE_LIBRARIES} dialoge widgets )
ADD_DEPENDENCIES( radiomixer dialoge widgets )

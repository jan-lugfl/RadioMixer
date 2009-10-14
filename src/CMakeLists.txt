ADD_SUBDIRECTORY( widgets )
ADD_SUBDIRECTORY( dialoge )

SET( SRC_CPPS
	mixerchannelmanager.cpp
	channelbufmngr.cpp
	metainfo.cpp
	mixergui.cpp
	mixerguiplayer.cpp
	mixerchannel.cpp
	radiomixer.cpp
	soundplayeralsa.cpp
	filedecoder.cpp
	metatag.cpp
	mixerguialsamix.cpp
	mpgdecoder.cpp
	mixerchannel_fileplayer.cpp
	mixerchannel_alsa.cpp
	playlistitem.cpp
	soundplayerjack.cpp
	mipucom.cpp
	mixerguijackport.cpp
	oggdecoder.cpp
	playerchanneljackport.cpp
	playlistitemsongdb.cpp
	soundplayer.cpp
	mainWindow.cpp
)

SET( SRC_MOC_HDRS
	mixerchannelmanager.h
	metainfo.h
	mixergui.h
	mixerguiplayer.h
	mixerchannel.h
	mixerchannel_fileplayer.h
	mixerchannel_alsa.h
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
	mainWindow.h
)

SET( SRC_UIS
	uis/radiomixer.ui
)

SET( RADIOMIXER_RCS
	../radiomixer.qrc
)

INCLUDE_DIRECTORIES( "." "widgets" "dialoge" ${CMAKE_BINARY_DIR}/src/widgets ${CMAKE_BINARY_DIR}/src/dialoge ${CMAKE_BINARY_DIR}/src )
QT4_WRAP_CPP( SRC_MOC_SRCS ${SRC_MOC_HDRS} )
QT4_WRAP_UI( SRC_UI_HDRS ${SRC_UIS} )
QT4_ADD_RESOURCES( RADIOMIXER_RC_SRCS ${RADIOMIXER_RCS} )
ADD_EXECUTABLE(radiomixer ${SRC_CPPS} ${SRC_UI_HDRS} ${SRC_MOC_SRCS} ${RADIOMIXER_RC_SRCS} )

TARGET_LINK_LIBRARIES(radiomixer ${QT_LIBRARIES} ${ALSA_LIBRARIES} ${JACKD_LIBRARIES} ${MAD_LIBRARIES} ${OGG_LIBRARIES} ${SAMPLERATE_LIBRARIES} dialoge widgets )
ADD_DEPENDENCIES( radiomixer dialoge widgets )

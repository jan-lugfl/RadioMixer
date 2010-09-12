ADD_SUBDIRECTORY( widgets )
ADD_SUBDIRECTORY( dialogs )

SET( SRC_CPPS
	jack.cpp
	jack_midicontrol.cpp
	mixerchannelmanager.cpp
	channelbufmngr.cpp
	channelmixer.cpp
	metainfo.cpp
	mixergui.cpp
	mixerguiplayer.cpp
	radiomixer.cpp
	soundplayeralsa.cpp
	filedecoder.cpp
	metatag.cpp
	mixerguialsamix.cpp
	mpgdecoder.cpp
	mixerchannel.cpp
	mixerchannel_alsa.cpp
	mixerchannel_fileplayer.cpp
	mixerchannel_jackin.cpp
	mixerchannel_jackout.cpp
	remotecontrolchannel.cpp
	remotecontrol.cpp
	remotecontrol_midi.cpp
	playlistitem.cpp
	mipucom.cpp
	oggdecoder.cpp
	mixerguijackport.cpp
	playlistitemsongdb.cpp
	soundplayer.cpp
	mainWindow.cpp
	settings.cpp
	playlist.cpp
	playlistmanager.cpp
)

SET( SRC_MOC_HDRS
	channelmixer.h
	mixerchannelmanager.h
	metainfo.h
	mixergui.h
	mixerguiplayer.h
	mixerchannel.h
	mixerchannel_alsa.h
	mixerchannel_fileplayer.h
	mixerchannel_jackin.h
	mixerchannel_jackout.h
	playlistitem.h
	remotecontrolchannel.h
	remotecontrol.h
	remotecontrol_midi.h
	filedecoder.h
	mixerguialsamix.h
	mpgdecoder.h
	mipucom.h
	mixerguijackport.h
	oggdecoder.h
	playlistitemsongdb.h
	mainWindow.h
	playlist.h
	playlistmanager.h
)

SET( SRC_UIS
	uis/radiomixer.ui
)

SET( RADIOMIXER_RCS
	../radiomixer.qrc
)

INCLUDE_DIRECTORIES( "." "widgets" "dialogs" ${CMAKE_BINARY_DIR}/src/widgets ${CMAKE_BINARY_DIR}/src/dialogs ${CMAKE_BINARY_DIR}/src )
QT4_WRAP_CPP( SRC_MOC_SRCS ${SRC_MOC_HDRS} )
QT4_WRAP_UI( SRC_UI_HDRS ${SRC_UIS} )
QT4_ADD_RESOURCES( RADIOMIXER_RC_SRCS ${RADIOMIXER_RCS} )
ADD_EXECUTABLE(radiomixer ${SRC_CPPS} ${SRC_UI_HDRS} ${SRC_MOC_SRCS} ${RADIOMIXER_RC_SRCS} )

TARGET_LINK_LIBRARIES(radiomixer ${QT_LIBRARIES} ${ALSA_LIBRARIES} ${JACKD_LIBRARIES} ${MAD_LIBRARIES} ${OGG_LIBRARIES} ${SAMPLERATE_LIBRARIES} dialogs widgets )
ADD_DEPENDENCIES( radiomixer dialogs widgets )

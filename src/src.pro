# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/radiomixer

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3 += mainform.ui
HEADERS += mainformdlg.h \
           soundplayer.h \
           playerchannelstd.h \
           playerchannelfile.h \
           mixerchannelgui.h \
           channelbufmngr.h \
           mixerguiplayer.h \
           metainfo.h \
           metatag.h \
           ringbuffer.h \
           filedecoder.h \
           version.h \
           playlistitem.h
SOURCES += radiomixer.cpp \
           mainformdlg.cpp \
           soundplayer.cpp \
           playerchannelstd.cpp \
           playerchannelfile.cpp \
           mixerchannelgui.cpp \
           channelbufmngr.cpp \
           mixerguiplayer.cpp \
           metainfo.cpp \
           metatag.cpp \
           filedecoder.cpp \
           playlistitem.cpp
TARGET = ../bin/radiomixer
CONFIG += warn_off \
qt \
thread
TEMPLATE = app
RESOURCES += ../radiomixer.qrc
include(../conf.pri)
vorbisSupport {
    HEADERS += oggdecoder.h
    SOURCES += oggdecoder.cpp
}
madSupport {
    HEADERS += mpgdecoder.h
    SOURCES += mpgdecoder.cpp
}
alsaSupport {
    HEADERS += playerchannelalsamix.h \
  mixerguialsamix.h \
  soundplayeralsa.h
    SOURCES += playerchannelalsamix.cpp \
  mixerguialsamix.cpp \
  soundplayeralsa.cpp
}
jackSupport {
    HEADERS += soundplayerjack.h \
  playerchanneljackport.h \
  mixerguijackport.h
    SOURCES += soundplayerjack.cpp \
  playerchanneljackport.cpp \
  mixerguijackport.cpp
}
hwmixer {
    HEADERS += mipucom.h
    SOURCES += mipucom.cpp
}
songdb {
    HEADERS += playlistitemsongdb.h
    SOURCES += playlistitemsongdb.cpp
}
#The following line was inserted by qt3to4
QT += xml  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3

CONFIG -= warn_off
DESTDIR = .

INCLUDEPATH += ../src \
../src/dialoge \
../src/widgets
LIBS += ../src/dialoge/libdialoge.a \
../src/widgets/libwidgets.a
TARGETDEPS += ../src/widgets/libwidgets.a \
../src/dialoge/libdialoge.a

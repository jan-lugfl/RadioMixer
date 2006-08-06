# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/radiomixer

FORMS += mainform.ui 
HEADERS += mainformdlg.h \
           soundplayer.h \
           playerchannelstd.h \
           playerchannelfile.h \
           mixerchannelgui.h \
           channelbufmngr.h \
           playlst.h \
           mixerguiplayer.h \
           metainfo.h \
           title.h \
           metatag.h \
           ringbuffer.h \
           filedecoder.h \
           version.h 
SOURCES += radiomixer.cpp \
           mainformdlg.cpp \
           soundplayer.cpp \
           playerchannelstd.cpp \
           playerchannelfile.cpp \
           mixerchannelgui.cpp \
           channelbufmngr.cpp \
           playlst.cpp \
           mixerguiplayer.cpp \
           metainfo.cpp \
           title.cpp \
           metatag.cpp \
           filedecoder.cpp 
TARGETDEPS += ../src/widgets/libwidgets.a \
../src/dialoge/libdialoge.a
LIBS += ../src/dialoge/libdialoge.a \
../src/widgets/libwidgets.a
INCLUDEPATH += ../src \
../src/dialoge \
../src/widgets
TARGET = ../bin/radiomixer
CONFIG += release \
warn_off \
qt \
thread
TEMPLATE = app
include (../conf.pri)
vorbisSupport{
  HEADERS += oggdecoder.h
  SOURCES += oggdecoder.cpp
}
madSupport{
  HEADERS += mpgdecoder.h
  SOURCES += mpgdecoder.cpp
}
alsaSupport{
  HEADERS += playerchannelalsamix.h \
  mixerguialsamix.h \
  soundplayeralsa.h
  SOURCES += playerchannelalsamix.cpp \
  mixerguialsamix.cpp \
  soundplayeralsa.cpp
}
jackSupport{
  HEADERS += soundplayerjack.h \
  playerchanneljackport.h \
  mixerguijackport.h
  SOURCES += soundplayerjack.cpp \
  playerchanneljackport.cpp \
  mixerguijackport.cpp
}
hwmixer{
  HEADERS += mipucom.h
  SOURCES += mipucom.cpp
}

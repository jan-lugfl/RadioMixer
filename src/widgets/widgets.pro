# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src/widgets
# Das Target ist eine Bibliothek:  

HEADERS += glowbutton.h \
           blinkbutton.h \
           timedisplay.h \
           vumeter.h \
           playlistitem.h \
           playlist.h 
SOURCES += glowbutton.cpp \
           blinkbutton.cpp \
           timedisplay.cpp \
           vumeter.cpp \
           playlistitem.cpp \
           playlist.cpp 
INCLUDEPATH += ../../src 
CONFIG += release \
          warn_on \
          qt \
          staticlib 
TEMPLATE = lib 
include (../../conf.pri)
songdb{
  HEADERS += playlistitemsongdb.h 
  SOURCES += playlistitemsongdb.cpp 
}

# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src/dialoge
# Das Target ist eine Bibliothek:  

FORMS += preferencesdlg.ui \
         playlist.ui \
         help_playlist_dlg.ui \
         kanalpreferences.ui \
         ch_add_dlg.ui \
         ch_del_dlg.ui \
         ch_move_dlg.ui \
         cuelist.ui \
         aboutDialog.ui \
         playlistng.ui 
HEADERS += prefdlg.h \
           playlistdlg.h \
           chanprefdlg.h \
           cuelistdlg.h \
           playlistmanager.h 
SOURCES += playlistdlg.cpp \
           prefdlg.cpp \
           chanprefdlg.cpp \
           cuelistdlg.cpp \
           playlistmanager.cpp 
INCLUDEPATH += ../../src \
../../src/widgets \
..
CONFIG += release \
warn_on \
qt \
staticlib
TEMPLATE = lib
include (../../conf.pri)
songdb{
  FORMS += songDBDialog.ui
  HEADERS += songdbdlg.h
  SOURCES += songdbdlg.cpp
}

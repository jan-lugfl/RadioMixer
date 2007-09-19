# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src/dialoge
# Das Target ist eine Bibliothek:  

FORMS += preferencesdlg.ui \
         help_playlist_dlg.ui \
         kanalpreferences.ui \
         ch_add_dlg.ui \
         ch_del_dlg.ui \
         ch_move_dlg.ui \
         cuelist.ui \
         aboutDialog.ui \
         playlistng.ui
HEADERS += prefdlg.h \
           chanprefdlg.h \
           cuelistdlg.h \
           playlistmanager.h
SOURCES += prefdlg.cpp \
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
include(../../conf.pri)

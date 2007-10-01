# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src/dialoge
# Das Target ist eine Bibliothek:  

FORMS += aboutDialog.ui \
         ch_add_dlg.ui \
         ch_del_dlg.ui \
         ch_move_dlg.ui \
         cuelist.ui \
	 preferencesdlg.ui \
         kanalpreferences.ui
HEADERS += prefdlg.h \
           chanprefdlg.h \
           cuelistdlg.h \
           playlistmanager.h
SOURCES += prefdlg.cpp \
           chanprefdlg.cpp \
           cuelistdlg.cpp \
           playlistmanager.cpp
CONFIG += warn_on \
qt \
staticlib
TEMPLATE = lib
include(../../conf.pri)

#The following line was inserted by qt3to4
QT += xml  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3
#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3 +=  playlistng.ui

DESTDIR = .

INCLUDEPATH += .. \
../../src \
../../src/widgets

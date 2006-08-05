# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src/widgets
# Das Target ist eine Bibliothek:  

HEADERS += glowbutton.h \
           blinkbutton.h \
           timedisplay.h \
           vumeter.h \
           playlistviewitem.h 
SOURCES += glowbutton.cpp \
           blinkbutton.cpp \
           timedisplay.cpp \
           vumeter.cpp \
           playlistviewitem.cpp 
CONFIG += release \
warn_on \
qt \
staticlib
TEMPLATE = lib

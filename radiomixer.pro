# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: .
# Das Target ist Projekt im Unterordner 

TARGETDEPS += src/widgets/widgets \
              src/dialoge/dialoge \
              src/../bin/radiomixer 
CONFIG += release \
          warn_off \
          qt \
          thread \
          x11 \
          ordered 
TEMPLATE = subdirs 
SUBDIRS += src/widgets \
           src/dialoge \
           src 
TRANSLATIONS = radiomixer_de.ts

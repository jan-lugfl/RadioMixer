/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2009 by Jan Boysen                                *
 *   trekkie@media-mission.de                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef HAVE_ALSA
#ifndef HAVE_JACK
#error wheter JackD nor ALSA support is being compiled, so this application seems completely useless...
#error please install JackIt or alsa-lib to use this application....
#endif //HAVE_JACK
#endif //HAVE_ALSA

#include <qapplication.h>
#include <qmainwindow.h>
#include "mainWindow.h"

int main( int argc, char ** argv ) {
       QApplication a( argc, argv );
       mainWindow main;

       a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
       a.connect( &main, SIGNAL(showAboutQt()), &a, SLOT(aboutQt()) );

       main.show();
       return a.exec();
}

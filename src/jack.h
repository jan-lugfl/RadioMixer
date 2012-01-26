/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2009 by Jan Boysen                                      *
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

// include Jacks headers
#include <jack/jack.h>
#include <jack/midiport.h>

// include needed QT stuff
#include <QString>
#include <QMessageBox>
#include <QVector>

#include "jack_midicontrol.h"

#ifndef JACK_H
#define JACK_H

class Jack
{
public:
    static void errorHandler( const char* msg );
    static void connect( QString client_name = QString("RadioMixer") );
    static jack_port_t* newPort(QString name, unsigned long flags);
    static void unregisterPort( jack_port_t* port );
    static jack_MIDIControl* newController( QString name = QString("Remote Control"), bool bidirectional = true );
    static int process( jack_nframes_t frames, void * arg );
    static void jackShutdown( void * arg );
    static unsigned int getSmplRate();

private:
    static bool connected;
    static int bufSize;
    static unsigned int smplRate;
    static jack_client_t* jack;
    static QVector<jack_MIDIControl*> midi_controllers;
};

#endif // JACK_H

/* $Id$ */
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

#include "jack.h"
#include "mixerchannelmanager.h"
#include "mixerchannel_jackin.h"
#include "mixerchannel_jackout.h"

// static values,,,
bool Jack::connected = false;
int Jack::bufSize = 0;
unsigned int Jack::smplRate = 0;
jack_client_t* Jack::jack = NULL;
QVector<jack_MIDIControl*> Jack::midi_controllers;

void Jack::errorHandler( const char* msg )
{
    qWarning(msg);
}

int Jack::process( jack_nframes_t frames, void * arg )
{
    // iterate through all cannels and let the Jack cannels process their data...

    // Inputs first ;)
    mixerChannelManager::storageType::iterator it;
    for( it = mixerChannelManager::inChannels.begin(); it != mixerChannelManager::inChannels.end(); it++ )
	if( (*it)->getType() == "JACKIN" )
	    dynamic_cast<mixerChannel_jackIn*>(*it)->process( frames );

    // now do the same for the outputs...
    for( it = mixerChannelManager::outChannels.begin(); it != mixerChannelManager::outChannels.end(); it++ )
	if( (*it)->getType() == "JACKOUT" )
	    dynamic_cast<mixerChannel_jackOut*>(*it)->process( frames );

    // process MIDI data..
    QVector<jack_MIDIControl*>::iterator midi_it;
    for( midi_it = midi_controllers.begin(); midi_it != midi_controllers.end(); midi_it++ )
        (*midi_it)->process( frames );

    return 0;
}

unsigned int Jack::getSmplRate()
{
    return smplRate;
}

void Jack::jackShutdown( void * arg )
{
    qWarning("got shotdown signal from jackd in connected state");
    Jack::connected = FALSE;
}

void Jack::connect( QString client_name )
{
	if(connected)
	{
		QMessageBox::warning( NULL, QObject::tr("RadioMixer - JackD"), QObject::tr("already connected to JackD Server with name %1...").arg(client_name), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}else
	{
		if ((jack = jack_client_new(client_name)) == 0)
			QMessageBox::warning( NULL, QObject::tr("RadioMixer - JackD"), QObject::tr("could not connect to JackD Server\nmaybe it is not running ? "), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		else
		{
			connected = true;
			// register new Jack Ports
			jack_set_process_callback(jack, process, NULL );
			jack_on_shutdown(jack, jackShutdown, NULL);
			jack_set_error_function( errorHandler );
			jack_activate (jack);

			bufSize = jack_get_buffer_size( jack );
			smplRate = jack_get_sample_rate( jack );
		}
	}
}

// this is just a wrapper for registering a new port within the actual client connection we are using...
jack_port_t* Jack::newPort( QString name, unsigned long flags )
{
    if( Jack::connected )
        return  jack_port_register( jack, name, JACK_DEFAULT_AUDIO_TYPE, flags ,0 );
    else
	return NULL;
}

// This registers a new controller port
jack_MIDIControl* Jack::newController( QString name, bool bidirectional )
{
    jack_MIDIControl* controller = NULL;
    if( Jack::connected )
    {
        controller = new jack_MIDIControl();
        controller->input = jack_port_register( jack, name+"_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput ,0 );
        if( bidirectional )
        {
            controller->is_bidirectional = true;
            controller->output = jack_port_register( jack, name+"_out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput ,0 );
        }
        midi_controllers.append( controller );
    }
    return controller;
}

// unregisteres a port when its not used anymore...
void Jack::unregisterPort( jack_port_t* port )
{
    jack_port_unregister( jack, port );
    port = NULL;
}

/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2007 by Jan Boysen                                *
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
#include "soundplayerjack.h"

soundPlayerJack::soundPlayerJack( )
: soundPlayer( ), buffResetted( FALSE)
{
    jack_set_error_function( jackError );
}

soundPlayerJack::~soundPlayerJack()
{
	close();
}

void soundPlayerJack::open( QString device )
{
	if(devOpened)
	{
		QMessageBox::warning( NULL, tr("RadioMixer - JackD"), tr("already connected to JackD Server %1...").arg(device), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}else
	{
		if ((jack = jack_client_new ("RadioMixer")) == 0)
			QMessageBox::warning( NULL, tr("RadioMixer - JackD"), tr("could not connect to JackD Server\nmaybe it is not running ? "), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		else
		{
			devOpened = TRUE;
			// register new Jack Ports
			
			outputPorts[0] = jack_port_register( jack, "Master_Left",  JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput ,0 );
			outputPorts[1] = jack_port_register( jack, "Master_Right",  JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput ,0 );
                        jack_set_process_callback(jack, process, (void *)this);
                        jack_on_shutdown(jack, jackShutdown, (void*)this);
			jack_activate (jack);

			bufSize = jack_get_buffer_size( jack );

			outRate = jack_get_sample_rate( jack );
			emit onConnect();
		}
	}
}

void soundPlayerJack::close( )
{
	if( devOpened )
	{
		emit onDisconnect();
		devOpened = FALSE;
		jack_client_close ( jack );
	}
}

int soundPlayerJack::process( jack_nframes_t frames, void * arg )
{
        soundPlayerJack* self = (soundPlayerJack*)arg;
	bool playing = FALSE;

        Q3ValueVector<jackPort*>::iterator portIt;
	for( portIt = self->jackPorts.begin(); portIt != self->jackPorts.end(); ++portIt )
		(*portIt)->process( frames );

        mixerChannelManager::storageType::iterator it;
        for( it = mixerChannelManager::inChannels.begin(); it != mixerChannelManager::inChannels.end(); it++ )
	{
//		if( (*it)->isPlaying() )
			playing = TRUE;
	}
	if( playing )
	{
		self->buffResetted = FALSE;
		//Yeahhrr... move Data to the Buffer of Jack
		self->mixChannels();

		jack_default_audio_sample_t*	mixedLeft = ( jack_default_audio_sample_t* ) jack_port_get_buffer (self->outputPorts[0], self->interMixSamples);
		jack_default_audio_sample_t*	mixedRight = ( jack_default_audio_sample_t* ) jack_port_get_buffer (self->outputPorts[1], self->interMixSamples);

		if( self->outputBuffers[0].canRead( self->bufSize ) && self->outputBuffers[1].canRead( self->bufSize ) )
		{
			self->outputBuffers[0].read( mixedLeft, self->bufSize );
			self->outputBuffers[1].read( mixedRight, self->bufSize );
		}

	}
	else if( !self->buffResetted )
	{
		jack_default_audio_sample_t*	mixedL = ( jack_default_audio_sample_t* ) jack_port_get_buffer (self->outputPorts[0], frames);
		jack_default_audio_sample_t*	mixedR = ( jack_default_audio_sample_t* ) jack_port_get_buffer (self->outputPorts[1], frames);
		unsigned int sample_off = 0;
		while( sample_off < frames )
		{
			mixedL[sample_off] = 0.f;
			mixedR[sample_off] = 0.f;
			sample_off++;
		}
		self->buffResetted = TRUE;
	}
        return 0;
}

jackPort* soundPlayerJack::registerJackPort( QString name, unsigned long flags )
{
	jackPort* jPort = new jackPort( jack, jack_port_register( jack, name,  JACK_DEFAULT_AUDIO_TYPE, flags ,0 ) );
	jackPorts.append( jPort );
	return jPort;
}

jackPort::jackPort( jack_client_t* jackClient, jack_port_t* jPort )
{
	jackport = jPort;
	jack = jackClient;
}

jackPort::~ jackPort( )
{
//	if( jack_port_unregister( jack, jackport) == 0)
//		qDebug("successfully unregistered Jack Port..");
}

void jackPort::process( jack_nframes_t frames )
{
	emit processData( frames );
}

void soundPlayerJack::jackShutdown( void * arg )
{
	soundPlayerJack* self = (soundPlayerJack*)arg;
	qWarning("got shotdown signal from jackd in connected state");
	self->devOpened = FALSE;
//	emit self->onDisconnect();
}

void soundPlayerJack::jackError( const char* msg )
{
    qWarning(msg);
}

void soundPlayerJack::unregisterJackPort( jackPort * port)
{
	Q3ValueVector<jackPort*>::iterator portIt;
	for( portIt = jackPorts.begin(); portIt != jackPorts.end(); ++portIt )
		if( (*portIt) == port )
		{
			delete (*portIt);
			jackPorts.erase( portIt );
			break;
		}
}

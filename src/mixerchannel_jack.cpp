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
#include "mixerchannel_jack.h"

mixerChannel_jack::mixerChannel_jack( soundPlayerJack* jackPlayer, QString chName, const char *name )
 : mixerChannel( name ), levelMeterLeft(0), levelMeterRight(0)
{
	this->name = chName;
	this->player = jackPlayer;
	connect( jackPlayer, SIGNAL( onConnect() ), this, SLOT( connectPort()) );
	connect( jackPlayer, SIGNAL( onDisconnect() ), this, SLOT( disconnectPort()) );
	if( player->isConnected() )
		connectPort();
}


mixerChannel_jack::~mixerChannel_jack( )
{
	if( player->isConnected() )
		disconnectPort();
}

void mixerChannel_jack::processJackLeft( jack_nframes_t frames  )
{
	this->frames = frames;
	jack_default_audio_sample_t*	source = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jackIn[0]->jackport, frames);
	jack_default_audio_sample_t*	mixed = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jackOut[0]->jackport, frames);
	unsigned int sample_off = 0;
	while( sample_off < frames )
	{
		mixed[sample_off] = source[sample_off]*volume*volume_left;
		if(mixed[sample_off] > 0.f)
			levelMeterLeft += mixed[sample_off];
		sample_off++;
	}
	levelMeterLeft /= frames;
}

void mixerChannel_jack::processJackRight( jack_nframes_t frames  )
{
	jack_default_audio_sample_t*	source = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jackIn[1]->jackport, frames);
	jack_default_audio_sample_t*	mixed = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jackOut[1]->jackport, frames);
	unsigned int sample_off = 0;
	while( sample_off < frames )
	{
		mixed[sample_off] = source[sample_off]*volume*volume_right;
		if(mixed[sample_off] > 0.f)
			levelMeterRight += mixed[sample_off];
		sample_off++;
	}
	levelMeterRight /= frames;
}

void mixerChannel_jack::setVolume( int volume )
{
	this->volume = float(100.f-volume)/100.f;
}

void mixerChannel_jack::mute( )
{
	disconnect( jackIn[0], SIGNAL( processData( jack_nframes_t ) ), this, SLOT( processJackLeft( jack_nframes_t ) ) );
	disconnect( jackIn[1], SIGNAL( processData( jack_nframes_t ) ), this, SLOT( processJackRight( jack_nframes_t ) ) );

	jack_default_audio_sample_t*	mixedL = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jackOut[0]->jackport, frames);
	jack_default_audio_sample_t*	mixedR = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jackOut[1]->jackport, frames);
	unsigned int sample_off = 0;
	while( sample_off < frames )
	{
		mixedL[sample_off] = 0.f;
		mixedR[sample_off] = 0.f;
		sample_off++;
	}
}

void mixerChannel_jack::unMute( )
{
	connect( jackIn[0], SIGNAL( processData( jack_nframes_t ) ), this, SLOT( processJackLeft( jack_nframes_t ) ) );
	connect( jackIn[1], SIGNAL( processData( jack_nframes_t ) ), this, SLOT( processJackRight( jack_nframes_t ) ) );
}

void mixerChannel_jack::connectPort( )
{
	jackIn[0] = player->registerJackPort( getName()+QString("_leftIn"), JackPortIsInput);
	jackIn[1] = player->registerJackPort( getName()+QString("_rightIn"), JackPortIsInput);
	jackOut[0] = player->registerJackPort( getName()+QString("_leftOut"), JackPortIsOutput);
	jackOut[1] = player->registerJackPort( getName()+QString("_rightOut"), JackPortIsOutput);
	unMute();
}

void mixerChannel_jack::disconnectPort( )
{
	mute();
	player->unregisterJackPort( jackIn[0] );
	player->unregisterJackPort( jackOut[0] );
	player->unregisterJackPort( jackIn[1] );
	player->unregisterJackPort( jackOut[1] );
}

const float mixerChannel_jack::getLevelMeterLeft( )
{
	return levelMeterLeft;
}

const float mixerChannel_jack::getLevelMeterRight( )
{
	return levelMeterRight;
}

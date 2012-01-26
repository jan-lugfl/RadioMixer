/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2009-2010 by Jan Boysen                                 *
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
#include "mixerchannel_jackout.h"
#include "mixerchannelmanager.h"

QString const mixerChannel_jackOut::Type = QString("JACKOUT");

mixerChannel_jackOut::mixerChannel_jackOut( QString name, QUuid uuid )
 : mixerChannel( name, uuid ), levelMeterLeft(0), levelMeterRight(0)
{
    type = Type;

    // shrink the buffer size for lower latency...
    soundBuffers[0].setBufSize(2050);
    soundBuffers[1].setBufSize(2050);

    connectPort();
    registerChannel();
}


mixerChannel_jackOut::~mixerChannel_jackOut( )
{
    // unregister myself in the channel manager
    mixerChannelManager::unregisterChannel( this );
    disconnectPort();
}

void mixerChannel_jackOut::process( jack_nframes_t frames  )
{
    if(!jack_port[0]||!jack_port[1])
        return;

    jack_default_audio_sample_t*	destL = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jack_port[0], frames);
    jack_default_audio_sample_t*	destR = ( jack_default_audio_sample_t* ) jack_port_get_buffer (jack_port[1], frames);

    if( canGetData(frames) && !muted )
    {
        getDataLeft( destL, frames );
        getDataRight( destR, frames );
    }
    else
    {
        memset( destL, 0, frames*sizeof(destL));
        memset( destR, 0, frames*sizeof(destR));
    }
}

void mixerChannel_jackOut::mute( )
{
    muted = true;
    emit( muteChanged(muted) );
}

void mixerChannel_jackOut::unMute( )
{
    muted = false;
    emit( muteChanged(muted) );
}

void mixerChannel_jackOut::toggleMute()
{
    muted = !muted;
    emit( muteChanged(muted) );
}

void mixerChannel_jackOut::connectPort( )
{
    jack_port[0] = Jack::newPort( getName().left(16)+QString("_L"), JackPortIsOutput);
    jack_port[1] = Jack::newPort( getName().left(16)+QString("_R"), JackPortIsOutput);
    unMute();
}

void mixerChannel_jackOut::disconnectPort( )
{
    if(jack_port[0])
        Jack::unregisterPort( jack_port[0] );
    if(jack_port[1])
        Jack::unregisterPort( jack_port[1] );
}

void mixerChannel_jackOut::setName( QString newName )
{
    if(!jack_port[0]||!jack_port[1])
        return;
    mixerChannel::setName( newName );
    jack_port_set_name(jack_port[0], QString( settings["name"].toString()+QString("_L")).toAscii() );
    jack_port_set_name(jack_port[1], QString( settings["name"].toString()+QString("_R")).toAscii() );
}

void mixerChannel_jackOut::updateSettings( settingsType settings )
{
    if(jack_port[0] && jack_port[1])
    {
	jack_port_set_name(jack_port[0], QString( settings["name"].toString()+QString("_L")).toAscii() );
	jack_port_set_name(jack_port[1], QString( settings["name"].toString()+QString("_R")).toAscii() );
    }
    mixerChannel::updateSettings( settings );
}

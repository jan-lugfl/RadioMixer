/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
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
#include "settings.h"
#include "mixerchannel.h"
#include "mixerchannelmanager.h"

#include <QPalette>

QString const mixerChannel::Type = QString("unknown");

mixerChannel::mixerChannel(  QString name, QUuid uuid )
 : QObject(0), volume(1), sendVuMeterChanged_left(0), sendVuMeterChanged_right(0)
{
    // register settings type in QT
    qRegisterMetaType<mixerChannel::settingsType>("mixerChannel::settingsType");

    if(uuid.isNull())
        this->uuid = QUuid::createUuid(); // generate UUID..
    else
        this->uuid = uuid;

    if(name == QString())
            name = this->uuid.toString();

    settings["name"] = name;
    // get systems default color...
    QPalette pal;
    settings["color"] = pal.background().color();

    // initiate my thread and move my eventloop into it...
    thread = new QThread();
    //this->moveToThread( thread );
    thread->start();

    // allocate Stereo Sound Ringbuffer.,...
    soundBuffers = new soundRingBuffer[2];
    soundBuffers[0].setName(name+QString("_left"));
    soundBuffers[1].setName(name+QString("_right"));
    soundBuffers[0].setBufSize(16384);
    soundBuffers[1].setBufSize(16384);

    volume_left = 1;
    volume_right = 1;
}


mixerChannel::~mixerChannel()
{
    // unregister myself in the channel manager
    //mixerChannelManager::unregisterChannel( this );
}

void mixerChannel::registerChannel()
{
    // register myself in the channel manager
    mixerChannelManager::registerChannel( this );
}

void mixerChannel::setVolume( int newValue )
{
        float newVolume = (float)newValue/100.f;
        if( newVolume != volume )
        {
            volume = newVolume;
            emit( volumeChanged(newValue) );
        }
}

QString mixerChannel::getName( )
{
    return settings["name"].toString();
}

void mixerChannel::setName( QString newName )
{
    settings["name"] = newName;
    emit( settingsChanged( settings ) );
}

QUuid mixerChannel::getUuid()
{
    return uuid;
}

mixerChannel::settingsType mixerChannel::getSettings()
{
    return settings;
}

float mixerChannel::getLevelLeft( )
{
        return volume*volume_left;
}

float mixerChannel::getLevelRight( )
{
        return volume*volume_right;
}

/**
 * signal Wrapper only that emits the refresh signal to outside connected stuff.
 */
void mixerChannel::refresh( )
{
	emit( refreshed() );
}

const unsigned int mixerChannel::getSmplRate( )
{
        return smplRate;
}

void mixerChannel::setTreble( int level )
{
    if(level > 0)
    {
        volume_left = (100-level)/100.f;
        volume_right = 1.f;
    }
    else if(level < 0)
    {
        volume_left = 1.f;
        volume_right = (100+level)/100.f;
    }
    else
    {
        volume_left = 1.f;
        volume_right = 1.f;
    }
    emit(trebleChanged(level));
}

bool mixerChannel::canGetData( unsigned int size )
{
	return ( soundBuffers[0].canRead( size ) && soundBuffers[1].canRead( size ) );
}

void mixerChannel::getDataLeft( float * dataOut, unsigned int size )
{
    if(sendVuMeterChanged_left >= 2)
    {
	sendVuMeterChanged_left = 0;
	emit(vuMeterChanged_left(soundBuffers[0].getLastReadAverage()));
    }
    else
	sendVuMeterChanged_left++;

    if(soundBuffers[0].canRead( size ))
	soundBuffers[0].read( dataOut, size );
    else qWarning("read: oupsi this should never happen........");
}

void mixerChannel::getDataRight( float * dataOut, unsigned int size )
{
    if(sendVuMeterChanged_right >= 2)
    {
	sendVuMeterChanged_right = 0;
	emit(vuMeterChanged_right(soundBuffers[1].getLastReadAverage()));
    }
    else
	sendVuMeterChanged_right++;

    if(soundBuffers[1].canRead( size ))
	soundBuffers[1].read( dataOut, size );
    else qWarning("read: oupsi this should never happen........");
}

bool mixerChannel::canAddData( unsigned int size )
{
    return ( soundBuffers[0].canWrite( size ) && soundBuffers[1].canWrite( size ) );
}

void mixerChannel::addDataLeft( float * data, unsigned int size )
{
    if(soundBuffers[0].canWrite( size ))
	soundBuffers[0].write( data, size );
    else qWarning("write: oupsi this should never happen........");
}

void mixerChannel::addDataRight( float * data, unsigned int size )
{
    if(soundBuffers[1].canWrite( size ))
	soundBuffers[1].write( data, size );
    else qWarning("Write: oupsi this should never happen........");
}

void mixerChannel::checkBuffer( )
{
}

unsigned int mixerChannel::getBuffFill( )
{
	return soundBuffers[0].getFill();
}

void mixerChannel::updateSettings( settingsType settings )
{
    this->settings = settings;
    settings.insert("type", type);
    Settings::set("channels/"+uuid.toString(), settings);
    emit( settingsChanged( this->settings ) );
}

/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005, 2006 by Jan Boysen                                *
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
#include "playerchannelstd.h"

playerChannelStd::playerChannelStd(QObject *parent, const char *name)
 : QObject(parent, name), volume(1)
{
	// allocate Stereo Sound Ringbuffer.,...
	soundBuffers = new soundRingBuffer[2];
	soundBuffers[0].setName(name+QString("_left"));
	soundBuffers[1].setName(name+QString("_right"));
	soundBuffers[0].setBufSize(16384);
	soundBuffers[1].setBufSize(16384);

	state = 0;
	volume_left = 1;
	volume_right = 1;
}


playerChannelStd::~playerChannelStd()
{
}

void playerChannelStd::setVolume( int newValue )
{
	volume = (100.f-newValue)/100.f;
}

void playerChannelStd::play( )
{
	state = 1;
}

void playerChannelStd::stop( )
{
	state = 0;
}

void playerChannelStd::pause( )
{
	state = 2;
}

void playerChannelStd::cue( )
{
	state = 3;
}

const bool playerChannelStd::isPlaying( )
{
	if( state == 1)
		return TRUE;
	else
		return FALSE;
}

const bool playerChannelStd::isStopped( )
{
	if( state == 0)
		return TRUE;
	else
		return FALSE;
}

const bool playerChannelStd::isPaused( )
{
	if( state == 2)
		return TRUE;
	else
		return FALSE;
}

const bool playerChannelStd::isCued( )
{
	if( state == 3)
		return TRUE;
	else
		return FALSE;
}

QString playerChannelStd::getName( )
{
	return name;
}

void playerChannelStd::setName( QString newName )
{
	name = newName;
	emit( nameChanged( name ) );
}

int playerChannelStd::getLevelLeft( )
{
	return int(volume_left*100);
}

int playerChannelStd::getLevelRight( )
{
	return int(volume_right*100);
}

/**
 * signal Wrapper only that emits the refresh signal to outside connected stuff.
 */
void playerChannelStd::refresh( )
{
	emit( refreshed() );
}

const unsigned int playerChannelStd::getSmplRate( )
{
	return smplRate;
}

void playerChannelStd::connectPort( )
{
}

void playerChannelStd::disconnectPort( )
{
}

void playerChannelStd::setLevelLeft( int level )
{
	volume_left = level/100.f;
}

void playerChannelStd::setLevelRight( int level )
{
	volume_right = level/100.f;
}

bool playerChannelStd::canGetData( unsigned int size )
{
	return ( soundBuffers[0].canRead( size ) && soundBuffers[1].canRead( size ) );
}

void playerChannelStd::getDataLeft( float * dataOut, unsigned int size )
{
	if(soundBuffers[0].canRead( size ))
		soundBuffers[0].read( dataOut, size );
	else qWarning("oupsi this should never happen........");
}

void playerChannelStd::getDataRight( float * dataOut, unsigned int size )
{
	if(soundBuffers[1].canRead( size ))
		soundBuffers[1].read( dataOut, size );
	else qWarning("oupsi this should never happen........");
}

void playerChannelStd::checkBuffer( )
{
}

unsigned int playerChannelStd::getBuffFill( )
{
	return soundBuffers[0].getFill();
}

const float playerChannelStd::getLevelMeterLeft( )
{
	return soundBuffers[0].getLastReadAverage();
}

const float playerChannelStd::getLevelMeterRight( )
{
	return soundBuffers[1].getLastReadAverage();
}

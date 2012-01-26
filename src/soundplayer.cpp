/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                *
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
#include "soundplayer.h"

soundPlayer::soundPlayer( )
{
	devOpened = FALSE;
	interMixSamples = 1024;
	outputBuffers = new soundRingBuffer[2];
	outputBuffers[0].setName("outputBuffer_left");
	outputBuffers[1].setName("outputBuffer_right");

	mixBufL = new float[interMixSamples];
	mixBufR = new float[interMixSamples];
	tempBufL = new float[interMixSamples];
	tempBufR = new float[interMixSamples];
	chanBufL = new float[interMixSamples];
	chanBufR = new float[interMixSamples];
}


soundPlayer::~soundPlayer()
{
	delete[] outputBuffers;
	delete[] mixBufL;
	delete[] mixBufR;
	delete[] tempBufL;
	delete[] tempBufR;
	delete[] chanBufL;
	delete[] chanBufR;
}

void soundPlayer::mixChannels( )
{
}

const unsigned int soundPlayer::getOutputSampleRate( )
{
	return outRate;
}

bool soundPlayer::isConnected( )
{
	return devOpened;
}


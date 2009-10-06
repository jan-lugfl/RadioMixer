/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 200-2007 by Jan Boysen                                *
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
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <qthread.h>
#include <qmessagebox.h>
#include <samplerate.h>

#include "mixerchannelmanager.h"
#include "mixerchannel.h"
#include "ringbuffer.h"

/**
@author Jan Boysen
*/
class soundPlayer : public QObject
{
public:
    soundPlayer( );
    ~soundPlayer();

    virtual void open( QString device ) = 0;
    virtual void close() = 0;

    virtual const unsigned int getOutputSampleRate( );
    virtual bool isConnected();


protected:
	bool devOpened;

	soundRingBuffer*	outputBuffers;
	float* mixBufL;
	float* mixBufR;
	float* tempBufL;
	float* tempBufR;
	float* chanBufL;
	float* chanBufR;

	unsigned int outRate;

	virtual void mixChannels();

        virtual void fetchSampleData( mixerChannel* channel, float* bufferLeft, float* bufferRight );

	unsigned int 	interMixSamples;
};

#endif

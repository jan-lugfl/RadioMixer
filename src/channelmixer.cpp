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

#include <samplerate.h>

#include "channelmixer.h"
#include "mixerchannelmanager.h"

channelMixer::channelMixer( )
{
    // allocate some space for the mixin buffer
    mixBufL = new float[1024];
    mixBufR = new float[1024];
    tempBufL = new float[1024];
    tempBufR = new float[1024];
    chanBufL = new float[1024];
    chanBufR = new float[1024];
}

channelMixer::~channelMixer()
{
    delete[] mixBufL;
    delete[] mixBufR;
    delete[] tempBufL;
    delete[] tempBufR;
    delete[] chanBufL;
    delete[] chanBufR;
}

void channelMixer::run()
{
    while(1)
    {
	bool hasData = false;
	// empy buffer
	for(unsigned int bufPos = 0;bufPos < 1024; bufPos++)
	{
		mixBufL[bufPos] = 0;
		mixBufR[bufPos] = 0;
	}
	mixerChannelManager::storageType::iterator it;
	for( it = mixerChannelManager::inChannels.begin(); it != mixerChannelManager::inChannels.end(); it++ )
	    if( (*it)->canGetData( 1024 ) )
	    {
		// WOW finaly I've got this mixengine working clipfree.....
		// I had the Idea when I was trying to sleep some days ago :-)
		bool mixed = FALSE;
		float volume = 1.0f;

		fetchSampleData( (*it), chanBufL, chanBufR );
		while(!mixed)
		{
		    mixed = TRUE;
		    for(unsigned int bufPos = 0;bufPos < 1024; bufPos++)
		    {
			tempBufL[bufPos] = (mixBufL[bufPos] + chanBufL[bufPos])*volume;
			tempBufR[bufPos] = (mixBufR[bufPos] + chanBufR[bufPos])*volume;
			if( tempBufL[bufPos] > 1.0f || tempBufL[bufPos] < -1.0f || tempBufR[bufPos] > 1.0f || tempBufR[bufPos] < -1.0f)
			{
			    // mix once more with less Volume......
			    mixed = FALSE;
			    volume *= 0.9f;
			}
		    }
		}
		// move ready mixed Buffers
		for(unsigned int bufPos = 0;bufPos < 1024; bufPos++)
		{
		    mixBufL[bufPos] = tempBufL[bufPos];
		    mixBufR[bufPos] = tempBufR[bufPos];
		}
		hasData = true;
	    }

	// copy ready mixed Buffers to output channels
	if( hasData )
	{
	    for( it = mixerChannelManager::outChannels.begin(); it != mixerChannelManager::outChannels.end(); it++ )
	    {
		// wait until the output channel can take data
		while( !(*it)->canAddData( 1024 ) )
		    msleep(20);
		(*it)->addDataLeft( mixBufL, 1024 );
		(*it)->addDataRight( mixBufR, 1024 );
	    }
	}
	else
	    msleep(100);
    }
}

void channelMixer::fetchSampleData( mixerChannel* channel, float * bufferLeft, float * bufferRight )
{
	unsigned int dataToRead = int( (((double)channel->getSmplRate()/(double)48000)*1024)+1);
	float* fetchBufL = new float[dataToRead];
	float* fetchBufR = new float[dataToRead];

	if( channel->canGetData(dataToRead) )
	{
		channel->getDataLeft( fetchBufL, dataToRead );
		channel->getDataRight(  fetchBufR, dataToRead );
	}
	else
		qWarning( tr("soundPlayer::mixChannels( ): Buffer underrun in Channel ")+channel->getName()+tr(" while Mixing....")+QString::number(channel->getSmplRate()) );

	// resample the Channels Data....
	SRC_DATA* resamplerData;
	resamplerData = new SRC_DATA;

	// resample left channel
	resamplerData->data_in = fetchBufL;
	resamplerData->input_frames = dataToRead;
	resamplerData->data_out = bufferLeft;
	resamplerData->output_frames = 1024;
	resamplerData->src_ratio = (double)48000/(double)channel->getSmplRate();

	src_simple( resamplerData, 2, 1);

	// lets try to resample Right Channel too
	resamplerData->data_in = fetchBufR;
	resamplerData->data_out = bufferRight;
	src_simple( resamplerData, 2, 1);

	delete resamplerData;
	delete[] fetchBufL;
	delete[] fetchBufR;
}

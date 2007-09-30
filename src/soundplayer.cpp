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

const int soundPlayer::addChannel( playerChannelStd* newChannel )
{
	channels.append( newChannel );
	
	return channels.size();
}

void soundPlayer::mixChannels( )
{
	if( outputBuffers[0].canWrite( interMixSamples ) && outputBuffers[1].canWrite( interMixSamples ) )
	{
		// Init Buffers
		for(unsigned int bufPos = 0;bufPos < interMixSamples; bufPos++)
		{
			mixBufL[bufPos] = 0;
			mixBufR[bufPos] = 0;
		}
		Q3ValueVector<playerChannelStd*>::iterator it;
		for( it = channels.begin(); it != channels.end(); it++ )
		{
			if( (*it)->isPlaying() && (*it)->canGetData( interMixSamples ) )
			{
				// WOW finaly I've got this mixengine working clipfree.....
				// I had the Idea when I was trying to sleep some days ago :-)
				bool mixed = FALSE;
				float volume = 1.0f;
	
				fetchSampleData( (*it), chanBufL, chanBufR );
				while(!mixed)
				{
					mixed = TRUE;
					for(unsigned int bufPos = 0;bufPos < interMixSamples; bufPos++)
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
				for(unsigned int bufPos = 0;bufPos < interMixSamples; bufPos++)
				{
					mixBufL[bufPos] = tempBufL[bufPos];
					mixBufR[bufPos] = tempBufR[bufPos];
				}
			}
		}
		//limiter();
		// move ready mixed Buffers
		outputBuffers[0].write(mixBufL, interMixSamples );
		outputBuffers[1].write(mixBufR, interMixSamples );
	}else
		qWarning( tr("mix: Buffer overfllow in output buffer") );
}

void soundPlayer::fetchSampleData( playerChannelStd * channel, float * bufferLeft, float * bufferRight )
{
	unsigned int dataToRead = int( (((double)channel->getSmplRate()/(double)outRate)*interMixSamples)+1);
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
	resamplerData->output_frames = interMixSamples;
	resamplerData->src_ratio = (double)outRate/(double)channel->getSmplRate();

	src_simple( resamplerData, 2, 1);

	// lets try to resample Right Channel too	
	resamplerData->data_in = fetchBufR;
	resamplerData->data_out = bufferRight;
	src_simple( resamplerData, 2, 1);

	delete resamplerData;
	delete[] fetchBufL;
	delete[] fetchBufR;
}

const unsigned int soundPlayer::getOutputSampleRate( )
{
	return outRate;
}

bool soundPlayer::isConnected( )
{
	return devOpened;
}


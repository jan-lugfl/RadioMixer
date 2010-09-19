/* $Id$ */
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

#include <samplerate.h>
#include <math.h>

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
    int buffer_samples = 1024;
    int buffer_size = buffer_samples*sizeof(float);
    while(1)
    {
	bool hasData = false;
	// empy buffer
        memset(mixBufL, 0, buffer_size);
        memset(mixBufR, 0, buffer_size);

	mixerChannelManager::storageType::iterator it;
	for( it = mixerChannelManager::inChannels.begin(); it != mixerChannelManager::inChannels.end(); it++ )
            if( (*it)->canGetData( buffer_samples ) )
	    {
		// WOW finaly I've got this mixengine working clipfree.....
		// I had the Idea when I was trying to sleep some days ago :-)
		bool mixed = FALSE;
		float mix_volume = 1.0f;
                // This complex formular makes a nice logarithmic volume slide....
                float chan_volumeLeft = (exp((*it)->getLevelLeft() * 1.2f)/exp(1.2f))*(*it)->getLevelLeft();
                float chan_volumeRight = (exp((*it)->getLevelRight() * 1.2f)/exp(1.2f))*(*it)->getLevelRight();

		fetchSampleData( (*it), chanBufL, chanBufR );
		while(!mixed)
		{
		    mixed = TRUE;
                    for(unsigned int bufPos = 0;bufPos < buffer_samples; bufPos++)
		    {
                        tempBufL[bufPos] = (mixBufL[bufPos] + chanBufL[bufPos]*chan_volumeLeft)*mix_volume;
                        tempBufR[bufPos] = (mixBufR[bufPos] + chanBufR[bufPos]*chan_volumeRight)*mix_volume;
			if( tempBufL[bufPos] > 1.0f || tempBufL[bufPos] < -1.0f || tempBufR[bufPos] > 1.0f || tempBufR[bufPos] < -1.0f)
			{
			    // mix once more with less Volume......
			    mixed = FALSE;
			    mix_volume *= 0.9f;
			}
		    }
		}
                
		// move ready mixed Buffers
                memcpy(mixBufL, tempBufL, buffer_size);
                memcpy(mixBufR, tempBufR, buffer_size);
                hasData = true;
	    }

	// copy ready mixed Buffers to output channels
	if( hasData )
	{
	    for( it = mixerChannelManager::outChannels.begin(); it != mixerChannelManager::outChannels.end(); it++ )
	    {
		// wait until the output channel can take data
                while( !(*it)->canAddData( buffer_samples ) )
                    msleep(10);
                (*it)->addDataLeft( mixBufL, buffer_samples );
                (*it)->addDataRight( mixBufR, buffer_samples );
	    }
	}
        msleep(10);
    }
}

void channelMixer::fetchSampleData( mixerChannel* channel, float * bufferLeft, float * bufferRight )
{
        unsigned int dataToRead = int( (((double)channel->getSmplRate()/(double)48000)*1024)+1);

	if( channel->canGetData(dataToRead) )
	{
            // resample the Channels Data if required...
            if( channel->getSmplRate() != 48000 )
            {
                float* fetchBufL = new float[dataToRead];
                float* fetchBufR = new float[dataToRead];
                SRC_DATA* resamplerData;
                resamplerData = new SRC_DATA;

                channel->getDataLeft( fetchBufL, dataToRead );
                channel->getDataRight(  fetchBufR, dataToRead );

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

                // clean up....
                delete resamplerData;
                delete[] fetchBufL;
                delete[] fetchBufR;
            }
            else
            {
                channel->getDataLeft( bufferLeft, dataToRead );
                channel->getDataRight(  bufferRight, dataToRead );
            }
        }
        else
        {
            qWarning( QString(tr("channelMixer::fetchSampleData: Buffer underrun in Channel ")+channel->getName()+tr(" while Mixing....")).toAscii() );
            // fill output buffer with empty data to not disturb the sound...
            memset(bufferLeft, 0, 1024*sizeof(float));
            memset(bufferRight, 0, 1024*sizeof(float));
        }
}

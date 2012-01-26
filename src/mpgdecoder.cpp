/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006-2010 by Jan Boysen                                 *
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
#include "mpgdecoder.h"

mpgDecoder::mpgDecoder(FILE* File, QObject *parent )
 : fileDecoder(File, parent )
{
	// use QFile as input Buffer, which is needed by libMad, cause it cannot detect EOF....
	madFile = new QFile();
        madFile->open(fHandle, QIODevice::ReadOnly);

	scanFile();
#ifdef HAVE_MAD
	// Allocate Memory for the MAD decoder Library
	madStream = new mad_stream;
	madFrame = new mad_frame;
	madSynth = new mad_synth;
	madTimer = new mad_timer_t;
	mad_inputBuffer = new unsigned char[163840+MAD_BUFFER_GUARD];
        memset(mad_inputBuffer,0 , 163840+MAD_BUFFER_GUARD);

	// Initiate libMad structs
	mad_stream_init( madStream );
	mad_frame_init( madFrame );
	mad_synth_init( madSynth );
	mad_timer_reset( madTimer );
#endif

	//allocate decoder output ringbuffers
	mad_outputBuffer = new soundRingBuffer[2];
	mad_outputBuffer[0].setName("mad_output1");
	mad_outputBuffer[1].setName("mad_output2");
	returnBuffer = new float*[2];
	returnBuffer[0] = new float[8192];
	returnBuffer[1] = new float[8192];
        memset(returnBuffer[0], 0, 8192*sizeof(float));
        memset(returnBuffer[1], 0, 8192*sizeof(float));

	frameCounter=0;
}


mpgDecoder::~mpgDecoder()
{
	delete []mad_outputBuffer;
	delete []returnBuffer;
}

unsigned int mpgDecoder::decode( float *** data, int count )
{
#ifdef HAVE_MAD
        if(data)
		*data=returnBuffer;

	while( mad_outputBuffer[0].canWrite(1152) )  // well letts refill the Input buffer..........
	{
		if( madStream->buffer == NULL || madStream->error == MAD_ERROR_BUFLEN)  // well letts refill the Input buffer..........
		{
			size_t readSize, remaining;
			unsigned char* readStart;
			unsigned char* guardPtr;

			if( madFile->atEnd() )
				return(0);

			if(madStream->next_frame)  // move remaining bytes to start wich has not been decoded yet...
			{
				remaining = mad_inputBuffer + 163840 - madStream->next_frame;
				memmove( mad_inputBuffer, madStream->next_frame, remaining );
				readStart = mad_inputBuffer + remaining;
				readSize = 163840 - remaining;
			}
			else
			{
				readSize = 163840;
				readStart = mad_inputBuffer;
				remaining = 0;
			}

			// get some more Byte from File...
			unsigned int readCnt = 0;
			while( !madFile->atEnd() && readCnt < readSize)
			{
                            readStart[readCnt] = madFile->read(1).at(0);
                            readCnt++;
			}
			
                        //bei EOF ein paar GUARD 0Bytes anhaengen die MAD benötigt..
			if( madFile->atEnd() )
			{
				guardPtr = readStart + readCnt;
				memset( guardPtr, 0, MAD_BUFFER_GUARD);
				readCnt += MAD_BUFFER_GUARD;
			}
			mad_stream_buffer( madStream, mad_inputBuffer, readCnt+remaining);
			madStream->error = MAD_ERROR_NONE;
		}

		// well lets decode the buffer and get some Music out of it :-)
		mad_frame_decode( madFrame, madStream );
		frameCounter++;

		// first MPeg Frame isnt Played cause it contains noisy Metadata if we Play it :D
		// frame is also not played if we could not decode the whole MPeg frame in case of end of input buffer
		if(madStream->error==MAD_ERROR_BUFLEN || frameCounter ==1)
			continue;

		mad_timer_add( madTimer, madFrame->header.duration );
		mad_synth_frame( madSynth, madFrame );

		// decoding done.. convert sampletype...
		for( unsigned int j=0; j<channels; j++ )
			for(unsigned int i=0; i<madSynth->pcm.length; i++ )
			{
                                float temp = scale( madSynth->pcm.samples[j][i]);
                                mad_outputBuffer[j].write( &temp, 1 );
                        }
	}
	//nice little workarround so the buffer never gets so much back it wants to....
	int dataAvailable = mad_outputBuffer[0].canRead(count*.95)?count*.95:mad_outputBuffer[0].getAvailable()-8;

        // ensure the return buffer is clean...
        memset(returnBuffer[0], 0, 8192*sizeof(float));
        memset(returnBuffer[1], 0, 8192*sizeof(float));

        for( unsigned int j=0; j<channels; j++ )
		mad_outputBuffer[j].read( returnBuffer[j], dataAvailable );

        return dataAvailable;
#endif
}

const int mpgDecoder::getRTime( )
{
#ifdef HAVE_MAD
	return getTime()-mad_timer_count( *madTimer, MAD_UNITS_SECONDS);
#endif
}

const int mpgDecoder::getTime( )
{
	return 9999;
}

const float mpgDecoder::getPosition_Samples( )
{
#ifdef HAVE_MAD
	return getTime()-mad_timer_count( *madTimer, MAD_UNITS_SECONDS);
#endif
}

const float mpgDecoder::getTotal_Samples( )
{
	return 9999999;
}

/* convert libmad's fixed point representation to 16 bit signed integers. This
 * code is taken verbatim from minimad.c. */
inline float mpgDecoder::scale(mad_fixed_t sample)
{
#ifdef HAVE_MAD
   return (float) (sample / (float) (1L << MAD_F_FRACBITS));
#endif
}

const float mpgDecoder::getTotalFrames( )
{
	return maxFrames;
}

const float mpgDecoder::getPlayedFrames( )
{
#ifdef HAVE_MAD
	return mad_timer_count( *madTimer, MAD_UNITS_25_FPS);
#endif
}

const float mpgDecoder::getRemainFrames( )
{
	return (getTotalFrames()<getPlayedFrames())?0:getTotalFrames()-getPlayedFrames( );
}

// the following function has been copied from XMMS' mad input plugin
// Copyright (C) 2001-2002 Sam Clegg
void mpgDecoder::scanFile( )
{
#ifdef HAVE_MAD
  mad_stream scanStream;
  mad_header scanHeader;
  mad_timer_t scanTimer;
  int remainder = 0;
  int data_used = 0;
  int pos = 0;
  unsigned char buffer[4096];

  // reset file, so we can read from the beginning
  madFile->reset();

  mad_stream_init (&scanStream);
  mad_header_init (&scanHeader);
  mad_timer_reset(&scanTimer );
  while (1)
    {
      remainder = scanStream.bufend - scanStream.next_frame;
      memcpy (buffer, scanStream.this_frame, remainder);
	// get some more Byte from File...
	int readCnt = 0;
	unsigned char* readStart;
	readStart = buffer+remainder;
	while( !madFile->atEnd() && readCnt < (4096 - remainder) )
	{
                readStart[readCnt] = madFile->read(1).at(0);
		readCnt++;
	}

      if (madFile->atEnd())
	break;

      mad_stream_buffer (&scanStream, buffer, readCnt + remainder);

      while (1)
        {
	  if (mad_header_decode (&scanHeader, &scanStream) == -1)
	    {
	      if (scanStream.error == MAD_ERROR_BUFLEN)
		{
		  break;
		}
	      if (!MAD_RECOVERABLE (scanStream.error))
		{
		  break;
		}
	      continue;
	    }
	  pos++;
	  data_used += scanStream.next_frame - scanStream.this_frame;
	  mad_timer_add( &scanTimer, scanHeader.duration );
	  if (pos == 1)
	    {
	      sampleRate = scanHeader.samplerate;
	      channels = MAD_NCHANNELS(&scanHeader);
	    }
	}
      if (scanStream.error != MAD_ERROR_BUFLEN)
	break;
    }
   maxFrames = mad_timer_count( scanTimer, MAD_UNITS_25_FPS);

  mad_header_finish (&scanHeader);
  mad_stream_finish (&scanStream);

  // reset the file again, so we can read from the beginning when playing
  madFile->reset();
#endif
}

void mpgDecoder::reset( )
{
#ifdef HAVE_MAD
	madFile->reset();
	mad_timer_reset( madTimer );
	mad_stream_init( madStream );
	mad_frame_init( madFrame );
	frameCounter = 0;
#endif
}

// This is a shorter and faster version of scanFile(), that only scans the samplerate and number of Channels of the stream.
void mpgDecoder::readMetaFromFile( playListItem * pli )
{
#ifdef HAVE_MAD
	// use QFile as input Buffer, which is needed by libMad, cause it cannot detect EOF....
	QFile madFile(pli->getFile());
	madFile.open( QIODevice::ReadOnly );

	unsigned char buffer[65536];

	mad_stream scanStream;
	mad_header scanHeader;

	mad_stream_init (&scanStream);
	mad_header_init (&scanHeader);

	// get some more Byte from File...
	int readCnt = 0;
	while( !madFile.atEnd() && readCnt < 65536 )
	{
                buffer[readCnt] = madFile.read(1).at(0);
		readCnt++;
	}

	if (madFile.atEnd())
		return;

	mad_stream_buffer (&scanStream, buffer, readCnt );

	while (1)
        {
		if (mad_header_decode (&scanHeader, &scanStream) == -1)
		{
			if (scanStream.error == MAD_ERROR_BUFLEN)
				break;
			if (!MAD_RECOVERABLE (scanStream.error))
				break;
			continue;
		}
	}
	pli->setSamplerate( scanHeader.samplerate );
	pli->setChannels( MAD_NCHANNELS(&scanHeader) );

	mad_header_finish (&scanHeader);
	mad_stream_finish (&scanStream);
#endif
// read ID3 Metadata
#ifdef HAVE_ID3TAG
    id3_file* id3f = id3_file_open( pli->getFile().toAscii(), ID3_FILE_MODE_READONLY );
    id3_tag* id3 = id3_file_tag( id3f );

    pli->setArtist( getID3String(id3, ID3_FRAME_ARTIST) );
    pli->setTitle( getID3String(id3, ID3_FRAME_TITLE) );
    pli->setGenre( getID3String(id3, ID3_FRAME_GENRE) );
    id3_file_close( id3f );

#endif
}

#ifdef HAVE_ID3TAG
QString mpgDecoder::getID3String( id3_tag* idtag, const char* field_frame )
{
    QString str;
    id3_frame* frame = id3_tag_findframe( idtag, field_frame, 0 );
    if(frame)
    {
        id3_field* field = id3_frame_field(frame, 1);
        if(field)
            str = (char*)id3_ucs4_utf8duplicate(id3_field_getstrings(field, 0));
    }
    return str;
}
#endif

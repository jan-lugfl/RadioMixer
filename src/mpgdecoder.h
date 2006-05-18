/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006 by Jan Boysen                                      *
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
#ifndef MPGDECODER_H
#define MPGDECODER_H

#include <filedecoder.h>
#include "ringbuffer.h"
#include <qfile.h>

// Includes libMAD for MPEG decoding
#include <mad.h>

/**
MPEG Decoder Class for decodeung MPEG 1 and 2 Layer I,II and II with libmad..

	@author Jan Boysen <trekkie@media-mission.de>
*/
class mpgDecoder : public fileDecoder
{
Q_OBJECT
public:
	mpgDecoder(FILE* File, QObject *parent = 0, const char *name = 0);

	~mpgDecoder();

	virtual unsigned int decode( float*** data, int count );
	virtual void reset( );

	// Stream Infos
	virtual const int getRTime();
	virtual const int getTime() ;
	virtual const float getPosition_Samples();
	virtual const float getTotal_Samples();

	virtual const float getTotalFrames();
	virtual const float getPlayedFrames();
	virtual const float getRemainFrames();

	virtual void setMetaInfos( title* titel);

protected:
	QFile* madFile;
	mad_stream* madStream;
	mad_frame* madFrame;
	mad_synth* madSynth;
	mad_timer_t* madTimer;
	unsigned int frameCounter;

	unsigned char* mad_inputBuffer;
	soundRingBuffer* mad_outputBuffer;
	float** returnBuffer;
	float scale(mad_fixed_t sample);

	virtual void scanFile();

	unsigned int channels;
	unsigned int maxFrames;
};


#endif

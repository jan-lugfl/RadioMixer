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

#ifndef OGGDECODER_H
#define OGGDECODER_H

#include "filedecoder.h"

// Includes OGG/vorbis decoder Headers
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class oggDecoder : public fileDecoder
{
Q_OBJECT
public:
        oggDecoder(FILE* File, QObject *parent = 0);
	~oggDecoder();

	virtual unsigned int decode( float*** data, int count );
	virtual void reset( );

	// Stream Infos
	virtual const int getRTime();
	virtual const int getTime();
	virtual const float getPosition_Samples();
	virtual const float getTotal_Samples();

	virtual const float getTotalFrames();
	virtual const float getPlayedFrames();
	virtual const float getRemainFrames();

	static void readMetaFromFile( playListItem* pli );

private:
	// OGGVorbis Decoder
	OggVorbis_File* decoder;
	int current_section;

};

#endif

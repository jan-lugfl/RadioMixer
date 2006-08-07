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

#include "oggdecoder.h"

oggDecoder::oggDecoder(FILE* File, QObject *parent, const char *name)
 : fileDecoder(File, parent, name)
{
	decoder = new OggVorbis_File;
	if(ov_open( File, decoder, NULL, 0) != 0 )
	{
		throw( new decoderException("OGGDecoder", tr("error opening File")) );
	}
}

oggDecoder::~oggDecoder()
{
	ov_clear( decoder );
	delete decoder;
}

unsigned int oggDecoder::decode( float*** data, int count )
{
	return ov_read_float( decoder, data, count, &current_section);
}

const int oggDecoder::getRTime( )
{
	return int( ov_time_total( decoder, -1 )-ov_time_tell( decoder ));
}

const int oggDecoder::getTime( )
{
	return int(ov_time_tell( decoder ));
}

const float oggDecoder::getPosition_Samples( )
{
	return ov_pcm_tell( decoder );
}

const float oggDecoder::getTotal_Samples( )
{
	return ov_pcm_total( decoder, -1);
}

// depricated
void oggDecoder::setMetaInfos( title * titel )
{
	// first open the File and read the File Infos
	FILE* fHandle = fopen( titel->getFile(), "r");
	OggVorbis_File oggInfo;
	ov_open( fHandle, &oggInfo, NULL, 0 );
	titel->setChannels( oggInfo.vi->channels );
	sampleRate = oggInfo.vi->rate;
	titel->setSamplerate( oggInfo.vi->rate);
	ov_clear( &oggInfo );
}

const float oggDecoder::getTotalFrames( )
{
	return (ov_pcm_total( decoder, -1)/( sampleRate/25) );
}

const float oggDecoder::getPlayedFrames( )
{
	return (ov_pcm_tell( decoder)/( sampleRate/25) );
}

const float oggDecoder::getRemainFrames( )
{
	return getTotalFrames( )-getPlayedFrames( );
}

void oggDecoder::reset( )
{
	ov_raw_seek( decoder, 0);
}



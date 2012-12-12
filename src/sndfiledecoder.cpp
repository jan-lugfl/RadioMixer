/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2012 by Jan Boysen                                      *
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

#include "sndfiledecoder.h"
#include <QDebug>

sndfileDecoder::sndfileDecoder(FILE* File, QObject *parent)
    : fileDecoder(File, parent ), played_frames(0)
{
    sndfile = sf_open_fd(fileno(File), SFM_READ, &sfinfo, true );
    if(!sndfile)
        throw( new decoderException("sndfileDecoder", tr("error opening File")) );

    sampleRate = sfinfo.samplerate;

    returnBuffer = new float*[2];
    returnBuffer[0] = new float[16384];
    returnBuffer[1] = new float[16384];

    buffer = new float[32768];
}

sndfileDecoder::~sndfileDecoder()
{
    delete[] returnBuffer[0];
    delete[] returnBuffer[1];
    delete[] returnBuffer;
    delete[] buffer;
    sf_close( sndfile );
}

unsigned int sndfileDecoder::decode( float*** data, int count )
{
    if(data)
        *data=returnBuffer;

    int ret = sf_readf_float( sndfile, buffer, count );
    played_frames += ret;

    for(int i=0;i<ret;i++)
    {
        returnBuffer[0][i] = buffer[i*2];
        returnBuffer[1][i] = buffer[i*2+1];
    }
    return ret;
}

const int sndfileDecoder::getRTime( )
{
    return sfinfo.frames/sampleRate - getTime();
}

const int sndfileDecoder::getTime( )
{
    return played_frames/sampleRate;
}

const float sndfileDecoder::getPosition_Samples( )
{
    return played_frames;
}

const float sndfileDecoder::getTotal_Samples( )
{
    return sfinfo.frames;
}

const float sndfileDecoder::getTotalFrames( )
{
    return sfinfo.frames/(sampleRate/25);
}


const float sndfileDecoder::getPlayedFrames( )
{
    return played_frames/(sampleRate/25);
}

const float sndfileDecoder::getRemainFrames( )
{
    return getTotalFrames()-getPlayedFrames();
}

void sndfileDecoder::reset( )
{
    played_frames = sf_seek(sndfile, 0, SEEK_SET);
}

QStringList sndfileDecoder::getSupportedFileExtensions()
{
    QStringList list = QStringList();
    list << "bwf" << "wav" << "aif" << "snd" << "paf" << "svx" << "voc" << "w64" << "pvf" << "xi" << "htk" << "fla" << "flac";
    return list;
}

void sndfileDecoder::readMetaFromFile( playListItem * pli )
{
    SF_INFO  my_sfinfo;
    SNDFILE* my_sndfile;

    my_sndfile = sf_open(pli->getFile().toAscii(), SFM_READ, &my_sfinfo );

    // set some generic information we already have in SF_INFO
    pli->setChannels( my_sfinfo.channels );
    pli->setSamplerate( my_sfinfo.samplerate );
    pli->setLength( QTime(0,0,my_sfinfo.frames/my_sfinfo.samplerate) );

    // TODO: Add support for reading CART CHUCK Data once libsndfile supports it
    //       https://github.com/erikd/libsndfile/pull/11

    SF_BROADCAST_INFO bc_info;
    if(sf_command(my_sndfile, SFC_GET_LOOP_INFO, &bc_info, sizeof(bc_info)) == SF_TRUE)
    {
            // read Meta from Broadcast header
    }
    else // try generic get_string otherwise
    {
        const char* str = sf_get_string(my_sndfile, SF_STR_TITLE);
        if(str)
            pli->setTitle( str );
        str = sf_get_string(my_sndfile, SF_STR_ARTIST);
        if(str)
            pli->setArtist( str );
        str = sf_get_string(my_sndfile, SF_STR_GENRE);
        if(str)
            pli->setGenre( str );
    }
    sf_close(my_sndfile);
}

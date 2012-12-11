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

#ifndef SNDFILEDECODER_H
#define SNDFILEDECODER_H

#include "filedecoder.h"

// Includes libsndfile Header
#include <sndfile.h>

#include <QFile>


/**
    @author Jan Boysen <trekkie@media-mission.de>
*/
class sndfileDecoder : public fileDecoder
{
Q_OBJECT
public:
        sndfileDecoder(FILE* File, QObject *parent = 0);
        ~sndfileDecoder();

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
    float*  buffer;
    float** returnBuffer;
    int     played_frames;

    SF_INFO     sfinfo;
    SNDFILE*    sndfile;
};

#endif // SNDFILEDECODER_H

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
#ifndef CHANNELMIXER_H
#define CHANNELMIXER_H

#include <QThread>
#include <mixerchannel.h>

/**
@author Jan Boysen
*/
class channelMixer : public QThread
{
public:
    channelMixer( );
    ~channelMixer( );

    // The main mixer....
    virtual void run();
private:
    virtual void fetchSampleData( mixerChannel* channel, float* bufferLeft, float* bufferRight );

    float*  mixBufL;
    float*  mixBufR;

    float* chanBufL;
    float* chanBufR;

    float* tempBufL;
    float* tempBufR;

    bool running;
};

#endif

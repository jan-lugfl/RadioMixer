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
#ifndef mixerChannel_filePlayer_H
#define mixerChannel_filePlayer_H

#include "mixerchannel.h"
#include "channelbufmngr.h"
#include "filedecoder.h"
#include "playlistitem.h"

#include <stdio.h>
#include <qregexp.h>

#ifdef HAVE_VORBIS
#include "oggdecoder.h"
#endif

#ifdef HAVE_MAD
#include "mpgdecoder.h"
#endif


/**
@author Jan Boysen
*/
class mixerChannel_filePlayer : public mixerChannel
{
  Q_OBJECT

public:
    mixerChannel_filePlayer( const char *name = 0 );
    ~mixerChannel_filePlayer();
	
	// Public Member for the BufferMngr Thread
	virtual void checkBuffer();
	virtual const bool isFileOpen();
	virtual const bool isLooping();
	
	// Specialisations
        virtual QString getType() { return QString("PLAYER"); }
	virtual void setName( QString newName );
	
	// Stream Infos
	const int getRTime();
	const int getTime();
	const float getPosition_Samples();
	const float getTotal_Samples();

	virtual const float getPrerollFrames();
	virtual const float getTotalFrames();
	virtual const float getPlayedFrames();
	virtual const float getRemainFrames();

protected:
	QString fileName;
	FILE* fHandle;
	bool fileOpen;
	bool loopMode;

	fileDecoder* decoder;
	void decode();

	// Buffer Manager
	channelBufMngr* bufferThread;

public slots:
	virtual void open( playListItem* track );
	virtual void close();
	virtual void stop( );
	virtual void play();
	virtual void pause();
	virtual void toggleLoop();

signals:
	void stopped();
	void paused();
	void playing();
	void cued( metaTag );
	void trackEnded();
};

#endif

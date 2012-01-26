/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006-2007 by Jan Boysen                                      *
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
#ifndef FILEDECODER_H
#define FILEDECODER_H

#include <QObject>
#include <QString>

#include "playlistitem.h"

/**
abstract Base Class of all our File Decoders

	@author Jan Boysen <trekkie@media-mission.de>
*/
class fileDecoder : public QObject
{
Q_OBJECT
public:
        fileDecoder(FILE* File, QObject *parent = 0 );
	~fileDecoder() ;

	virtual unsigned int decode(float*** data, int count) =0;
	virtual void reset( ) = 0;

	// Stream Infos
	virtual const int getRTime() = 0;
	virtual const int getTime() = 0;
	virtual const float getPosition_Samples() = 0;
	virtual const float getTotal_Samples() = 0;

	virtual const float getTotalFrames() = 0;
	virtual const float getPlayedFrames() = 0;
	virtual const float getRemainFrames() = 0;

//	Implement in subclasses to read Infos from files...
	static void readMetaFromFile( playListItem* pli );

protected:
	FILE* fHandle;
	unsigned int sampleRate;
};

class decoderException
{
public:
	decoderException( QString newSource, QString newMsg) : source(newSource), msg(newMsg) {};
	~decoderException();
	QString	source;
	QString	msg;
};

#endif

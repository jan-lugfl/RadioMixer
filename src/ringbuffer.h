/* $Id$ */
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
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <qobject.h>
#include <qstring.h>
/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
template <class T>
class ringBuffer
{
public:
    ringBuffer(unsigned int bufSize = 8192);
    ~ringBuffer();

	bool		canRead( unsigned int size );
	bool		canWrite( unsigned int size);

	void		read( T* dataOut, unsigned int size );
	void		write( T* data, unsigned int size );
	void		flush();

	unsigned int getBufferSize();
	void setBufSize( unsigned int newSize);
	unsigned int getFree();
	unsigned int getAvailable();
	unsigned int getFill();
	const T getLastReadAverage();

	//for debugung purpose only..
	void setName(QString newName);
private:
	unsigned int	bufferSize;
	
	T*	ringbuffer;
	T*	readPos;
	T*	writePos;
	QString	name;

	int toEnd( T* pointer );
	int fromStart( T* pointer );

	T lastReadAverage;
};

typedef ringBuffer<float> soundRingBuffer;

#include "ringbuffer.cpp"

#endif

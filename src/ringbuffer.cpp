/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
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

#include <QString>

template <class T>
ringBuffer<T>::ringBuffer(unsigned int bufSize) : bufferSize(bufSize)
{
    this->name = name;
    readPos = ringbuffer = writePos = new T[bufferSize];
    memset(ringbuffer, 0, bufferSize*sizeof(T));
    writePos++;
}

template <class T>
ringBuffer<T>::~ringBuffer()
{
	readPos = writePos = 0;
	delete[] ringbuffer;
}

template <class T>
void ringBuffer<T>::write( T* data, unsigned int size )
{
	while( !canWrite( size ) )
            usleep(2);
	for(unsigned int i=0;i<size;i++)
	{
	    if( toEnd( writePos ) <= 0 )
			writePos = ringbuffer;
	    (*writePos) = data[i];
	    writePos++;
	}
}

template <class T>
bool ringBuffer<T>::canWrite( unsigned int size )
{
	if( size <= getFree() )
		return true;
	else
		return false;
}

template <class T>
void ringBuffer<T>::read( T* dataOut, unsigned int size )
{
	while( !canRead( size ) )
		usleep(2);
	lastReadAverage=0;
	for(unsigned int i=0;i<size;i++)
	{
		if( toEnd( readPos ) <= 0 )
			readPos = ringbuffer;
		dataOut[i] = (*readPos);
		lastReadAverage +=  float((*readPos)<0?-(*readPos):(*readPos));
		readPos++;
	}

	lastReadAverage = lastReadAverage/(size+1);
}

template <class T>
bool ringBuffer<T>::canRead( unsigned int size )
{
	unsigned int dataAvailable = getAvailable();
	if( size < dataAvailable )
		return true;
	else
		return false;
}

template <class T>
int ringBuffer<T>::toEnd( T* pointer )
{
	return (&ringbuffer[bufferSize] - pointer);
}

template <class T>
int ringBuffer<T>::fromStart( T* pointer )
{
	return (pointer - &ringbuffer[0]);
}

template <class T>
unsigned int ringBuffer<T>::getBufferSize( )
{
	return bufferSize;
}

template <class T>
void ringBuffer<T>::flush( )
{
	readPos = writePos = ringbuffer;
        memset(ringbuffer, 0, bufferSize*sizeof(T));
	lastReadAverage = 0;
	writePos++;
}

template <class T>
void ringBuffer<T>::setName( QString newName )
{
	name = newName;
}

template <class T>
void ringBuffer<T>::setBufSize( unsigned int newSize )
{
	T* tempBuffer = new T[newSize];
	// update our read/write pointers
	if( newSize >= bufferSize )
	{
	    for(unsigned int i=0;i<bufferSize;i++)
		tempBuffer[i] = ringbuffer[i];
	    writePos = tempBuffer + fromStart(writePos);
	    readPos = tempBuffer + fromStart(readPos);
	}
	else // if shrinking the buffer reset it...
	    readPos = writePos = tempBuffer;

	delete[] ringbuffer;
	ringbuffer = tempBuffer;
	bufferSize = newSize;
}

/**
 * get the amount free space in the buffer
 * @return 
 */
template <class T>
unsigned int ringBuffer<T>::getFree( )
{
	return bufferSize-getAvailable()-1;
}

/**
 * get the fillrate of the ringbuffer
 * @return returns the Fillrate in %
 */
template <class T>
unsigned int ringBuffer<T>::getFill( )
{
	return (unsigned int)(float(float(getAvailable())/float(bufferSize))*100);
}

template <class T>
unsigned int ringBuffer<T>::getAvailable( )
{
	unsigned int dataAvailable = 0;
	if( writePos > readPos && toEnd(writePos) >= 0 )
		dataAvailable = (writePos - readPos);
	else if( writePos < readPos)
		dataAvailable = toEnd(readPos) + fromStart(writePos);
	return dataAvailable;
}

template <class T>
const T ringBuffer<T>::getLastReadAverage( )
{
	return lastReadAverage;
}

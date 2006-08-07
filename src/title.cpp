/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005, 2006 by Jan Boysen                                *
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
#include "title.h"

title::title( QString file)
 : metaTag("", "", "", QTime())
{
	if( !file.isEmpty())
	{
		QRegExp rx1( "^(.*)/(.*\\..*)$" );
	 	if ( rx1.search( file ) != -1 ) {
			Path = rx1.cap(1);
			Filename = rx1.cap(2);
		}
		else
		{
				qWarning( QString( "unknown Filetype !!!" ) );
		}
		QRegExp rx( "^(.*)/(.*)[ |_]*\\-[ |_]*(.*)$" );
	 	if ( rx.search( Filename ) != -1 ) {
			Artist = rx.cap(2);
			Title = rx.cap(3);
			Artist.replace("_", " ");
			Title.replace("_", " ");
		}
		else
		{
			Title = Filename;
			Title.replace("_", " ");
			Title = Title.left( Title.length()-4);
		}
	}
}

title::title( const title& oldTitle )
{
	Artist = oldTitle.Artist;
	Title = oldTitle.Title;
	Path = oldTitle.Path;
	Filename = oldTitle.Filename;
	Format = oldTitle.Format;
	Time = oldTitle.Time;
	Samplerate = oldTitle.Samplerate;
}

title::~title()
{
}

QString title::getFilepath( )
{
	return Path;
}

QString title::getFilename( )
{
	return Filename;
}

QString title::getTotalTime( )
{
	return Time;
}

unsigned int title::getSamplerate( )
{
	//return e defaul rate of 44100
	return Samplerate?Samplerate:44100;
}

QString title::getFile( )
{
	return Path+"/"+Filename;
}

title & title::operator =( const title newTitle )
{
	Artist = newTitle.Artist;
	Title = newTitle.Title;
	Path = newTitle.Path;
	Filename = newTitle.Filename;
	Format = newTitle.Format;
	Time = newTitle.Time;
	Samplerate = newTitle.Samplerate;
	channels = newTitle.channels;
	return *this;
}

unsigned int title::getChannels( )
{
	return channels?channels:2;
}

void title::setChannels( unsigned int chans )
{
	channels = chans;
}

void title::setSamplerate( unsigned int rate )
{
	Samplerate = rate;
}


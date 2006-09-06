/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006 by Jan Boysen                                *
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
#include "playlistitem.h"

playListItem::playListItem( QString fileName, QObject * parent, const char * name )
 : QObject( parent, name ), state( Normal ), Samplerate( 0 ), channels( 0 )
{
	if( !fileName.isEmpty())
	{
		parseAbsFile( fileName );
		readMeta();
	}
}

playListItem::~playListItem()
{
}

void playListItem::setFile( QString file )
{
	parseAbsFile( file );
	readMeta();
}

const QString playListItem::getFilepath( )
{
	return Path;
}

const QString playListItem::getFilename( )
{
	return Filename;
}

const QString playListItem::getFile( )
{
	return Path+"/"+Filename;
}

unsigned int playListItem::getChannels( )
{
	// by default we play Stereo Tracks
	return channels?channels:2;
}

void playListItem::setChannels( unsigned int chans )
{
	channels = chans;
}

unsigned int playListItem::getSamplerate( )
{
	// return a default rate of 44100
	return Samplerate?Samplerate:44100;
}

void playListItem::setSamplerate( unsigned int rate )
{
	Samplerate = rate;
}

void playListItem::startPlaying()
{
	state = Playing;
	emit( refreshed() );
	emit startToPlay( this );
}

const QString playListItem::getId( )
{
	return getFile();
}

void playListItem::readMeta( )
{
	if( !Filename.isEmpty())
	{
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
	emit refreshed();
}

void playListItem::parseAbsFile( QString file )
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
			qWarning( QString( "unknown Filepath: " )+file );
		}
	}
}

playListItem::songState playListItem::getState()
{
	return state;
}

void playListItem::stopped( )
{
	switch( state )
	{
		case Cued:
			state = Normal;
			break;
		case Playing:
			state = Played;
			break;
	}
	emit( refreshed() );
}

void playListItem::cueing( )
{
	state = Cued;
	emit( refreshed() );
}

QColor playListItem::getBackgroundColor( )
{
	switch( state )
	{
		case Played:
			return lightGray;
		case Cued:
			return  yellow.light( 175 );
		case playListItem::Playing:
			return green.light( 175 );
	}
	return white;
}

bool playListItem::hasCostumBackgroundColor( )
{
	return ( state != Normal );
}

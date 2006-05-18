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
#include "playlst.h"

playLST::playLST(QObject *parent , const char *name )
: QObject(parent, name)
{
	nextTrack=0;
}


playLST::~playLST()
{
}

const QString playLST::getPlayerName( )
{
	return playerName;
}

void playLST::setPlayerName( QString name )
{
	playerName = name;
}

title const playLST::getNextTrack( )
{
	if( count() < 1 || nextTrack >= (count()) )
	{
		return title("");;
	}
	else
	{
		nextTrack++;
		emit( changed() );
		return playList[ nextTrack-1];
	}
}

const int playLST::count( )
{
	return playList.count();
}

const title* playLST::at( int index )
{
	return &playList[index];
}

void playLST::append( playListItem newItem )
{
	playList.append( title( newItem.getFile() ) );
	emit( changed() );
}

void playLST::clear( )
{
	playList.clear();
	emit( changed() );
}

const QString playLST::getFileName( int index )
{
	return playList[index].getFilename();
}

void playLST::delItem( int index )
{
	if( index < count() && index > -1)
	playList.erase( &playList.at( index ) );
	emit( changed() );
}

void playLST::itemUp( int index )
{
	if(index > 0)
	{
		title temp = playList.at( index );
		playList.erase( &playList.at( index ) );
		playList.insert( &playList.at( index-1 ), temp);
	}
	emit( changed() );
}

void playLST::itemDown( int index )
{
	if(index < int(playList.size()-1) )
	{
		title temp = playList.at( index );
		playList.erase( &playList.at( index ) );
		playList.insert( &playList.at( index+1 ), temp);
	}
	emit( changed() );
}

const int playLST::getcurrentTrackIndex( )
{
	return (nextTrack-1);
}

void playLST::loadFromFile( QString fileName )
{
	QFile file( fileName );
	file.open( IO_ReadOnly );
	QTextStream stream( &file );
	while( !stream.atEnd() )
		append( playListItem(stream.readLine()) );
	file.close();
}

void playLST::saveToFile( QString fileName )
{
	QFile file( fileName );
	if( !file.open( IO_WriteOnly ) )
		qWarning( tr("error on saving file !!!") );
	QTextStream stream( &file );
	
	for(unsigned int i=0;i<playList.count();i++)
		stream << playList[i].getFile() << "\n";
		
	file.close();
}

void playLST::cueNext( int index )
{
	nextTrack = index;
}

void playLST::append( title newItem )
{
	playList.append( newItem );
	emit( changed() );
}

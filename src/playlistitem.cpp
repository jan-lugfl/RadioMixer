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
#include "playlistitem.h"

playListItem::playListItem()
{
	meta = new title();
}

playListItem::playListItem( QString file, QString trackName)
{
	meta = new title( file );
	trackName = trackName;
}

playListItem::~playListItem()
{
}

const QString playListItem::getFileName( )
{
	return meta->getFilename();
}

const QString playListItem::getFilePath( )
{
	return meta->getFilepath();
}

const QString playListItem::getTrackName( )
{
	return trackName;
}

void playListItem::setTrackName( QString trackName )
{
	this->trackName = trackName;
}

const QString playListItem::getFile( )
{
	return meta->getFile();
}

void playListItem::setFile( QString file )
{
	meta = new title( file );
}

title * const playListItem::getMeta( )
{
	return meta;
}


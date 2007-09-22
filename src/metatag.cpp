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
#include "metatag.h"

metaTag::metaTag(QString Title)
{
	this->Title = Title;
}

metaTag::metaTag( const QString artist, const QString title, const QString genre, const QTime length )
{
	Artist = artist;
	Title = title;
	Genre = genre;
	Length = length;
}

metaTag::~metaTag()
{
}

QString metaTag::getArtist( )
{
	return Artist;
}

QString metaTag::getTitle( )
{
	return Title;
}

QString metaTag::getSong( )
{
	return Artist+" - "+Title;
}

void metaTag::setArtist( QString artist )
{
	Artist = artist;
}

void metaTag::setTitle( QString title )
{
	Title = title;
}

QString metaTag::getGenre( )
{
	return Genre;
}

QTime metaTag::getLength( )
{
	return Length;
}

void metaTag::setGenre( QString genre )
{
	Genre = genre;
}

void metaTag::setLength( QString length )
{
	Length = QTime::fromString( length );
}

void metaTag::setLength( QTime length )
{
	Length = length;
}

void metaTag::setPreLength( QTime length )
{
	preLength = length;
}

void metaTag::setPreLength( QString length )
{
	preLength = QTime::fromString( length );
}

QTime metaTag::getPreLength( )
{
	return preLength;
}

void metaTag::refreshMeta()
{
}

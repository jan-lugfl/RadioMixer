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
#include "metatag.h"

metaTag::metaTag(QString Title, QObject *parent, const char *name)
 : QObject(parent, name)
{
	this->Title = Title;
}

metaTag::metaTag( const metaTag & oldMeta )
: QObject()
{
	Title = oldMeta.Title;
	Artist = oldMeta.Artist;
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

metaTag & metaTag::operator =( const metaTag oldMeta )
{
	Title = oldMeta.Title;
	Artist = oldMeta.Artist;
	return *this;
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




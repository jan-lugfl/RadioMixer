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

playListItem::playListItem( QListView* parent)
 : QListViewItem( parent )
{
}


playListItem::playListItem( playListItem* parent )
 : QListViewItem( parent )
{
}


playListItem::playListItem( QListView* parent, QString filename )
 : QListViewItem( parent )
{
}

playListItem::playListItem( QListView* parent, metaTag metaData )
 : QListViewItem( parent, metaData.getArtist()+" - "+metaData.getTitle(), metaData.getGenre(), metaData.getLength().toString(Qt::ISODate) )
{
	meta = new metaTag(metaData);
}

playListItem::~playListItem()
{
}

void playListItem::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int alignment )
{
	QColor itemBGColor = cg.base();
	QColor itemFGColor = cg.text();

	if( isSelected() )
	{
		itemBGColor = cg.highlight();
		itemFGColor = cg.highlightedText();
	}else
		if( (itemPos()/height())%2 == 1)
			itemBGColor = cg.highlight().light( 175 );

	QColorGroup _cg( cg );
	_cg.setColor( QColorGroup::Base, itemBGColor );
	_cg.setColor( QColorGroup::Text, itemFGColor );
	QListViewItem::paintCell( p, _cg, column, width, alignment );
}

const QString playListItem::getTrackName( )
{
	return trackName;
}

void playListItem::setTrackName( QString trackName )
{
	this->trackName = trackName;
}

void playListItem::setFile( QString file )
{
//	meta = new title( file );
}

 const metaTag playListItem::getMeta( )
{
	return *meta;
}

const QString playListItem::getFilepath( )
{
}

const QString playListItem::getFilename( )
{
}

const QTime playListItem::getTotalTime( )
{
}

const QString playListItem::getFile( )
{
}

unsigned int playListItem::getChannels( )
{
}

void playListItem::setChannels( unsigned int chans )
{
}

unsigned int playListItem::getSamplerate( )
{
}

void playListItem::setSamplerate( unsigned int rate )
{
}

void playListItem::startPlaying( )
{
}


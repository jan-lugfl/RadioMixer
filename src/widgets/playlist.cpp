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
#include "playlist.h"

playList::playList( QListView* parent, QString name )
 : QListViewItem( parent, name ), cuedInChannel(-1)
{
	setDragEnabled( TRUE);
	setDropEnabled( TRUE);
	setRenameEnabled( 0, TRUE );
}


playList::~playList()
{
}

void playList::cueInChannel( int playerId )
{
	cuedInChannel = playerId;
}

void playList::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int alignment )
{
	QColor itemBGColor = cg.base();
	QColor itemFGColor = cg.text();

	if( isSelected() )
	{
		itemBGColor = cg.highlight();
		itemFGColor = cg.highlightedText();
	}
	QColorGroup _cg( cg );
	_cg.setColor( QColorGroup::Base, itemBGColor );
	_cg.setColor( QColorGroup::Text, itemFGColor );
	QListViewItem::paintCell( p, _cg, column, width, alignment );
}

bool playList::serveChannel( int channelID )
{
	return( channelID == cuedInChannel );
}

playListItem * playList::getNextSong( )
{
	QListViewItemIterator it( this );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLISTVIEWITEM_RTTI)
			return dynamic_cast<playListViewItem*>(*it)->playListEntry;
		++it;
	}
	return NULL;
}




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
#include "playlistitemsongdb.h"

playListItemSongDB::playListItemSongDB( QListView* parent )
 : playListItem(parent)
{
}

playListItemSongDB::playListItemSongDB( QListView* parent, metaTag metaData, const QString id,  unsigned int lastPlayedTS )
 : playListItem(parent, metaData)
{
	lastPlayed = lastPlayedTS;
}

playListItemSongDB::~playListItemSongDB()
{
}

void playListItemSongDB::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int alignment )
{
	QColorGroup _cg = cg;
	playListItem::paintCell( p, _cg, column, width, alignment );

	if( !isSelected() )
	{
		unsigned int playedAgo = QDateTime::currentDateTime().toTime_t() - lastPlayed;

		if( playedAgo < 7200 )
		{  // calculate the color of the Item
			_cg.setColor( QColorGroup::Base, Qt::red.dark( 150-(int(playedAgo/720)*10) ) );
			QListViewItem::paintCell( p, _cg, column, width, alignment );
		}
	}
}


/* $Id:$ */
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
#include "playlistviewitem.h"


playListViewItem::playListViewItem( QListView * parent )
 : QListViewItem( parent )
{
}

playListViewItem::playListViewItem( QListView * parent, QString filename )
 : QListViewItem( parent )
{
}

playListViewItem::playListViewItem( playListViewItem * parent )
 : QListViewItem( parent )
{
}

playListViewItem::~playListViewItem()
{
}

void playListViewItem::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int alignment )
{
	QColor itemBGColor = cg.base();
	QColor itemFGColor = cg.text();

	if( isSelected() )
	{
		itemBGColor = cg.highlight();
		itemFGColor = cg.highlightedText();
	}else
		if( playListEntry->hasCostumBackgroundColor() )
			itemBGColor = playListEntry->getBackgroundColor();
		else
			if( (itemPos()/height())%2 == 1)
				itemBGColor = cg.highlight().light( 175 );

	QColorGroup _cg( cg );
	_cg.setColor( QColorGroup::Base, itemBGColor );
	_cg.setColor( QColorGroup::Text, itemFGColor );
	QListViewItem::paintCell( p, _cg, column, width, alignment );
}



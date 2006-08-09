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
#ifndef PLAYLISTVIEWITEM_H
#define PLAYLISTVIEWITEM_H

#include <playlistitem.h>
#include <qlistview.h>
#include <qdatetime.h>


/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class playListViewItem : public QListViewItem
{
public:
	playListViewItem( QListView* parent );
	playListViewItem( QListView* parent, QString filename );
	playListViewItem( playListViewItem* parent );
	~playListViewItem();

	playListItem* playListEntry;

	virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
};

#endif
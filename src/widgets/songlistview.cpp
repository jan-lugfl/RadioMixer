/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2007 by Jan Boysen                                *
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
#include "songlistview.h"

songListView::songListView(QWidget *parent, const char *name)
 : QListView(parent, name)
{
}

songListView::~songListView()
{
}

QDragObject* songListView::dragObject()
{
	QListViewItem* item = selectedItem();
	QDragObject* dragged = 0;

	switch(item->rtti())
	{
		case PLAYLISTVIEWITEM_RTTI:  // is the currently selected Item a Playlist Item ?
			dragged = new songDragObject( dynamic_cast<playListViewItem*>(item), this, item->text(0) );
			break;

		case PLAYLIST_RTTI:  // is the currently selected Item a Playlist ?
			dragged = new playlistDragObject( dynamic_cast<playList*>(item), this, item->text(0) );
			break;
	};
	return(dragged);
}

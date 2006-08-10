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
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "playlistviewitem.h"

#include <qlistview.h>
#include <qobject.h>

#define PLAYLIST_RTTI 1001

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class playList : public QListViewItem
{
public:
	playList( QListView* parent, QString name = QObject::tr("unnamed Playlist") );
	~playList();

	virtual int rtti() const {return PLAYLIST_RTTI;}
	virtual void cueInChannel( int playerId );
	virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
	virtual bool serveChannel( int channelID );
	virtual playListItem* getNextSong();

protected:
	unsigned int	cuedInChannel;
};

#endif

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
#include <qfile.h>
#include <qdom.h>
#include <qmessagebox.h>
#include <qvaluelist.h>

#define PLAYLIST_RTTI 1001

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class playList : public QListViewItem
{
public:
	playList( QListView* parent, QString name = QObject::tr("unnamed Playlist"), QString file = "" );
	~playList();

	virtual int rtti() const {return PLAYLIST_RTTI;}

	// Overloaded Version of insertItem to set the new order number on insertion,
	virtual void insertItem ( playListViewItem* newItem );
	virtual void cueInChannel( int playerId );
	virtual void notCueInChannel( int playerId );
	virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
	virtual bool serveChannel( unsigned int channelID );
	virtual playListItem* getNextSong();

	virtual void setName( QString );
	virtual QString getName();
	virtual void loadFromFile( QString fileName );
	virtual void saveToFile( QString fileName );
	virtual void save();

	virtual QString toString();

	virtual void setCuePlayed( bool playedDisable );
	virtual bool cuePlayed();

	virtual void nextCueSelected();
	virtual void removePlayed();

	virtual bool acceptDrop ( const QMimeSource * mime ) const;
private:
	virtual playListViewItem* lastItem();

protected:
	virtual void dropped ( QDropEvent * e );
	QValueList<unsigned int>	cuedInChannel;
	QString	fileName;
	playListItem* manualNextSongPtr;
	bool recuePlayed;
};

class playlistDragObject : public QStoredDrag
{
public:
	playlistDragObject(  playList* pl, QWidget* dragSource, const char * name );
	~playlistDragObject();

};

#endif

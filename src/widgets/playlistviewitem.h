/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006-2007 by Jan Boysen                                *
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
#include <playlistitemsongdb.h>
#include <q3listview.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include <q3dragobject.h>
//Added by qt3to4:
#include <QDropEvent>

#define PLAYLISTVIEWITEM_RTTI 1101

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class playListViewItem : public QObject, public Q3ListViewItem
{
Q_OBJECT
public:
	playListViewItem( Q3ListView* parent );
	playListViewItem( Q3ListView* parent, playListItem* item, Q3ListViewItem* after = 0 );
	playListViewItem( Q3ListViewItem* parent, QString newSong, Q3ListViewItem* after = 0 );
	playListViewItem( Q3ListViewItem* parent, QDomDocument domdoc );
	playListViewItem( playListViewItem* parent );
	~playListViewItem();

	playListItem* playListEntry;

	virtual int rtti() const {return PLAYLISTVIEWITEM_RTTI;}
	virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
	virtual QString key ( int column, bool ascending ) const;

	virtual bool acceptDrop ( const QMimeSource * mime ) const;

private:
	void setVote( int vote );

protected:
	virtual void dropped ( QDropEvent * evt );
	QPixmap voteImage;

protected slots:
	virtual void refresh();

};

class songDragObject : public Q3StoredDrag
{
public:
	songDragObject( playListViewItem* item, QWidget* dragSource, const char * name );
	~songDragObject();

};

#endif

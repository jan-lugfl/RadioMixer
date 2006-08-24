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

playList::playList( QListView* parent, QString name, QString file )
 : QListViewItem( parent, name ), cuedInChannel(-1)
{
	setDragEnabled( TRUE);
	setDropEnabled( TRUE);
	setRenameEnabled( 0, TRUE );
	if( !file.isEmpty() )
		loadFromFile( file );
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

bool playList::serveChannel( unsigned int channelID )
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

void playList::loadFromFile( QString fileName )
{
	QDomDocument playListDocument;
	QFile playListFile( fileName );

	playListDocument.setContent( playListFile.readAll() );
	if( !playListDocument.isDocument() || !(playListDocument.doctype().name() == "RadioMixerPlayList") )
	{
		QMessageBox::information( NULL, QObject::tr("RadioMixer - Playlist Manager"), QObject::tr("This seems not to be a valid playlist") );
		return;
	}

}

void playList::saveToFile( QString fileName )
{
	this->fileName = fileName;
	save();
}

void playList::save( )
{
	QDomDocument playListDocument("RadioMixerPlayList");
	QDomElement playList = playListDocument.createElement("playlist");
	playListDocument.appendChild(playList);
	QFile playListFile( fileName );

	if( !playListFile.open( IO_WriteOnly ) )
	{
		QMessageBox::information( NULL, QObject::tr("RadioMixer - Playlist Manager"), QObject::tr("error on saving Playlist..") );
		return;
	}

	QListViewItemIterator it( this );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLISTVIEWITEM_RTTI )
		{
			QDomElement entry = playListDocument.createElement("playlistEntry");
			playListItem* pli = dynamic_cast<playListViewItem*>((*it))->playListEntry;

			entry.setAttribute("file", pli->getFile());

			playList.appendChild(entry);
		}
		++it;
	}
	QTextStream stream( &playListFile );
	stream << playListDocument.toString();
	playListFile.close();
}


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
#include "playlist.h"

playList::playList( QListView* parent, QString name, QString file )
 : QListViewItem( parent, name ), manualNextSongPtr( NULL ), recuePlayed( FALSE )
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
	cuedInChannel.append( playerId );
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
	return( cuedInChannel.findIndex( channelID ) != -1 );
}

playListItem * playList::getNextSong( )
{
	if( manualNextSongPtr )
	{
		playListItem* tmp = manualNextSongPtr;
		manualNextSongPtr = NULL;
		return tmp;
	}else{
		QListViewItem* child = firstChild();
		while( child )
		{
			if( child->rtti() == PLAYLISTVIEWITEM_RTTI )
			{
				playListItem* item = dynamic_cast<playListViewItem*>(child)->playListEntry;
				if( item->getState() <= playListItem::Normal || ( item->getState() == playListItem::Played && recuePlayed ) )
					return item;
			}
			child = child->nextSibling();
		}
	}
qWarning("null.");
	return NULL;
}

void playList::loadFromFile( QString fileName )
{
	QDomDocument playListDocument;
	QFile playListFile( fileName );
	if( !playListFile.open( IO_ReadOnly ) )
	{
		QMessageBox::information( NULL, QObject::tr("RadioMixer - Playlist Manager"), QObject::tr("Error on opening playlist...") );
		return;
	}

	playListDocument.setContent( playListFile.readAll() );
	if( !playListDocument.isDocument() || !(playListDocument.doctype().name() == "RadioMixerPlayList") )
	{
		QMessageBox::information( NULL, QObject::tr("RadioMixer - Playlist Manager"), QObject::tr("This seems not to be a valid playlist") );
		return;
	}
	
	QDomElement dataroot = playListDocument.documentElement();
	setName(dataroot.attribute("name"));
	// get files from playlist
	QDomNode node = dataroot.firstChild();
	while( !node.isNull() )
	{
		QDomElement song = node.toElement();
		(new playListViewItem( this, song.attribute("file")))->moveItem( lastItem() );
		node = node.nextSibling();
	}
}

void playList::saveToFile( QString fileName )
{
	this->fileName = fileName;
	save();
}

void playList::save( )
{
	QFile playListFile( fileName );

	if( !playListFile.open( IO_WriteOnly ) )
	{
		QMessageBox::information( NULL, QObject::tr("RadioMixer - Playlist Manager"), QObject::tr("error on saving Playlist..") );
		return;
	}

	QTextStream stream( &playListFile );
	stream << toString();
	playListFile.close();
}

void playList::setName( QString name )
{
	setText(0, name );
}

QString playList::getName( )
{
	return text(0);
}

void playList::setCuePlayed( bool playedDisable )
{
	recuePlayed = playedDisable;
}

bool playList::cuePlayed( )
{
	return recuePlayed;
}

void playList::nextCueSelected( )
{
	QListViewItemIterator it( this );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLISTVIEWITEM_RTTI )
		{
			playListViewItem* plvi = dynamic_cast<playListViewItem*>((*it));
			if( plvi->isSelected() )
				manualNextSongPtr = plvi->playListEntry;
		}
		++it;
	}
}

void playList::notCueInChannel( int playerId )
{
	cuedInChannel.remove( playerId );
}

void playList::removePlayed( )
{
	QListViewItemIterator it( this );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLISTVIEWITEM_RTTI )
		{
			if( dynamic_cast<playListViewItem*>(*it)->playListEntry->getState() == playListItem::Played )
				takeItem( *it );
		}
		++it;
	}
}

void playList::insertItem( playListViewItem * newItem )
{
	QListViewItem::insertItem( newItem );
	newItem->moveItem( lastItem() );
}

playListViewItem * playList::lastItem( )
{
	QListViewItem* item = firstChild();
	while(item)
		if( item->nextSibling() )
			item = item->nextSibling();
		else
			break;
	return dynamic_cast<playListViewItem*>(item);
}

QString playList::toString()
{
	QDomDocument playListDocument("RadioMixerPlayList");
	QDomElement playList = playListDocument.createElement("playlist");
	playList.setAttribute("name", getName() );
	playListDocument.appendChild(playList);

	QListViewItem* child = firstChild();
	while( child )
	{
		if( child->rtti() == PLAYLISTVIEWITEM_RTTI )
		{
			QDomElement entry = playListDocument.createElement("playlistEntry");
			playListItem* pli = dynamic_cast<playListViewItem*>(child)->playListEntry;
			playList.appendChild( pli->toDomElement( &playListDocument ) );
		}
		child = child->nextSibling();
	}
	return playListDocument.toString();
}

bool playList::acceptDrop(const QMimeSource * mime) const
{
	return mime->provides("application/x-radiomixer-playlistitem") || mime->provides("text/uri-list");
}

void playList::dropped(QDropEvent * evt)
{
	if( evt->provides("application/x-radiomixer-playlistitem") )
	{
		if( evt->source() == listView() ) // do we move the Item in our own list ?
		{
			QListView* sender = dynamic_cast<QListView*>(evt->source());
			sender->selectedItem()->moveItem( this->itemAbove() );
		}else
		{
			QDomDocument doc;
			doc.setContent( evt->encodedData("application/x-radiomixer-playlistitem") );
			if( !doc.isDocument() )
			{
				qWarning( QObject::tr("Received an invalid Document") );
				return;
			}
			playListViewItem* pli = new playListViewItem( this, doc );
			pli->moveItem( lastItem() );

			QListView* sender = dynamic_cast<QListView*>(evt->source());
			if( sender )
				sender->takeItem( sender->selectedItem() );
		}
	}else if( evt->provides("text/uri-list") )
	{
		if( QUriDrag::canDecode( evt ) )
		{
			QStringList uriList;
			QUriDrag::decodeLocalFiles( evt, uriList );
			if(uriList.first() && ( uriList.first().contains(".ogg" ) || uriList.first().contains(".mp3" ) ) )
			{
				playListViewItem* pli = new playListViewItem( this, uriList.first() );
				pli->moveItem( lastItem() );
			}
		}
	}
}

//
// playlistDragObject Class Below
//

playlistDragObject::playlistDragObject( playList* pl, QWidget* dragSource = 0, const char * name = 0) :
QStoredDrag( "application/x-radiomixer-playlist", dragSource, name )
{
	QString str = pl->toString();
	QByteArray* data = new QByteArray( str.length() );

	memcpy( data->data(), str.latin1(), str.length() );
	setEncodedData( *data );
}

playlistDragObject::~playlistDragObject()
{
}

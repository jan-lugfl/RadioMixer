/* $Id:$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2010 by Jan Boysen                                      *
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
#include "playlistmanager.h"
#include "playlistitemsongdb.h"

#include <QFile>

playList::playList(QUuid uuid, QObject *parent) :
    QObject(parent), currentIndex(0)
{
    if(!uuid.isNull())
        this->uuid = uuid;
    else
        this->uuid = QUuid::createUuid();

    // get manager an register myself... The manager connects some signals ans slots...
    playlistManager::getInstance()->registerPlaylist( this );
}

playList::~playList()
{
    // get manager an register myself... The manager connects some signals ans slots...
    playlistManager::getInstance()->unregisterPlaylist( this );

    // destroy all items...
    foreach(playListItem* item, items)
        delete item;
}

QString playList::getName()
{
    return name;
}

QList<playListItem*> playList::getItems()
{
    return items;
}

QUuid playList::getUuid()
{
    return uuid;
}

void playList::reset()
{
    foreach( playListItem* pi, items)
        pi->resetState();
    currentIndex = 0;
    emit( changed() );
}

void playList::rename(QString name)
{
    if(name != this->name)
    {
        this->name = name;
        emit( renamed( this->name ) );
        emit( changed() );
    }
}

void playList::addItem( playListItem* newItem )
{
    items.append( newItem );
    connect( newItem, SIGNAL(refreshed()), this, SLOT(itemChanged()));
    emit( changed() );
}

playListItem* playList::getNext()
{
    if(currentIndex >= items.count())
        return new playListItem("");

    return items[currentIndex++];
}

void playList::itemChanged()
{
    // currently only a signal wrapper...
    emit( changed() );
}

bool playList::loadFromFile( QString filename )
{
    QFile file( filename );
    file.open( QIODevice::ReadOnly );
    bool res = loadFromXML( file.readAll() );
    file.close();
    return res;
}

bool playList::loadFromXML( QString xml )
{
    QXmlStreamReader stream;
    stream.addData( xml );
    if( stream.readNextStartElement())
    {
	if(stream.name() != "playlist")
	    return false;

	// set playlist name...
	rename( stream.attributes().value("name").toString());

	while(stream.readNextStartElement())
	{
	    if( stream.name() == "item")
	    {
		if( stream.attributes().value("type") == "STD" )
		    addItem( new playListItem( stream.attributes().value("file").toString()));
		else if( stream.attributes().value("type") == "SONGDB" )
		    addItem( new playListItemSongDB( stream.attributes().value("id").toString().toInt() ));
	    }
	    stream.skipCurrentElement();
	}
    }else
	return false;

    return true;
}

void playList::saveToFile( QString filename )
{
    QFile file( filename );
    file.open( QIODevice::WriteOnly );
    file.write( getAsXML().toAscii() );
    file.close();
}

QString playList::getAsXML()
{
    QString xml;
    QXmlStreamWriter stream(&xml);
    stream.setAutoFormatting( true );
    stream.writeStartDocument( "1.0", true );
    stream.writeStartElement("playlist");
    stream.writeAttribute( "name", name );
    foreach( playListItem* itm, items )
    {
	stream.writeStartElement( "item" );
	stream.writeAttributes( itm->toXmlStreamAttributes() );
	stream.writeEndElement();
    }
    stream.writeEndDocument();
    return xml;
}

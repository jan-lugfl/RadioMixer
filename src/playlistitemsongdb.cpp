/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006-2010 by Jan Boysen                                *
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

#include "settings.h"
#include "httpcomm.h"

playListItemSongDB::playListItemSongDB( const unsigned int id,  unsigned int lastPlayedTS, QObject *parent )
 : playListItem( "", parent )
{
	lastPlayed = lastPlayedTS;
	songDBId = id;

	apiUrl = Settings::get( "songDBURL", "http://songdb.mm.loc/xmlctrl.pl").toString();

	if(songDBId)
	    readMeta();
}

playListItemSongDB::~playListItemSongDB()
{
}

bool playListItemSongDB::hasCostumBackgroundColor( )
{
	if( state <= Normal )
		return ((QDateTime::currentDateTime().toTime_t() - lastPlayed) < 7200);
	else
		return playListItem::hasCostumBackgroundColor( );
}

QColor playListItemSongDB::getBackgroundColor( )
{
	if( state <= Normal )
	{
		unsigned int playedAgo = QDateTime::currentDateTime().toTime_t() - lastPlayed;

		// calculate the color of the Item
		if( playedAgo < 7200 )
			return QColor(Qt::red).dark( 150-(int(playedAgo/720)*10) );
	}
	return playListItem::getBackgroundColor();
}

const QString playListItemSongDB::getId( )
{
	return QString::number(songDBId);
}

void playListItemSongDB::readMeta( )
{
    lastApiReply = httpComm::post( QNetworkRequest(QUrl(apiUrl)), QString("getSonginfo=1&songID="+QString::number(songDBId)).toAscii() );
    connect( lastApiReply, SIGNAL(finished()), this, SLOT(receiveData()) );
}

void playListItemSongDB::receiveData( )
{
    QXmlStreamReader xml;
    xml.addData( lastApiReply->readAll() );
    if( xml.readNextStartElement() )
    {
	if(xml.name() == "songinfo")
	{
	    xml.readNextStartElement();
	    Filename = Settings::get("songDBBasePath", "/songs/").toString()
		       +xml.attributes().value("relPath").toString()
		       +xml.attributes().value("filename").toString();
	    setArtist( xml.attributes().value("interpret").toString() );
	    setTitle( xml.attributes().value("title").toString() );
	    setVote( xml.attributes().value("rated").toString().toInt() );
	    setGenre( xml.attributes().value("genre").toString() );
	    setLength( xml.attributes().value("length").toString() );
	    setPreLength( xml.attributes().value("preTime").toString() );
	    emit( refreshed() );
	    state = Normal;
	    emit( ready( this ) );
	}
	xml.skipCurrentElement();
    }
    lastApiReply->deleteLater();
}

void playListItemSongDB::setFile( QString file )
{
    parseAbsFile(file);
}

void playListItemSongDB::startPlaying( )
{
    playListItem::startPlaying();
    lastApiReply = httpComm::post( QNetworkRequest(QUrl(apiUrl)), QString("updateLastPlayed=1&songID="+QString::number(songDBId)).toAscii() );
    connect( lastApiReply, SIGNAL(finished()), this, SLOT(receiveData()) );
}

QXmlStreamAttributes playListItemSongDB::toXmlStreamAttributes()
{
    QXmlStreamAttributes attrs;
    attrs.append( QString("type"), getType() );
    attrs.append( QString("id"), QString::number(songDBId) );
    return attrs;
}

void playListItemSongDB::refreshMeta()
{
	readMeta();
}

void playListItemSongDB::load(const unsigned int id )
{
	songDBId = id;
	readMeta();
}

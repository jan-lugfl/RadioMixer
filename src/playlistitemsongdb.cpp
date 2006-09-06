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
#include "playlistitemsongdb.h"

playListItemSongDB::playListItemSongDB( const unsigned int id,  unsigned int lastPlayedTS, QObject *parent, const char *name  )
 : playListItem( "", parent, name )
{
	lastPlayed = lastPlayedTS;
	songDBId = id;

	QSettings* config = new QSettings();
	songDBHndl = new QHttp( this, "httpSocket" );
	songDBHndl->setHost( config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );
	connect( songDBHndl, SIGNAL(done(bool)), this, SLOT(receiveData(bool)));

	songDB = new QHttpRequestHeader( "POST", config->readEntry( "/radiomixer/network/songDBScriptname", "/xmlctrl.pl" ) );
	songDB->setValue( "Host", config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );
	delete config;
}

playListItemSongDB::~playListItemSongDB()
{
	delete songDBHndl;
	delete songDB;
}

bool playListItemSongDB::hasCostumBackgroundColor( )
{
	if( state == Normal )
		return ((QDateTime::currentDateTime().toTime_t() - lastPlayed) < 7200);
	else
		return playListItem::hasCostumBackgroundColor( );
}

QColor playListItemSongDB::getBackgroundColor( )
{
	if( state == Normal )
	{
		unsigned int playedAgo = QDateTime::currentDateTime().toTime_t() - lastPlayed;

		// calculate the color of the Item
		if( playedAgo < 7200 )
			return Qt::red.dark( 150-(int(playedAgo/720)*10) );
	}
	return playListItem::getBackgroundColor();
}

const QString playListItemSongDB::getId( )
{
	return QString::number(songDBId);
}

void playListItemSongDB::readMeta( )
{
	qWarning("playListItemSongDB::readMeta( )");
	songDBHndl->request( *songDB, QString("getSonginfo=1&songID="+QString::number(songDBId)).utf8() );
	emit refreshed();
}

void playListItemSongDB::receiveData( bool )
{
	QSettings* config = new QSettings();
	QDomDocument readdata;

	readdata.setContent( songDBHndl->readAll() );
        if( !readdata.isDocument() )
        {
            qWarning( tr("Received an invalid Document") );
        }else
		if( readdata.doctype().name() == "songDBSongInfo" )
		{
			QDomElement dataroot = readdata.documentElement();
			QDomElement song = dataroot.childNodes().item(0).toElement();

			Filename = config->readEntry( "/radiomixer/network/songDBBasePath", "/songs/" )+song.attribute("relPath")+song.attribute("filename");
			Artist = song.attribute("interpret");
			Title = song.attribute("title");
		}
	delete config;
}

void playListItemSongDB::setFile( QString file )
{
	parseAbsFile(file);
}

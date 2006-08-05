/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005, 2006 by Jan Boysen                                *
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
#include "songdbdlg.h"

songDBDlg::songDBDlg(QWidget *parent, const char *name)
    :songDBDialog(parent, name), state(0)
{
	QSettings* config = new QSettings();

	songDBHndl = new QHttp( this, "httpSocket" );
	songDBHndl->setHost( config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );
	connect( songDBHndl, SIGNAL(done(bool)), this, SLOT(displayData(bool)));

	songDB = new QHttpRequestHeader( "POST", config->readEntry( "/radiomixer/network/songDBScriptname", "xmlctrl.pl" ) );
	songDB->setValue( "Host", config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );

	songDBListView->addColumn(tr("Id"));
	songDBListView->addColumn(tr("Artist"));
	songDBListView->addColumn(tr("Song"));
	songDBListView->addColumn(tr("Genre"));
	songDBListView->addColumn(tr("Length"));

	songDBListView->setColumnWidth(0, 30);
	songDBListView->setColumnWidth(1, 150);
	songDBListView->setColumnWidth(2, 150);
	songDBListView->setColumnWidth(4, 80);

	playlistChannel->insertItem( tr("Global Playlist") );
	genre->insertItem( tr("all Genres") );

	//get Genres from sondDB
	requestData( "getGenres=1" );

	delete config;
}

songDBDlg::~ songDBDlg( )
{
	delete songDBHndl;
	delete songDB;
}

void songDBDlg::displayData( bool )
{
	QSettings* config = new QSettings();
	QDomDocument readdata;
	readdata.setContent( songDBHndl->readAll() );
        if( !readdata.isDocument() )
        {
            qWarning( tr("Received an invalid Document") );
        }else{
		if( readdata.doctype().name() == "songDBSongList" )
		{
			songDBListView->clear();

			QDomElement dataroot = readdata.documentElement();
			QDomNodeList songs = dataroot.childNodes();

			if(songs.count() < 1)
			{
				QListViewItem noSongs = new QListViewItem( songDBListView, "", tr("no songs found") );
			}
			else	
				for(unsigned int i=0;i<songs.count();i++)
				{
					QDomElement songAttr = songs.item(i).toElement();
					playListViewItem mySongs = new playListViewItem( songDBListView, songAttr.attribute("id"), songAttr.attribute("interpret"), songAttr.attribute("title"),
															songAttr.attribute("genre"), songAttr.attribute("length"), QString(songAttr.attribute("lastPlayed")).toInt() );
				}
		}else if( readdata.doctype().name() == "songDBSongInfo" )
		{
			QDomElement dataroot = readdata.documentElement();
			QDomElement song = dataroot.childNodes().item(0).toElement();
			qWarning(config->readEntry( "/radiomixer/network/songDBBasePath", "/songs/" )+song.attribute("relPath")+song.attribute("filename"));
			title songObj( config->readEntry( "/radiomixer/network/songDBBasePath", "/songs/" )+song.attribute("relPath")+song.attribute("filename") );
			songObj.setArtist( song.attribute("interpret") );
			songObj.setTitle( song.attribute("title") );

			switch(state)
			{
				case 1: //cue response
					emit cueTrack( cueChannel->currentText(),  songObj );
					break;
				case 2: //Add to playlist response
					emit addToPlaylist( playlistChannel->currentText(),  songObj );
					break;
				default:
					break;
			}
			state = 0;
		}else if( readdata.doctype().name() == "songDBGenreList" )
		{
			QDomElement dataroot = readdata.documentElement();
			QDomNodeList genres = dataroot.childNodes();

			if(genres.count() < 1)
			{
				genre->insertItem( tr("no genres") );
			}
			else	
				for(unsigned int i=0;i<genres.count();i++)
				{
					QDomElement genreAttr = genres.item(i).toElement();
					Genre myGenre;
					myGenre.id = genreAttr.attribute("id");
					myGenre.name = genreAttr.attribute("name");
					genreList.push_back( myGenre );
					genre->insertItem( genreAttr.attribute("name") );
				}
		}
	}
	delete config;
}

void songDBDlg::requestData( QString query )
{
	songDBHndl->request( *songDB, query.utf8() );
}

void songDBDlg::cue()
{
	if( songDBListView->selectedItem()->text( 0 ) )
	{
		state = 1;
		requestData( "getSonginfo=1&songID="+songDBListView->selectedItem()->text( 0 ) );
	}else
		QMessageBox::critical( this, tr("RadioMixer - Song DB"), tr("no song selected....") );
}

void songDBDlg::search()
{
	QString freigabe = onlyFree->isChecked()?"freigabe=1&":"";
	QString searchGenre = genre->currentItem()>0?"genre="+getGenreId(genre->currentText())+"&":"";
	requestData( freigabe+searchGenre+"query="+searchFor->text() );
}

void songDBDlg::playListAdd()
{
	if( songDBListView->selectedItem()->text( 0 ) )
	{
		state = 2;
		requestData( "getSonginfo=1&songID="+songDBListView->selectedItem()->text( 0 ) );
	}else
		QMessageBox::critical( this, tr("RadioMixer - Song DB"), tr("no song selected....") );
}

QString songDBDlg::getGenreId( QString genre )
{
	QValueVector<Genre>::iterator It;
	for( It = genreList.begin(); It != genreList.end(); ++It )
	{
		if( It->name == genre )
			return It->id;
	}
	return "";
}

void songDBDlg::resizeEvent( QResizeEvent *e )
{
	songDBListView->resize( e->size().width()-18, e->size().height()-110);
	frame3->move( frame3->x(), e->size().height()-95);
	frame4->move( e->size().width()-348, e->size().height()-95);

	songDBDialog::resizeEvent( e );
}


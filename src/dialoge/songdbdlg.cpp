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

	songDBView->insertColumns( 0, 5 );
    	QStringList items;
    	items << tr("Id") << tr("Artist") << tr("Song") << tr("Genre") << tr("Length");
	songDBView->setColumnLabels( items );

	songDBView->setColumnWidth(0, 30);
	songDBView->setColumnWidth(1, 200);
	songDBView->setColumnWidth(2, 300);
	songDBView->setColumnWidth(4, 80);

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
	QDomDocument readdata;
	readdata.setContent( songDBHndl->readAll() );
        if( !readdata.isDocument() )
        {
            qWarning( tr("Received an invalid Document") );
        }else{

		if( readdata.doctype().name() == "songDBSongList" )
		{
			// clear Table..
			while(songDBView->numRows())
				songDBView->removeRow( 0 );

			QDomElement dataroot = readdata.documentElement();
			QDomNodeList songs = dataroot.childNodes();

			if(songs.count() < 1)
			{
				songDBView->insertRows( songDBView->numRows(), 1 );
				songDBView->setText( songDBView->numRows()-1, 2, tr("no songs found") );
			}
			else	
				for(unsigned int i=0;i<songs.count();i++)
				{
					songDBView->insertRows( songDBView->numRows(), 1 );
					QDomElement songAttr = songs.item(i).toElement();
					songDBView->setText( songDBView->numRows()-1, 0, songAttr.attribute(tr("id")));
					songDBView->setText( songDBView->numRows()-1, 1, songAttr.attribute(tr("interpret")));
					songDBView->setText( songDBView->numRows()-1, 2, songAttr.attribute(tr("title")));
					songDBView->setText( songDBView->numRows()-1, 3, songAttr.attribute(tr("genre")));
					songDBView->setText( songDBView->numRows()-1, 4, songAttr.attribute(tr("length")));
				}
		}else if( readdata.doctype().name() == "songDBSongInfo" )
		{
			QDomElement dataroot = readdata.documentElement();
			QDomElement song = dataroot.childNodes().item(0).toElement();
			title songObj("/home/trekkie/OpenRadio/Musik/Archiv/OGG/"+song.attribute("filename"));
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
}

void songDBDlg::requestData( QString query )
{
	songDBHndl->request( *songDB, query.utf8() );
}

void songDBDlg::cue()
{
	if( songDBView->text( songDBView->currentRow(),0 ) )
	{
		state = 1;
		requestData( "getSonginfo=1&songID="+songDBView->text( songDBView->currentRow(),0 ) );
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
	if( songDBView->text( songDBView->currentRow(),0 ) )
	{
		state = 2;
		requestData( "getSonginfo=1&songID="+songDBView->text( songDBView->currentRow(),0 ) );
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




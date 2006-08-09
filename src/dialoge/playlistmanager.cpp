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

#include "playlistmanager.h"

playListManager::playListManager(QWidget *parent, const char *name)
 : playListNG(parent, name)
{
	QSettings* config = new QSettings();

#ifdef ENABLE_SONGDB
	songDBHndl = new QHttp( this, "httpSocket" );
	songDBHndl->setHost( config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );
	connect( songDBHndl, SIGNAL(done(bool)), this, SLOT(displayData(bool)));

	songDB = new QHttpRequestHeader( "POST", config->readEntry( "/radiomixer/network/songDBScriptname", "/xmlctrl.pl" ) );
	songDB->setValue( "Host", config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );

	genre->insertItem( tr("all Genres") );

	//get Genres from sondDB
	requestData( "getGenres=1" );

	songDBPopup = new QPopupMenu( songDBListView );
#else
	delete songDBFrame;
#endif

	// setup the Popup menu of the playListView
	playListPopup = new QPopupMenu( playListView );
	playListPopup->insertItem( tr("new &Playlist"), this, SLOT(createNewPlaylist()) );
	playListPopup->insertItem( tr("&rename Playlist"), this, SLOT(renamePlaylist()) );
	playListPopup->insertItem( tr("&load Playlist"), this, SLOT(loadPlaylist()) );
	playListPopup->insertItem( tr("&save Playlist"), this, SLOT(savePlaylist()) );

	playListView->setRootIsDecorated(TRUE);
	delete config;
}

playListManager::~ playListManager( )
{
}

#ifdef ENABLE_SONGDB
QString playListManager::getGenreId( QString genre )
{
	QValueVector<Genre>::iterator It;
	for( It = genreList.begin(); It != genreList.end(); ++It )
	{
		if( It->name == genre )
			return It->id;
	}
	return "";
}

void playListManager::displayData( bool )
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
/*					playListItemSongDB* mySong = new playListItemSongDB( songDBListView,
										metaTag(
											songAttr.attribute("interpret"),
											songAttr.attribute("title"),
											songAttr.attribute("genre"),
											QTime::fromString(songAttr.attribute("length"))
										),
										songAttr.attribute("id"),
										QString(songAttr.attribute("lastPlayed")).toInt()
					);
					connect( mySong, SIGNAL( startToPlay( playListItem* ) ), this, SLOT( updateLastPlayed( playListItem* ) ) );
*/				}
		}else if( readdata.doctype().name() == "songDBSongInfo" )
		{
			QDomElement dataroot = readdata.documentElement();
			QDomElement song = dataroot.childNodes().item(0).toElement();
//			title songObj( config->readEntry( "/radiomixer/network/songDBBasePath", "/songs/" )+song.attribute("relPath")+song.attribute("filename") );
//			songObj.setArtist( song.attribute("interpret") );
//			songObj.setTitle( song.attribute("title") );

			switch(state)
			{
				case 1: //cue response
					emit cueTrack( cueChannel->currentText(),  dynamic_cast<playListItem*>(songDBListView->selectedItem()) );
					break;
				case 2: //Add to playlist response
//					emit addToPlaylist( playlistChannel->currentText(),  songObj );
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

void playListManager::requestData( QString query )
{
	songDBHndl->request( *songDB, query.utf8() );
}

void playListManager::playListAdd( )
{
	if( songDBListView->selectedItem()->text( 0 ) )
	{
		state = 2;
		requestData( "getSonginfo=1&songID="+songDBListView->selectedItem()->text( 0 ) );
	}else
		QMessageBox::critical( this, tr("RadioMixer - Song DB"), tr("no song selected....") );
}

void playListManager::cue( )
{
	if( songDBListView->selectedItem()->text( 0 ) )
	{
		state = 1;
		requestData( "getSonginfo=1&songID="+songDBListView->selectedItem()->text( 0 ) );
	}else
		QMessageBox::critical( this, tr("RadioMixer - Song DB"), tr("no song selected....") );
}

void playListManager::search( )
{
	QString freigabe = onlyFree->isChecked()?"freigabe=1&":"";
	QString searchGenre = genre->currentItem()>0?"genre="+getGenreId(genre->currentText())+"&":"";
	requestData( freigabe+searchGenre+"query="+searchFor->text() );
}
#endif

void playListManager::resizeEvent( QResizeEvent *e )
{
	miscSplitter->resize( e->size().width()-20, e->size().height()-20);
	playListNG::resizeEvent( e );
}

void playListManager::updateLastPlayed( playListItem * item )
{
	qWarning("updating last played.....");
	requestData( "updateLastPlayed=1&filename="+item->getFilename() );
}

void playListManager::showSongDBContextmenu( QListViewItem * item, const QPoint & pos, int col )
{
	songDBPopup->popup( pos );
}


void playListManager::showPlaylistContextmenu( QListViewItem * item, const QPoint & pos, int col )
{
	playListPopup->popup( pos );
}

void playListManager::createNewPlaylist( QString name )
{
	playList* newPlaylist = new playList( playListView, name );
	newPlaylist->setOpen(TRUE);
	playlistChannel->insertItem( name );
}

void playListManager::loadPlaylist( )
{
   QString s = QFileDialog::getOpenFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("open File Dialog"),
                    tr("load playlist...") );
	if( s.length() >0)
	{
		qWarning("not implemented...");
	}
}

void playListManager::savePlaylist( )
{
   QString s = QFileDialog::getSaveFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("save file Dialog"),
                    tr("save playlist...") );
	if( s.length() >0)
	{
		qWarning("not implemented...");
	}
}

void playListManager::renamePlaylist( )
{
	if(playListView->selectedItem())
		playListView->selectedItem()->startRename(0);
}


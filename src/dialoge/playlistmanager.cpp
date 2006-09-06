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

	playlistChannel->insertItem(tr("-- selected --"));

	connect( playListView, SIGNAL( itemRenamed( QListViewItem*, int) ), this, SLOT( refreshPlaylists() ) );
	connect( songDBListView, SIGNAL( doubleClicked( QListViewItem*, const QPoint& , int) ), this, SLOT( songDBViewdoubleClicked( QListViewItem *, const QPoint &, int ) ) );
#else
	delete songDBFrame;
#endif

	// setup the Popup menu of the playListView
	playListPopup = new QPopupMenu( playListView );
	playListPopup->insertItem( tr("new &Playlist"), 0);
	playListPopup->connectItem(0, this, SLOT(createNewPlaylist()) );
	playListPopup->insertItem( tr("&rename Playlist"), 1);
	playListPopup->connectItem(1,  this, SLOT(renamePlaylist()) );
	playListPopup->insertItem( tr("&load Playlist"), 2);
	playListPopup->connectItem(2, this, SLOT(loadPlaylist()) );
	playListPopup->insertItem( tr("&save Playlist"), 3);
	playListPopup->connectItem(3, this, SLOT(savePlaylist()) );
	playListPopup->insertSeparator();

	playListPopupChannelList = new QPopupMenu( playListPopup );
	connect( playListPopupChannelList, SIGNAL(activated(int)), this, SLOT(cuePlaylist(int)) );
	playListPopup->insertItem( tr("&cue playlist in"), playListPopupChannelList, 4 );

	playListPopup->insertSeparator();

	playListPopup->insertItem( tr("&add Track"), 5);
	playListPopup->connectItem(5, this, SLOT(addNewTrackToPlaylist()) );

	playListPopup->insertSeparator();

	playListPopupOptions = new QPopupMenu( playListPopup );
	playListPopupOptions->setCheckable( TRUE );
	playListPopup->insertItem( tr("&Options"), playListPopupOptions, 6 );

	// setup the playlist View
	playListView->setRootIsDecorated(TRUE);

	playList* newPlaylist = new playList( playListView, tr("default playlist") );
	newPlaylist->setOpen(TRUE);
	refreshPlaylists();

	connect( &refreshTimer, SIGNAL(timeout()), playListView, SLOT(triggerUpdate()) );
	refreshTimer.start(1000);

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
					playListItemSongDB* listEntry = new playListItemSongDB(
											QString(songAttr.attribute("id")).toInt(),
											QString(songAttr.attribute("lastPlayed")).toInt()
										);
					listEntry->setFile(config->readEntry( "/radiomixer/network/songDBBasePath", "/songs/" )+songAttr.attribute("relPath")+songAttr.attribute("filename"));
					listEntry->setArtist(songAttr.attribute("interpret"));
					listEntry->setTitle(songAttr.attribute("title"));
					listEntry->setGenre(songAttr.attribute("genre"));
					listEntry->setLength(songAttr.attribute("length"));

					playListViewItem* mySong = new playListViewItem( songDBListView, listEntry);
					connect( listEntry, SIGNAL( startToPlay( playListItem* ) ), this, SLOT( updateLastPlayed( playListItem* ) ) );
				}
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
	if( songDBListView->selectedItem() )
		if( playlistChannel->currentItem() == 0)
		{
			if( playListView->selectedItem() )
			{
				playListViewItem* item = dynamic_cast<playListViewItem*>(songDBListView->selectedItem());
				songDBListView->takeItem( item );
				playListView->selectedItem()->insertItem( item );
			}
			else
				QMessageBox::critical( this, tr("RadioMixer - Playlist Manager"), tr("no playlist selected....") );
		}else{
			QListViewItemIterator it( playListView );
			while( it.current() )
			{
				if( (*it)->text(0) == playlistChannel->currentText())
				{
					playListViewItem* item = dynamic_cast<playListViewItem*>(songDBListView->selectedItem());
					songDBListView->takeItem( item );
					(*it)->insertItem( item );
					break;
				}
				++it;
			}

		}
	else
		QMessageBox::critical( this, tr("RadioMixer - Playlist Manager"), tr("no song selected....") );
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

void playListManager::refreshPlaylists()
{
	playlistChannel->clear();
	playlistChannel->insertItem(tr("-- selected --"));

	QListViewItemIterator it( playListView );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLIST_RTTI )
			playlistChannel->insertItem( (*it)->text(0) );
		++it;
	}
}

void playListManager::songDBViewdoubleClicked( QListViewItem * item, const QPoint &, int )
{
	if( item )
		playListAdd();
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
	// enable all Items
	for(int i=0;i<6;i++)
		playListPopup->setItemEnabled( i, TRUE );

	if( item )
	{
		currentlySelectedItem = item;
		switch(item->rtti())
		{
		case PLAYLISTVIEWITEM_RTTI:  // is the currently selected Item a Playlist Item ?
			for(int i=0;i<5;i++)
				playListPopup->setItemEnabled( i, FALSE);
			playListPopupOptions->clear();

			playListPopupOptions->insertItem( tr("cue next"), 1 );
			playListPopupOptions->connectItem(1, this, SLOT( cueAsNextTreck() ) );

			break;
		case PLAYLIST_RTTI:  // is the currently selected Item a Playlist ?
			playListPopupChannelList->clear();
			QValueVector<filePlayer>::iterator playerIt;
			for( playerIt = filePlayers.begin(); playerIt != filePlayers.end(); ++playerIt )
				playListPopupChannelList->insertItem( (*playerIt).name, (*playerIt).id );

			playList* plst = dynamic_cast<playList*>(item);
			playListPopupOptions->clear();
			playListPopupOptions->insertItem( tr("recue played"), 1 );
			playListPopupOptions->setItemChecked( 1, plst->cuePlayed() );
			playListPopupOptions->connectItem(1, this, SLOT(setCuePlayed()) );

			playListPopupOptions->insertItem( tr("reset states"), 2 );
			playListPopupOptions->connectItem(2, this, SLOT(resetPlaylistStates()) );

			break;
		}
	}else
	{
		playListPopup->setItemEnabled( 1, FALSE);
		playListPopup->setItemEnabled( 3, FALSE);
		playListPopup->setItemEnabled( 4, FALSE);
		playListPopup->setItemEnabled( 5, FALSE);
	}
	playListPopup->popup( pos );
}

void playListManager::createNewPlaylist( QString name )
{
	playList* newPlaylist = new playList( playListView, name );
	newPlaylist->setOpen(TRUE);
	refreshPlaylists();
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
		playList* newPlaylist = new playList( playListView, "", s );
		newPlaylist->setOpen(TRUE);
	}
	refreshPlaylists();
}

void playListManager::savePlaylist( )
{
	if( !currentlySelectedItem->rtti() == PLAYLIST_RTTI )
		return;

	QString s = QFileDialog::getSaveFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("save file Dialog"),
                    tr("save playlist...") );
	if( s.length() >0)
	{
		dynamic_cast<playList*>(currentlySelectedItem)->saveToFile( s );
	}
}

void playListManager::renamePlaylist( )
{
	if(playListView->selectedItem())
		playListView->selectedItem()->startRename(0);
}

void playListManager::addPlayer( filePlayer player )
{
	filePlayers.append( player );
	cueChannel->insertItem( player.name );
}

void playListManager::removePlayer( unsigned int playerId )
{
	QValueVector<filePlayer>::iterator playerIt;
	for( playerIt = filePlayers.begin(); playerIt != filePlayers.end(); ++playerIt )
		if( playerId == (*playerIt).id )
			filePlayers.erase( playerIt );
}

void playListManager::updatePlayer( filePlayer player )
{
	QValueVector<filePlayer>::iterator playerIt;
	for( playerIt = filePlayers.begin(); playerIt != filePlayers.end(); ++playerIt )
		if( player.id == (*playerIt).id )
			( *playerIt ) = player;
}

void playListManager::cuePlaylist( int item )
{
	QListViewItemIterator it( playListView );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLIST_RTTI )
			dynamic_cast<playList*>((*it))->notCueInChannel( item );
		++it;
	}
	dynamic_cast<playList*>(currentlySelectedItem)->cueInChannel( item );
}

void playListManager::cueNewTrack( unsigned int playerId )
{
	QListViewItemIterator it( playListView );
	while( it.current() )
	{
		if( (*it)->rtti() == PLAYLIST_RTTI )
		{
			playList* pl = dynamic_cast<playList*>(*it);
			if( pl->serveChannel( playerId) )
			{
				playListItem* pli = pl->getNextSong();
				if( pli )
					emit cueTrack( playerId, pli );
				break;
			}
		}
		++it;
	}
}

void playListManager::addNewTrackToPlaylist( )
{
	QString extensions = ""
#ifdef HAVE_VORBIS
   +QString(" *.ogg")
#endif
#ifdef HAVE_MAD
   +QString(" *.mp2 *.mp3")
#endif
   ;
   QString fileTypes = tr("all supported soundfiles (%1)").arg(extensions)
#ifdef HAVE_VORBIS
   +QString(";;OGG/Vorbis (*.ogg)")
#endif
#ifdef HAVE_MAD
   +QString(";;MPEG-1 Layer III (*.mp2 *.mp3)")
#endif
   ;
	QString s = QFileDialog::getOpenFileName(
		"",
		fileTypes,
		this,
		tr("load file dialog"),
		tr("append File to Playlist")+"..." );
	if( s.length() >0)
	{
		switch(currentlySelectedItem->rtti()) 
		{
		case PLAYLISTVIEWITEM_RTTI:  // is the currently selected Item a Playlist Item ?
			new playListViewItem(currentlySelectedItem->parent(), s);
			break;
		case PLAYLIST_RTTI:  // is the currently selected Item a Playlist ?
			new playListViewItem(currentlySelectedItem, s);
			break;
		}
	}
}

void playListManager::playlistViewdoubleClicked( QListViewItem *, const QPoint &, int )
{
}

void playListManager::setCuePlayed( )
{
	if( currentlySelectedItem )
		dynamic_cast<playList*>(currentlySelectedItem)->setCuePlayed( !playListPopupOptions->isItemChecked( 1 ) );
}

void playListManager::resetPlaylistStates( )
{
	if( currentlySelectedItem->rtti() ==PLAYLIST_RTTI )
	{
		QListViewItemIterator it( currentlySelectedItem );
		while( it.current() )
		{
			if( (*it)->rtti() == PLAYLISTVIEWITEM_RTTI )
			{
				dynamic_cast<playListViewItem*>((*it))->playListEntry->resetState();
			}
			++it;
		}
	}
}

void playListManager::cueAsNextTreck( )
{
	if( currentlySelectedItem->parent()->rtti() ==PLAYLIST_RTTI )
		dynamic_cast<playList*>(currentlySelectedItem->parent())->nextCueSelected();
}


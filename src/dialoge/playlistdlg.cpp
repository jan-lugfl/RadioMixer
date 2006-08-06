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


#include "playlistdlg.h"

playListDlg::playListDlg(QWidget* parent, const char* name, bool modal, WFlags fl)
    : Playlist(parent,name, modal,fl)
{}

playListDlg::~playListDlg()
{}

void playListDlg::addGlobalTrack()
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
                    tr("Open File"),
                    tr("Add file to playlist...") );
	if( s.length() >0)
	{
//		globalPlaylist->append( playListItem( s) );
	}
}

void playListDlg::addLocalTrack()
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
                    tr("Open File"),
                    tr("Add file to playlist...") );
	if( s.length() >0)
	{
//		(*localPlaylists)[playerSelect->currentItem()]->append( playListItem( s) );
	}
}

void playListDlg::loadLocalPlaylist()
{
   QString s = QFileDialog::getOpenFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("open File Dialog"),
                    tr("load playlist...") );
	if( s.length() >0)
	{
		(*localPlaylists)[playerSelect->currentItem()]->loadFromFile( s );
	}

}

void playListDlg::loadGlobalPlaylist()
{
   QString s = QFileDialog::getOpenFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("open File Dialog"),
                    tr("load playlist...") );
	if( s.length() >0)
	{
		globalPlaylist->loadFromFile( s );
	}
}

void playListDlg::saveGlobalPlaylist()
{
   QString s = QFileDialog::getSaveFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("save file Dialog"),
                    tr("save playlist...") );
	if( s.length() >0)
	{
		globalPlaylist->saveToFile( s );
	}
}

void playListDlg::saveLocalPlaylist()
{
   QString s = QFileDialog::getSaveFileName(
                    "",
                    "RadioMixer Playlist (*.plst)",
                    this,
                    tr("save file dialog"),
                    tr("save playlist...") );
	if( s.length() >0)
	{
		(*localPlaylists)[playerSelect->currentItem()]->saveToFile( s );
	}
}

void playListDlg::delLocalEntry()
{
	(*localPlaylists)[playerSelect->currentItem()]->delItem( playerSelect->currentItem() );
}


void playListDlg::delGlobalEntry()
{
//	globalPlaylist->delItem( Playlist_Global->currentItem() );
}

void playListDlg::showPlaylistHelp()
{
	Help_dlg hilfeDialog;
	hilfeDialog.exec();
}

void playListDlg::refreshPlaylists( )
{
	int savedPlayer = playerSelect->currentItem();
	int listItemNr = 1;
	QString playing = "";
	// Set Local Player playlists
	playerSelect->clear();
	if( localPlaylists->count() > 0 )
	{
		QValueVector< playLST * >::iterator locPlayit;
        	for( locPlayit = localPlaylists->begin(); locPlayit != localPlaylists->end(); ++locPlayit )
			playerSelect->insertItem( (*locPlayit)->getPlayerName(), -1 );
	
		playerSelect->setCurrentItem( savedPlayer );
		// get Local Playlist Entries
		Playlist_Local->clear();
       		for( int loc = 0; loc < (*localPlaylists)[playerSelect->currentItem()]->count(); loc++ )
		{
//			Playlist_Local->insertItem( QString::number( listItemNr )+". "+(*localPlaylists)[playerSelect->currentItem()]->getFileName( loc ) );
			listItemNr++;
		}
		LCDGesammt_local->display( (int)(*localPlaylists)[playerSelect->currentItem()]->count() );
	}
	
	
	// get Global Playlist Entries
	listItemNr = 1;
	Playlist_Global->clear();
        for( int glob = 0; glob < globalPlaylist->count(); glob++ )
	{
		if( (listItemNr-1) == globalPlaylist->getcurrentTrackIndex())
			playing = "    CUED >>";
		else
			playing = "";
//		Playlist_Global->insertItem( QString::number( listItemNr )+". "+globalPlaylist->getFileName( glob )+playing );
		listItemNr++;
	}
	LCDGesammt_global->display( (int)globalPlaylist->count() );
}

void playListDlg::setPlaylistPointers( playLST * global, QValueVector< playLST * > * local )
{
	globalPlaylist = global;
	localPlaylists = local;
}

void playListDlg::delLocalAll()
{
	(*localPlaylists)[playerSelect->currentItem()]->clear();
}


void playListDlg::delGlobalAll()
{
	globalPlaylist->clear();
}

void playListDlg::globalMoveUp()
{
//	globalPlaylist->itemUp( Playlist_Global->currentItem() );
}

void playListDlg::globalMoveDown()
{
//	globalPlaylist->itemDown( Playlist_Global->currentItem() );
}

void playListDlg::localMoveDown()
{
//	(*localPlaylists)[playerSelect->currentItem()]->itemUp( Playlist_Local->currentItem() );
}

void playListDlg::localMoveUp()
{
//	(*localPlaylists)[playerSelect->currentItem()]->itemDown( Playlist_Local->currentItem() );
}

void playListDlg::localCueNext()
{
//	(*localPlaylists)[playerSelect->currentItem()]->cueNext( Playlist_Local->currentItem() );
}


/*$SPECIALIZATION$*/

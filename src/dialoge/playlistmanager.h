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

#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include "playlistng.h"

#include "widgets/playlist.h"
#include "playlistviewitem.h"

#ifdef ENABLE_SONGDB
#include "playlistitemsongdb.h"
#include <qhttp.h>
#include <qurloperator.h>
#endif

#include <qdom.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qsettings.h>
#include <qvaluevector.h>
#include <qmessagebox.h>
#include <qsplitter.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <qtimer.h>

class playListManager: public playListNG {
Q_OBJECT

public:
	playListManager(QWidget *parent = 0, const char *name = 0);
	~playListManager();

	struct filePlayer {
		unsigned int id;
		QString name;
		QColor color;
	};
	virtual void addPlayer( filePlayer player );
	virtual void removePlayer( unsigned int playerId );
	virtual void updatePlayer( filePlayer player );

protected:
	QPopupMenu*	playListPopup;
	QPopupMenu*	playListPopupChannelList;
	QValueVector<filePlayer>	filePlayers;

	virtual void resizeEvent ( QResizeEvent *e );

#ifdef ENABLE_SONGDB
private:
	int state;
	// this is used to know in Contextmenu handling which Item to handle.
	QListViewItem*	currentlySelectedItem;

private slots:
	void refreshPlaylists();
	QTimer refreshTimer;

protected:
	QHttp*	songDBHndl;
	QHttpRequestHeader* songDB;
	struct Genre {
		QString id;
		QString name;
	};
	QValueVector<Genre>	genreList;
	QPopupMenu*	songDBPopup;
	QString getGenreId( QString genre );

protected slots:
	virtual void displayData( bool );
	virtual void requestData( QString query );
	virtual void playListAdd();
	virtual void cue();
	virtual void search();
	virtual void showSongDBContextmenu( QListViewItem * item, const QPoint & pos, int col );
	virtual void songDBViewdoubleClicked( QListViewItem *, const QPoint &, int );
#endif

public slots:
	virtual void createNewPlaylist( QString name=tr("unnamed Playlist") );
	virtual void cueNewTrack( unsigned int playerId );

protected slots:
	virtual void updateLastPlayed( playListItem* item ); 
	virtual void showPlaylistContextmenu( QListViewItem * item, const QPoint & pos, int col );
	virtual void playlistViewdoubleClicked( QListViewItem *, const QPoint &, int );
	virtual void loadPlaylist();
	virtual void savePlaylist();
	virtual void renamePlaylist();
	virtual void cuePlaylist( int item );
	virtual void addNewTrackToPlaylist();

signals:
	void cueTrack( unsigned int, playListItem* );

};

#endif

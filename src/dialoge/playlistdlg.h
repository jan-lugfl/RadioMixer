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

#ifndef PLAYLISTDLG_H
#define PLAYLISTDLG_H

#include "playlist.h"
#include <qfiledialog.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qlcdnumber.h>
#include <qvaluevector.h>

#include "playlst.h"

// include other dialogs
#include "help_playlist_dlg.h"

class playListDlg : public Playlist
{
  Q_OBJECT

public:
  playListDlg(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~playListDlg();
  
  void setPlaylistPointers(playLST* global, QValueVector<playLST*>* local );
  /*$PUBLIC_FUNCTIONS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

private:
	playLST* globalPlaylist;
	QValueVector<playLST*>* localPlaylists;

protected slots:
    virtual void delLocalAll();
    virtual void delGlobalAll();
  /*$PROTECTED_SLOTS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
    virtual void addGlobalTrack();
    virtual void saveLocalPlaylist();
    virtual void saveGlobalPlaylist();
    virtual void loadGlobalPlaylist();
    virtual void loadLocalPlaylist();
    virtual void addLocalTrack();
    virtual void showPlaylistHelp();
    virtual void delLocalEntry();
    virtual void delGlobalEntry();
    virtual void refreshPlaylists();
    virtual void localCueNext();
    virtual void localMoveDown();
    virtual void localMoveUp();
    virtual void globalMoveDown();
    virtual void globalMoveUp();

};

#endif


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

#ifndef MAINFORMDLG_H
#define MAINFORMDLG_H

#include "version.h"
#include "mainform.h"
#include "mixerguiplayer.h"

#ifdef HAVE_ALSA
#include "soundplayeralsa.h"
#include "mixerguialsamix.h"
#endif

#ifdef HAVE_JACK
#include "mixerguijackport.h"
#include "soundplayerjack.h"
#endif

#ifdef ENABLE_HWMIXER
#include "mipucom.h"
#endif

#ifdef ENABLE_SONGDB
#include "songdbdlg.h"
#endif

#include "playlst.h"
#include <qlineedit.h>
#include <qvaluevector.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qaction.h>
#include <qtabwidget.h>

// Andere Dialoge einbinden
#include "prefdlg.h"
#include "playlistdlg.h"
#include "metainfo.h"
#include "ch_add_dlg.h"
#include "ch_del_dlg.h"
#include "ch_move_dlg.h"
#include "cuelistdlg.h"
#include "aboutDialog.h"

class mainFormDlg : public RadioMixer
{
  Q_OBJECT

public:
  mainFormDlg(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~mainFormDlg();
    /*$PUBLIC_FUNCTIONS$*/

protected:
	virtual void keyPressEvent( QKeyEvent *k );
	QLabel*   soundStatus;
	QLabel*   mixerStatus;
  /*$PROTECTED_FUNCTIONS$*/

 private:
 	soundPlayer* player;
	playLST globalPlaylist;
#ifdef ENABLE_HWMIXER
	mipuCom* miPu;
	QTimer refTimer;
#endif
	QValueVector<playLST*> localPlaylistPointers;
	QValueVector<mixerChannelGUI*> playerGuis;
		
	// Settings storage
	QSettings* config;

	// Meta Info for Live Streaming with Ices
	metaInfo meta;

public slots:
  /*$PUBLIC_SLOTS$*/
    virtual void showPreferences();
    virtual void showPlaylist();
    virtual void addNewFilePlayer();
    virtual void addNewMixerChannel();
    virtual void addNewJackChannel( QString chName );
    virtual void globalPlaylist_getNextTrack( QString player );
    virtual void netDeviceSliderLoop();
    virtual void connectNetDevice();
    virtual void refreshSliders();

    virtual void showMoveChannel();
    virtual void showDelChannel();
    virtual void showAddChannel();
    virtual void showSongDBDialog();

    virtual void cueTitle( QString player, title song );
    virtual void playlistAddTitle( QString player, title song );
    virtual void showAbout();

protected slots:
  /*$PROTECTED_SLOTS$*/
	virtual void windowResize();

private slots:
    virtual void handleNetworkDeviceErrors( int error );
    virtual void networkDeviceStatusRefresh( );
    virtual void saveSettings();
    virtual void showCuesWindow();
    virtual void startALSA();
    virtual void disconnectJackd();
    virtual void connectJackd();

};

#endif


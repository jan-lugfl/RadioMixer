/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2007 by Jan Boysen                                *
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
#include "ui_mainform.h"
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

#include <qlineedit.h>
#include <q3valuevector.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qstatusbar.h>
#include <q3popupmenu.h>
#include <qaction.h>
#include <qtabwidget.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QLabel>
#include <QKeyEvent>

// Andere Dialoge einbinden
#include "playlistmanager.h"
#include "prefdlg.h"
#include "metainfo.h"
#include "cuelistdlg.h"

class mainFormDlg : public Ui::RadioMixer
{
public:
  mainFormDlg( );
  ~mainFormDlg();
    /*$PUBLIC_FUNCTIONS$*/

protected:
	virtual void keyPressEvent( QKeyEvent *k );
	QLabel*   soundStatus;
	QLabel*   mixerStatus;
	virtual void reorderChannels();

  /*$PROTECTED_FUNCTIONS$*/

 private:
#ifdef ENABLE_HWMIXER
	mipuCom* miPu;
	QTimer refTimer;
#endif
	Q3ValueVector<mixerChannelGUI*> playerGuis;

	playListManager*	playListMgr;

	Q3HBoxLayout* channelLayout;

public slots:
  /*$PUBLIC_SLOTS$*/
    virtual void show ();
    virtual void showPreferences();
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

    virtual void showAbout();

protected slots:
  /*$PROTECTED_SLOTS$*/
	virtual void showPlaylistManager( bool state );

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


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
#ifndef MIXERGUIPLAYER_H
#define MIXERGUIPLAYER_H

#include <mixerchannelgui.h>
#include "soundplayer.h"
#include "glowbutton.h"
#include "blinkbutton.h"
#include "timedisplay.h"
#include "title.h"


/**
@author Jan Boysen
*/
class mixerGuiPlayer : public mixerChannelGUI
{
  Q_OBJECT

public:
    mixerGuiPlayer( int chID, soundPlayer* soundplayer, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~mixerGuiPlayer();

	// Main Player
	playerChannelFile* player;

	virtual void languageChange();
	virtual QString getType();

protected:
	timeDisplay* tDisplay;
	blinkButton* playButton;
	glowButton* stopButton;
	blinkButton* pauseButton;
	blinkButton* cueButton;
	blinkButton* openButton;
	glowButton* loopButton;
	vuMeter* levelMeterLeft;
	vuMeter* levelMeterRight;

	// Playlist Selection 1= Global; 2= Local;
	unsigned short playListSelection;

protected slots:
	virtual void fileOpen();
	virtual void buttonBlinker();
	virtual void showPrefs();
	virtual void setMeta( metaTag );
	virtual void cued( metaTag );


public slots:
	virtual void buttonPressed( int hwChannel, int button );
	virtual void cueNewTrack();

signals:
	void global_getNextTrack( QString );
	void sixSecondsLeft();
	void onCue( metaTag, QString );

};

#endif

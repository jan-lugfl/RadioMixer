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
#ifndef MIXERGUIJACKPORT_H
#define MIXERGUIJACKPORT_H

#include <mixerchannelgui.h>
#include "glowbutton.h"
#include "soundplayerjack.h"
#include "playerchanneljackport.h"

/**
@author Jan Boysen
*/
class mixerGuiJackport : public mixerChannelGUI
{
Q_OBJECT
public:
    mixerGuiJackport( soundPlayerJack* jackPlayer, int chID, QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~mixerGuiJackport();

    virtual void languageChange();
    virtual QString getType();

protected:
	vuMeter* levelMeterLeft;
	vuMeter* levelMeterRight;
	glowButton*	muteBut;
	playerChannelJackport*	jackMixer;

private:
	bool mute;

public slots:
	virtual void toggleMute();
	virtual void changeName( QString newName );
	virtual void buttonPressed( int hwChannel, int button );

protected slots:
	virtual void buttonBlinker();
	virtual void showPrefs();

};

#endif

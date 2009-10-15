/* $Id$ */
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
#ifndef PLAYERCHANNELJACKPORT_H
#define PLAYERCHANNELJACKPORT_H

#include <mixerchannel.h>
#include "soundplayerjack.h"

/**
@author Jan Boysen
*/
class mixerChannel_jack : public mixerChannel
{
Q_OBJECT
public:
    mixerChannel_jack( soundPlayerJack* jackPlayer, QString chName="", const char *name = 0 );
    ~mixerChannel_jack();

    virtual QString getType() { return QString("JACK"); }

// Functions for the LevelMeter
   virtual const float getLevelMeterLeft();
   virtual const float getLevelMeterRight();

protected:
	jackPort*			jackIn[2];
	jackPort*			jackOut[2];
	jack_nframes_t	frames;
	soundPlayerJack*	player;

private:
	float levelMeterLeft;
	float levelMeterRight;

public slots:
	virtual void setVolume( int volume );
	virtual void mute();
	virtual void unMute();
	virtual void connectPort();
	virtual void disconnectPort();

protected slots:
	virtual void processJackLeft( jack_nframes_t );
	virtual void processJackRight( jack_nframes_t );
};

#endif

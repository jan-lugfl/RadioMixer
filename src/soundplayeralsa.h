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
#ifndef SOUNDPLAYERALSA_H
#define SOUNDPLAYERALSA_H

#include "soundplayer.h"
#include <qthread.h>
#include <qstringlist.h>
#include <alsa/asoundlib.h>

class soundPlayerALSA;

/**
@author Jan Boysen
*/
class alsaPlayerThread : public QThread
{
public:
	alsaPlayerThread( soundPlayerALSA* alsaPlayer );
	~alsaPlayerThread();

	virtual void run();
	virtual void open( QString device );

protected:
	// ALSA Player Stuff
	soundPlayerALSA*	alsaPlayer;
 	snd_pcm_t* alsa_handle;
 	static const snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
 	snd_pcm_hw_params_t *hwparams;
 	QString snd_device;

 	static const snd_pcm_uframes_t periodsize = 8192;
 	unsigned int exact_smpl_rate;
 	static const int periods = 2;
 	int dir;


friend class soundPlayerAlsa;
};

/**
@author Jan Boysen
*/
class soundPlayerALSA : public soundPlayer
{
public:
	soundPlayerALSA( );
	~soundPlayerALSA();

	virtual void open( QString device );
	virtual void interleave();
	virtual void close();

	// static Funktion that return a List of all available ALSA devices
	static QStringList getDevices();

protected:
	alsaPlayerThread*		alsaPlayer;

private:
	short* outputBuffer;

friend class alsaPlayerThread;
};

#endif

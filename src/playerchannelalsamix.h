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
#ifndef PLAYERCHANNELALSAMIX_H
#define PLAYERCHANNELALSAMIX_H

#include <playerchannelstd.h>
#include <alsa/asoundlib.h>
//#include <alsa/mixer.h>

/**
@author Jan Boysen
*/
class playerChannelAlsaMix : public playerChannelStd
{
	Q_OBJECT

public:
    playerChannelAlsaMix();
    ~playerChannelAlsaMix();

private:
	snd_mixer_t*	alsaMixer;
        snd_mixer_elem_t*	alsaChannel;
	snd_mixer_selem_id_t *chId;

public slots:
	virtual void setVolume( int volume );
	virtual void mute();
	virtual void unMute();
	
};

#endif

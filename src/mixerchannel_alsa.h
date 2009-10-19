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
#ifndef MIXERCHANNEL_ALSA_H
#define MIXERCHANNEL_ALSA_H

#include <mixerchannel.h>
#include <alsa/asoundlib.h>

/**
@author Jan Boysen
*/
class mixerChannel_ALSA : public mixerChannel
{
	Q_OBJECT

public:
    mixerChannel_ALSA();
    ~mixerChannel_ALSA();

    virtual AudioDataType getAudioDataType() { return mixerChannel::AudioDataNone; }
    virtual QString getType() { return QString("ALSAMIX"); }

private:
	snd_mixer_t*	alsaMixer;
        snd_mixer_elem_t*	alsaChannel;
	snd_mixer_selem_id_t *chId;
	bool muted;

	void startAlsa();
	void stopAlsa();

public slots:
	virtual void updateSettings( mixerChannel::settingsType );
	virtual void setVolume( int volume );
	virtual void toggleMute();
	virtual void mute();
	virtual void unMute();

signals:
	void muteChanged( bool );
};

#endif

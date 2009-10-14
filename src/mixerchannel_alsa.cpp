/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2009 by Jan Boysen                                *
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
#include "mixerchannel_alsa.h"

mixerChannel_ALSA::mixerChannel_ALSA()
 : mixerChannel()
{
#ifdef HAVE_ALSA
    snd_mixer_selem_id_alloca(&chId);

	if( snd_mixer_open( &alsaMixer, 0) )
		qWarning( tr("Could not open AlsaMixer !!!") );

	snd_mixer_attach(alsaMixer, "hw:1");

 	snd_mixer_selem_register(alsaMixer, NULL, NULL);
        snd_mixer_load(alsaMixer);             //load up the mixer
	
        snd_mixer_selem_id_set_name(chId, "Mic");
	 
        alsaChannel = snd_mixer_find_selem(alsaMixer, chId);

	//mute Channel by default
	snd_mixer_selem_set_playback_switch_all( alsaChannel, 0); 
#endif
}

mixerChannel_ALSA::~mixerChannel_ALSA()
{
#ifdef HAVE_ALSA
	snd_mixer_close(alsaMixer);
#endif
}

void mixerChannel_ALSA::setVolume( int volume )
{
#ifdef HAVE_ALSA
	snd_mixer_selem_set_playback_volume_all(alsaChannel, int(volume/3) );
#endif
}

void mixerChannel_ALSA::mute( )
{
#ifdef HAVE_ALSA
	snd_mixer_selem_set_playback_switch_all( alsaChannel, 1);
#endif
}

void mixerChannel_ALSA::unMute( )
{
#ifdef HAVE_ALSA
	snd_mixer_selem_set_playback_switch_all( alsaChannel, 0);
#endif
}



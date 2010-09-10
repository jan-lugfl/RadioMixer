/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
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

QString const mixerChannel_ALSA::Type = QString("ALSAMIX");

mixerChannel_ALSA::mixerChannel_ALSA( const char *name, QUuid uuid )
 : mixerChannel(name, uuid), muted( false )
{
    startAlsa();
    registerChannel();
}

mixerChannel_ALSA::~mixerChannel_ALSA()
{
    stopAlsa();
}

// opens the ALSA API and attaches to the device defined in the settings
void mixerChannel_ALSA::startAlsa()
{
#ifdef HAVE_ALSA
    snd_mixer_selem_id_alloca(&chId);
    if( snd_mixer_open( &alsaMixer, 0) )
	qWarning( tr("Could not open AlsaMixer !!!") );

    char device[16];
    sprintf(device, "hw:%i", settings["alsa_card"].toInt() );
    snd_mixer_attach(alsaMixer, device);
    snd_mixer_selem_register(alsaMixer, NULL, NULL);
    snd_mixer_load(alsaMixer); //load up the mixer

    char* channel;
    if( settings["alsa_channel"].isNull() )
	channel = "Mic";
    else
	channel = (char*)settings["alsa_channel"].toString().toStdString().c_str();

    snd_mixer_selem_id_set_name(chId, channel );
    alsaChannel = snd_mixer_find_selem(alsaMixer, chId);
    // unmute the channel
    if(alsaChannel)
	snd_mixer_selem_set_playback_switch_all( alsaChannel, 1);
#endif
}

// closes the ALSA API
void mixerChannel_ALSA::stopAlsa()
{
#ifdef HAVE_ALSA
	snd_mixer_close(alsaMixer);
#endif
}

void mixerChannel_ALSA::setVolume( int volume )
{
#ifdef HAVE_ALSA
    if(alsaChannel)
	snd_mixer_selem_set_playback_volume_all(alsaChannel, int(volume/3) );
#endif
}

void mixerChannel_ALSA::mute( )
{
#ifdef HAVE_ALSA
    if(alsaChannel)
    {
	snd_mixer_selem_set_playback_switch_all( alsaChannel, 0);
	muted = true;
	emit( muteChanged( true ) );
    }
#endif
}

void mixerChannel_ALSA::unMute( )
{
#ifdef HAVE_ALSA
    if(alsaChannel)
    {
	snd_mixer_selem_set_playback_switch_all( alsaChannel, 1);
	muted = false;
	emit( muteChanged( false ) );
    }
#endif
}

void mixerChannel_ALSA::toggleMute()
{
    if(muted)
	unMute();
    else
	mute();
}

void mixerChannel_ALSA::updateSettings( mixerChannel::settingsType settings )
{
    // save old settings first...
    settingsType oldSettings = this->settings;
    // load new settings...
    mixerChannel::updateSettings( settings );
    // check if something has changed... if so reinit ALSA device
    if( oldSettings["alsa_card"] != this->settings["alsa_card"] || oldSettings["alsa_channel"] != this->settings["alsa_channel"] )
    {
	stopAlsa();
	startAlsa();
    }
}

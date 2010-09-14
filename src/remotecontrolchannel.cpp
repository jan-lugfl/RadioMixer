/* $Id:$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2010 by Jan Boysen                                      *
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
#include "remotecontrolchannel.h"
#include "mixerchannel_fileplayer.h"

remoteControlChannel::remoteControlChannel(int channel_id, QObject *parent) :
    channel_id(channel_id), QObject(parent)
{
    name = tr("unnamed");
}

// This function processes the events and sends QT signals depending on the event...
void remoteControlChannel::process_event( RemoteControlerEvent event, QString value )
{
    switch(event)
    {
        case event_volumeFader:
                    emit volumeChanged( value.toInt() );
                    break;
        case event_balanceFader:
                    emit balanceChanged( value.toInt() );
                    break;
        case event_playButton:
                    emit play( );
                    break;
        case event_stopButton:
                    emit stop( );
                    break;
        case event_pauseButton:
                    emit pause( );
                    break;
        case event_queueButton:
                    emit queue( );
                    break;
        case event_repeatButton:
                    emit repeat( );
                    break;
        case event_openButton:
                    emit open( );
                    break;
        default:
                qWarning("unknown event triggered...");
    };
}

void remoteControlChannel::changeVolume( int value )
{
    emit stateChanged( channel_id, event_volumeFader, QString::number(value));
}

void remoteControlChannel::changeBalance( int value )
{
    emit stateChanged( channel_id, event_balanceFader, QString::number(value));
}

void remoteControlChannel::changeState( int state )
{
    //turn all buttons off
    emit stateChanged( channel_id, event_playButton, QString::number(0));
    emit stateChanged( channel_id, event_stopButton, QString::number(0));
    emit stateChanged( channel_id, event_pauseButton, QString::number(0));
    emit stateChanged( channel_id, event_queueButton, QString::number(0));

    switch(state)
    {
    case mixerChannel_filePlayer::Playing:
        emit stateChanged( channel_id, event_playButton, QString::number(127));
        break;
    case mixerChannel_filePlayer::Stopped:
        emit stateChanged( channel_id, event_stopButton, QString::number(127));
        break;
    case mixerChannel_filePlayer::Paused:
        emit stateChanged( channel_id, event_pauseButton, QString::number(127));
        break;
    case mixerChannel_filePlayer::Cued:
        emit stateChanged( channel_id, event_queueButton, QString::number(127));
        break;
    }
}

void remoteControlChannel::repeat( bool state )
{

}

// starts to control a given channel...
void remoteControlChannel::associateToChannel( mixerChannel* channel )
{
//    connect( channel, SIGNAL(nameChanged(QString)), this, SLOT(changeName(QString)) );
    connect( this, SIGNAL(volumeChanged( int )), channel, SLOT( setVolume( int ) ) );
    connect( channel, SIGNAL(volumeChanged( int )), this, SLOT( changeVolume( int ) ) );
    connect( this, SIGNAL(balanceChanged( int )), channel, SLOT( setTreble( int ) ) );
    connect( channel, SIGNAL(trebleChanged( int )), this, SLOT( changeBalance( int ) ) );
    connect( channel, SIGNAL(stateChanged(int)), this, SLOT(changeState(int)));
    connect( this, SIGNAL(play() ), channel, SLOT( play() ) );
    connect( this, SIGNAL(pause() ), channel, SLOT( pause() ) );
}

void remoteControlChannel::setName(QString newName)
{
    name = newName;
}

QString remoteControlChannel::getName()
{
    return name;
}

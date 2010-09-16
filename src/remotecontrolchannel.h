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
#ifndef REMOTECONTROLCHANNEL_H
#define REMOTECONTROLCHANNEL_H

#include "mixerchannel.h"
#include <QObject>
#include <QUuid>

class remoteControlChannel : public QObject
{
Q_OBJECT
public:
    explicit remoteControlChannel( QObject *parent = 0, QUuid uuid = QUuid::createUuid() );

    enum RemoteControlerEvent
    {
        event_volumeFader,
        event_balanceFader,
        event_playButton,
        event_stopButton,
        event_pauseButton,
        event_queueButton,
        event_repeatButton,
        event_openButton,
        event_muteButton
    };

    void process_event( RemoteControlerEvent event, QString value );
    void associateToChannel( mixerChannel* channel );

    QString getName();
    void setName( QString newName );

    QUuid getUuid();
    QStringList getAttachedTo();

protected:
    QString name;

    // contains the local channel ID the controller uses to identify the channel upon signals from the remote control
    QUuid uuid;

    // this holds the information to which mixer I am attached to, so we can restore this from config...
    QStringList attached_to;

signals:
    void volumeChanged( int );
    void balanceChanged( int );
    void play();
    void stop();
    void pause();
    void queue();
    void repeat();
    void open();
    void mute();
    void stateChanged( QUuid uuid, remoteControlChannel::RemoteControlerEvent, QString );

public slots:
    virtual void changeVolume( int value );
    virtual void changeBalance( int value );
    virtual void changeState( int state );
    virtual void repeat( bool state );
};

#endif // REMOTECONTROLCHANNEL_H

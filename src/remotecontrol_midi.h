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
#ifndef REMOTECONTROL_MIDI_H
#define REMOTECONTROL_MIDI_H

#include "remotecontrol.h"
#include "remotecontrolchannel.h"
#include "jack_midicontrol.h"

#include <QThread>
#include <QTimer>
#include <QMap>

// forward declaration for queue thread class
class remoteControl_MIDIQueueThread;

class remoteControl_MIDI : public remoteControl
{
Q_OBJECT
public:
    explicit remoteControl_MIDI(QObject *parent = 0, QString name = QString('Remote Control'), bool biderectional = true);

private:
    void queueMIDIMessage( int par, int val );

    jack_MIDIControl* controller;
    remoteControl_MIDIQueueThread* queue_thread;
    enum channelStates
    {
        Playing,
        Paused,
        Stopped,
        Cued
    };
    QMap<int, channelStates> channel_states;
    QTimer* refreshTimer; // used to refresh the midi device
    bool blink_state; // used to let buttons blink...

signals:

public slots:
       virtual void setControllerState( int channel_id, remoteControlChannel::RemoteControlerEvent event, QString value );

private slots:
       virtual void refreshTimerTimeout();
};

class remoteControl_MIDIQueueThread : public QThread
{
Q_OBJECT
public:
    jack_MIDIControl* controller;
    remoteControl_MIDI* remoteControl;
protected:
    // worker function...
    void run();
};

#endif // REMOTECONTROL_MIDI_H

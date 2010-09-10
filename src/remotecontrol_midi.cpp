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
#include "remotecontrol_midi.h"
#include "jack.h"

remoteControl_MIDI::remoteControl_MIDI(QObject *parent, QString name, bool bidirectional) :
    remoteControl(parent, name, bidirectional), blink_state( false )
{
    refreshTimer = new QTimer();
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshTimerTimeout()) );
    refreshTimer->start(500);

    // register new controller...
    controller = Jack::newController( name, bidirectional );

    // TODO: load from settings..
    remoteControlChannel* temp ;
    for(int i=1;i<9;i++)
    {
        temp = new remoteControlChannel( i, this);
        if(bidirectional)
            connect( temp, SIGNAL(stateChanged(int,remoteControlChannel::RemoteControlerEvent,QString)), this, SLOT(setControllerState(int,remoteControlChannel::RemoteControlerEvent,QString)) );
        channels.append( temp );
    }

    // lets fire up the queue thread...
    queue_thread = new remoteControl_MIDIQueueThread();
    queue_thread->controller = controller;
    queue_thread->remoteControl = this;
    queue_thread->start();
}

void remoteControl_MIDI::setControllerState( int channel_id, remoteControlChannel::RemoteControlerEvent event, QString value )
{
    switch( event )
    {
        case remoteControlChannel::event_balanceFader:
            queueMIDIMessage( channel_id, ((value.toInt()+100)/2.)*1.27 );
            break;
        case remoteControlChannel::event_volumeFader:
            queueMIDIMessage( channel_id+80, value.toInt()*1.27 );
            break;
        case remoteControlChannel::event_playButton:
            channel_states[channel_id] = Playing;
            // turn play button on
            queueMIDIMessage( channel_id+64, 127 );
            queueMIDIMessage( channel_id+72, 0 );
            break;
        case remoteControlChannel::event_pauseButton:
            channel_states[channel_id] = Paused;
            break;
        case remoteControlChannel::event_queueButton:
            channel_states[channel_id] = Cued;
            break;
        case remoteControlChannel::event_stopButton:
            channel_states[channel_id] = Stopped;
            // turn all buttons off
            queueMIDIMessage( channel_id+64, 0 );
            queueMIDIMessage( channel_id+72, 0 );
            break;
    }
}

void remoteControl_MIDI::refreshTimerTimeout()
{
    int channel = 0;
    foreach( channel, channel_states.keys() )
    {
        switch( channel_states[channel] )
        {
            case Playing: // turn play on, pause off
                queueMIDIMessage( channel+64, 127 );
                queueMIDIMessage( channel+72, 0 );
                break;
            case Paused: // turn pause on, play off
                queueMIDIMessage( channel+64, 0 );
                queueMIDIMessage( channel+72, 127 );
                break;
            case Cued: // turn pause blink, play off
                queueMIDIMessage( channel+64, 0 );
                if(blink_state)
                    queueMIDIMessage( channel+72, 127 );
                else
                    queueMIDIMessage( channel+72, 0 );
                break;
        }
    }
    blink_state = !blink_state;
}

void remoteControl_MIDI::queueMIDIMessage( int par, int val )
{
    // generate MIDI message
    jack_MIDIControl::midi_message msg;
    msg.command = 0xffffffb0; // bank 1...
    msg.par = par;
    msg.value = val;
    controller->enqueue_message_out( msg );
}

// remoteControl_MIDIQueueThread implementations below...
#include <stdio.h>

void remoteControl_MIDIQueueThread::run()
{
    while(1)
    {
        while(!controller->isInQueueEmpty())
        {
            jack_MIDIControl::midi_message msg = controller->dequeue_message_in();
            remoteControlChannel::RemoteControlerEvent evt;

            if(msg.par <= 16) // Parameter 1-16 are balance rotaries
            {
                evt = remoteControlChannel::event_balanceFader;
                msg.value = (((float)msg.value/127)*200)-100;
            }else if( msg.par >= 65 && msg.par <= 72 && msg.value == 0 ) // Parameter 65 - 72 are Play buttons ( only take release events not press events )
            {
                evt = remoteControlChannel::event_playButton;
                msg.par -= 64;
                msg.value = 127;
            }else if( msg.par >= 73 && msg.par <= 80 && msg.value == 0 ) // Parameter 73 - 80 are Pause buttons ( only take release event not the press events )
            {
                evt = remoteControlChannel::event_pauseButton;
                msg.par -= 72;
                msg.value = 127;
            }else if( msg.par >= 81 && msg.par <= 96 ) // Parameter 81 - 96 are volume Sliders
            {
                evt = remoteControlChannel::event_volumeFader;
                msg.par -= 80;
                msg.value = ((float)msg.value/127)*100;
            }

            remoteControlChannel* channel;
            foreach( channel, remoteControl->channels )
            {
                if( channel->channel_id == msg.par)
                    channel->process_event( evt, QString::number(msg.value) );
            }
        }
        msleep(50);
    }
}

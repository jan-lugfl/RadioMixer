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

    // lets fire up the queue thread...
    queue_thread = new remoteControl_MIDIQueueThread();
    queue_thread->controller = controller;
    queue_thread->remoteControl = this;
    queue_thread->start();
}

void remoteControl_MIDI::setControllerState( QUuid uuid, remoteControlChannel::RemoteControlerEvent event, QString value )
{
    switch( event )
    {
        case remoteControlChannel::event_balanceFader:
            queueMIDIMessage( controller_config[uuid][event].toInt(), ((value.toInt()+100)/2.)*1.27 );
            break;
        case remoteControlChannel::event_volumeFader:
            queueMIDIMessage( controller_config[uuid][event].toInt(), value.toInt()*1.27 );
            break;
        case remoteControlChannel::event_playButton:
            channel_states[uuid] = Playing;
            // turn play button on
            queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_playButton].toInt(), 127 );
            queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_pauseButton].toInt(), 0 );
            break;
        case remoteControlChannel::event_pauseButton:
            channel_states[uuid] = Paused;
            break;
        case remoteControlChannel::event_queueButton:
            channel_states[uuid] = Cued;
            break;
        case remoteControlChannel::event_stopButton:
            channel_states[uuid] = Stopped;
            // turn all buttons off
            queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_playButton].toInt(), 0 );
            queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_pauseButton].toInt(), 0 );
            break;
        default:
            break;
    }
}

void remoteControl_MIDI::refreshTimerTimeout()
{
    foreach( QUuid uuid, channel_states.keys() )
    {
        switch( channel_states[uuid] )
        {
            case Playing: // turn play on, pause off
                queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_playButton].toInt(), 127 );
                queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_pauseButton].toInt(), 0 );
                break;
            case Paused: // turn pause on, play off
                queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_playButton].toInt(), 0 );
                queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_pauseButton].toInt(), 127 );
                break;
            case Cued: // turn pause blink, play off
                queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_playButton].toInt(), 0 );
                if(blink_state)
                    queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_pauseButton].toInt(), 127 );
                else
                    queueMIDIMessage( controller_config[uuid][remoteControlChannel::event_pauseButton].toInt(), 0 );
                break;
            default:
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

remoteControlChannel* remoteControl_MIDI::createChannel(QUuid uuid)
{
    remoteControlChannel* temp = new remoteControlChannel( this, uuid );
    if(bidirectional)
        connect( temp, SIGNAL(stateChanged(QUuid,remoteControlChannel::RemoteControlerEvent,QString)), this, SLOT(setControllerState(QUuid,remoteControlChannel::RemoteControlerEvent,QString)) );
    channels.append( temp );
    return temp;
}


// remoteControl_MIDIQueueThread implementations below...
void remoteControl_MIDIQueueThread::run()
{
    while(1)
    {
        while(!controller->isInQueueEmpty())
        {
            jack_MIDIControl::midi_message msg = controller->dequeue_message_in();
            foreach( remoteControlChannel* channel, remoteControl->channels )
            {
                remoteControl_MIDI::channelConfig conf = remoteControl->getChannelConfig( channel->getUuid() );
                int value;
                foreach( remoteControlChannel::RemoteControlerEvent event, conf.keys() )
                    if( conf[event] == msg.par )
                        switch(event)
                        {
                            case remoteControlChannel::event_balanceFader:
                                value = (((float)msg.value/127)*200)-100;
                                channel->process_event( event, QString::number( value ) );
                                break;
                            case remoteControlChannel::event_volumeFader:
                                value = ((float)msg.value/127)*100;
                                channel->process_event( event, QString::number( value ) );
                                break;
                            default:
                                channel->process_event( event, QString::number(msg.value) );
                        }
            }
        }
        msleep(50);
    }
}

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
#include "jack_midicontrol.h"

#include <jack/ringbuffer.h>
#include <jack/midiport.h>
#include <stdio.h>
#include <QObject>

jack_MIDIControl::jack_MIDIControl() :
        is_bidirectional(false), input(NULL), output(NULL)
{
    // Init the Mutexes
    lock_inqueue = new QMutex();
    lock_outqueue = new QMutex();
}

jack_MIDIControl::~jack_MIDIControl()
{
    // free the Mutexes
    free(lock_inqueue);
    free(lock_outqueue);
}

void jack_MIDIControl::process(int frames)
{
    // return if channels are not available
    if(!input | !output)
        return;

    // get MIDI input data...
    void* port_buffer = NULL;
    int read, events;
    jack_midi_event_t event;

    port_buffer = jack_port_get_buffer(input, frames);
    if(port_buffer == NULL)
    {
        qWarning("cannot receive data...");
    }
    events = jack_midi_get_event_count(port_buffer);
    for(int i = 0;i<events;i++)
    {
        read = jack_midi_event_get(&event, port_buffer, i);
        char buf[event.size];
        memcpy(buf, event.buffer, event.size);
        printf( "%X %i %i\n",buf[0], buf[1], buf[2]);
        // Queue Midi message
        midi_message message;
        message.command = buf[0];
        message.par = buf[1];
        message.value = buf[2];
        lock_inqueue->lock();
        message_queue_in.enqueue( message );
        lock_inqueue->unlock();
    }

    // Process output if we have something in the queue...
    if(is_bidirectional)
    {
        port_buffer = jack_port_get_buffer(output, frames);
        jack_midi_clear_buffer(port_buffer);

        if(!message_queue_out.empty())
        {
            _jack_midi_message oMsg;
            midi_message message;
            while(!message_queue_out.empty() )
            {
                lock_outqueue->lock();
                message = message_queue_out.dequeue();
                lock_outqueue->unlock();
                oMsg.data[0] = message.command;
                oMsg.data[1] = message.par;
                oMsg.data[2] = message.value;
                jack_midi_event_write( port_buffer, 1, oMsg.data, 3);
            }
        }
    }
}

void jack_MIDIControl::enqueue_message_in( jack_MIDIControl::midi_message msg )
{
    lock_inqueue->lock();
    message_queue_in.enqueue( msg );
    lock_inqueue->unlock();
}

bool jack_MIDIControl::isInQueueEmpty( )
{
    lock_inqueue->lock();
    bool empty = message_queue_in.isEmpty( );
    lock_inqueue->unlock();
    return empty;
}

jack_MIDIControl::midi_message jack_MIDIControl::dequeue_message_in()
{
    lock_inqueue->lock();
    midi_message msg = message_queue_in.dequeue( );
    lock_inqueue->unlock();
    return msg;
}

bool jack_MIDIControl::isOutQueueEmpty( )
{
    lock_outqueue->lock();
    bool empty = message_queue_out.isEmpty( );
    lock_outqueue->unlock();
    return empty;
}

void jack_MIDIControl::enqueue_message_out( jack_MIDIControl::midi_message msg )
{
    lock_outqueue->lock();
    message_queue_out.enqueue( msg );
    lock_outqueue->unlock();
}

jack_MIDIControl::midi_message jack_MIDIControl::dequeue_message_out( )
{
    lock_outqueue->lock();
    midi_message msg = message_queue_out.dequeue( );
    lock_outqueue->unlock();
    return msg;
}

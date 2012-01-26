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
#ifndef JACK_MIDICONTROL_H
#define JACK_MIDICONTROL_H

#include <jack/jack.h>
#include <QQueue>
#include <QMutex>

class jack_MIDIControl
{
public:
    jack_MIDIControl();
    ~jack_MIDIControl();

    void process( int frames );

    bool is_bidirectional;
    jack_port_t* input;
    jack_port_t* output;

    struct _jack_midi_message
    {
        jack_nframes_t time;
        int len;
        unsigned char  data[3];
    };

    struct midi_message
    {
        int command;
        int par;
        int value;
    };

    bool isInQueueEmpty( );
    void enqueue_message_in( midi_message msg );
    midi_message dequeue_message_in();

    bool isOutQueueEmpty( );
    void enqueue_message_out( midi_message msg );
    midi_message dequeue_message_out( );

private:
    QMutex* lock_inqueue;
    QQueue<midi_message> message_queue_in;
    QMutex* lock_outqueue;
    QQueue<midi_message> message_queue_out;
};

#endif // JACK_MIDICONTROL_H

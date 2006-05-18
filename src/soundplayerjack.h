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
#ifndef SOUNDPLAYERJACK_H
#define SOUNDPLAYERJACK_H

#include "soundplayer.h"
#include <jack/jack.h>
#include <qvaluevector.h>

/**
@author Jan Boysen
*/

class jackPort;

class soundPlayerJack : public soundPlayer
{
 Q_OBJECT
public:
    soundPlayerJack( );
    ~soundPlayerJack();

    virtual void open( QString device );
    virtual void close();

    virtual jackPort* registerJackPort( QString name, unsigned long flags);
    virtual void unregisterJackPort( jackPort* port);

protected:
	// Jack Callbacks !
	static  int process(jack_nframes_t frames, void *arg);
	static void jackShutdown( void* arg);

	// Jack Stuff below
	jack_client_t*				jack;
	QValueVector<jackPort*>	jackPorts;
	jack_port_t*				outputPorts[2];
	bool						buffResetted;

	unsigned int bufSize;

signals:
	void onConnect();
	void onDisconnect();

};


class jackPort : public QObject
{
  Q_OBJECT

public:
    jackPort( jack_client_t* jackClient,  jack_port_t* jPort );
    ~jackPort();

    virtual void process( jack_nframes_t frames );
    jack_client_t*	jack;
    jack_port_t*		jackport;

signals:
	void processData( jack_nframes_t );

};

#endif

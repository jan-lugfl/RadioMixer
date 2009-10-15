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
#ifndef MIXERCHANNEL_H
#define MIXERCHANNEL_H

#include <QObject>
#include <QMessageBox>
#include <QThread>
#include <QHash>
#include <QVariant>

#include "metatag.h"
#include "ringbuffer.h"

/**
@author Jan Boysen
*/
class mixerChannel : public QObject
{
Q_OBJECT
public:
    mixerChannel( const char *name = 0 );
    ~mixerChannel();

    typedef QHash<QString, QVariant> settingsType;

    const bool isPlaying();
    const bool isStopped();
    const bool isPaused();
    const bool isCued();
    
    virtual QString getType() = 0;
    QString getName();
    void setName(QString newName);
    
    virtual int getLevelLeft();
    virtual int getLevelRight();
    virtual void checkBuffer();

   virtual const unsigned int getSmplRate();

// Wrapper functions for the ringbuffer
   virtual bool canGetData( unsigned int size );
   virtual void getDataLeft( float* dataOut, unsigned int size );
   virtual void getDataRight( float* dataOut, unsigned int size );
   virtual unsigned int getBuffFill( );

   bool providesAudioData();

protected:
	settingsType settings; // stores the channel dependant settings...
        bool provides_audiodata;
	float volume_left;
	float volume_right;
	float volume;
	unsigned int smplRate;
	unsigned int channels;
	QString name;

	metaTag*	meta;

	// States for the Channels
	// 0 = Channel is stopped
	// 1 = Channel is playing somthing
	// 2 = Channel is paused
	// 3 = Channel is cued
	int state;
        void setState(int newState );

	soundRingBuffer* soundBuffers;

private:
        QThread* thread;
	int sendVuMeterChanged_left;
	int sendVuMeterChanged_right;

protected slots:
	virtual void refresh();
	
public slots:
	virtual void setVolume( int newValue );
        virtual void setTreble( int newValue );
        virtual void play( );
	virtual void stop( );
	virtual void pause( );
	virtual void cue( );
	virtual void connectPort();
	virtual void disconnectPort();
	virtual void updateSettings( mixerChannel::settingsType );
	
signals:
        void volumeChanged( int );
        void trebleChanged( int );
        void vuMeterChanged_left( float );
        void vuMeterChanged_right( float );
	void newMeta( metaTag );
	void nameChanged( QString );
        void stateChanged(int);
	void refreshed();
	void settingsChanged( mixerChannel::settingsType );
};

#endif

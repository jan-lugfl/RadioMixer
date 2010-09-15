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
#ifndef MIXERCHANNEL_H
#define MIXERCHANNEL_H

#include <QObject>
#include <QThread>
#include <QHash>
#include <QVariant>
#include <QMessageBox>
#include <QUuid>

#include "metatag.h"
#include "ringbuffer.h"

/**
@author Jan Boysen
*/
class mixerChannel : public QObject
{
Q_OBJECT
public:
    explicit mixerChannel( QString name = QString(), QUuid uuid = QUuid() );
    virtual ~mixerChannel();

    static QString const Type;
    typedef QHash<QString, QVariant> settingsType;

    enum AudioDataType
    {
        AudioDataIn,
        AudioDataOut,
        AudioDataNone
    };

    virtual QString getType() = 0;
    virtual AudioDataType getAudioDataType() = 0;
    QString getName();
    virtual void setName(QString newName);
    QUuid getUuid();
    
    virtual void checkBuffer();

    virtual float getLevelLeft();
    virtual float getLevelRight();
    virtual const unsigned int getSmplRate();

    // Wrapper functions for the ringbuffer
    virtual bool canGetData( unsigned int size );
    virtual void getDataLeft( float* dataOut, unsigned int size );
    virtual void getDataRight( float* dataOut, unsigned int size );
    virtual bool canAddData( unsigned int size );
    virtual void addDataLeft( float* data, unsigned int size );
    virtual void addDataRight( float* data, unsigned int size );
    virtual unsigned int getBuffFill( );

    mixerChannel::settingsType getSettings();

protected:
        settingsType settings; // stores the channel settings...
	float volume_left;
	float volume_right;
	float volume;
	unsigned int smplRate;
	unsigned int channels;
        QString type;
        QUuid uuid;

	metaTag*	meta;
	soundRingBuffer* soundBuffers;

        void registerChannel();
private:
        QThread* thread;
	int sendVuMeterChanged_left;
	int sendVuMeterChanged_right;

protected slots:
	virtual void refresh();
	
public slots:
	virtual void setVolume( int newValue );
        virtual void setTreble( int newValue );
        virtual void updateSettings( mixerChannel::settingsType settings );
	
signals:
        void volumeChanged( int );
        void trebleChanged( int );
        void vuMeterChanged_left( float );
        void vuMeterChanged_right( float );
	void newMeta( metaTag );
        void stateChanged(int);
	void refreshed();
	void settingsChanged( mixerChannel::settingsType );
};

#endif

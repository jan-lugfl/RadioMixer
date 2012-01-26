/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2007 by Jan Boysen                                *
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
#ifndef MIPUCOM_H
#define MIPUCOM_H

#include <qobject.h>
#include <q3socket.h>
#include <qstring.h>
#include <qtimer.h>

/**
@author Jan Boysen
*/
class mipuCom : public QObject
{
Q_OBJECT
public:
    enum Error {
        ErrConnectionRefused,
        ErrHostNotFound,
        ErrSocketRead,
	ErrPeerProtVersionOld
    };
    
    mipuCom(QObject *parent = 0, const char *name = 0);
    ~mipuCom();
    
    const bool isConnected();
    const int getSlider(int slider);

private:
	Q3Socket* Socket;
	QTimer* keepAliveTimer;
	static const int protokollVersion = 1;
	static const int minProtVersion = 1;
	
	bool conn;
	int* channelValues;
	
	int peerProtVersion;
	QString peerFWVersion;
	int peerChannels;
	int peerTastperChannel;
	QString peerDevName;
	
public slots:
	virtual void open(QString host, int port);
	virtual void close();
	virtual void getAllChannels();

private slots:
	virtual void onConnected();
	virtual void onConnectionClosed();
	virtual void dataRead();
	virtual void keepAlive();
	
signals:
	void connected();
	void connectionClosed();
	void error( int );
	void channelsRefreshed();
	void butPres( int, int );
	void sliderMove( int, int );
};

#endif

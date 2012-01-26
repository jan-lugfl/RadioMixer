/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005 - 2007 by Jan Boysen                                *
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
#include "mipucom.h"
//Added by qt3to4:
#include <Q3CString>

mipuCom::mipuCom(QObject *parent, const char *name)
 : QObject(parent, name), conn(FALSE)
{
	Socket = new Q3Socket(this);
	connect( Socket, SIGNAL(connected()), this, SLOT(onConnected()) );
	connect( Socket, SIGNAL(readyRead()), this, SLOT(dataRead()) );
	keepAliveTimer = new QTimer;
	connect( keepAliveTimer, SIGNAL( timeout() ), this, SLOT(keepAlive() ) );
}

mipuCom::~mipuCom()
{
}

void mipuCom::open( QString host, int port )
{
	Socket->connectToHost( host, port);
}

void mipuCom::close( )
{
}

void mipuCom::onConnected( )
{
	QString cmd = "*CON"+QString::number( protokollVersion );
	Socket->writeBlock( cmd.latin1(), cmd.length() );
}

void mipuCom::onConnectionClosed( )
{
	keepAliveTimer->stop();
}

void mipuCom::dataRead( )
{
    // read the new data from the socket
    Q3CString data;
    data.resize( Socket->bytesAvailable() + 1 );
    Socket->readBlock( data.data(), Socket->bytesAvailable() );

    QString command = data.left(4);
    data.remove(0,4);

    if( command == "*CRP")
    {
	peerProtVersion = (data.left(3)).toInt();
	if( peerProtVersion <= minProtVersion )
	{
		data.remove(0,3);
		peerFWVersion = data.left(3);
		data.remove(0,3);
		peerChannels = data[0];
		channelValues = new int[peerChannels];
		data.remove(0,1);
		peerTastperChannel = data[0];
		data.remove(0,1);
		peerDevName = data;
		
		conn=TRUE;
		keepAliveTimer->start(30000);
		
		emit( connected() );
		
		QString cmd = "*STA";
		Socket->writeBlock( cmd.latin1(), cmd.length() );
	}else
	{
		emit( error( ErrPeerProtVersionOld ) );
		Socket->close();
	}
    }else if(command == "*STR")
    {
    }else if(command == "*GFR")
    {
	int channels = data[0];
	data.remove(0,1);	
	for(int i=0;i<channels;i++)
	{
		channelValues[i] = data[0];
		data.remove(0,1);
		emit( sliderMove( i, channelValues[i] ) );
	}
	emit( channelsRefreshed() );
    }else if(command == "*BUA")  // Button Action
    {
    	qDebug("received Button Action" );
	emit( butPres( (unsigned char)data[0], (unsigned char)data[1] ) );
    }else
   	qWarning("received unknown Data from Device: "+data);
}

const bool mipuCom::isConnected( )
{
	return conn;
}

void mipuCom::keepAlive( )
{
	QString cmd = "*STA";
	Socket->writeBlock( cmd.latin1(), 4 );
}

void mipuCom::getAllChannels( )
{
	QString cmd = "*GFA";
	Socket->writeBlock( cmd.latin1(), 4 );
}

const int mipuCom::getSlider( int slider )
{
	return channelValues[slider];
}




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
#include "timedisplay.h"

timeDisplay::timeDisplay(QWidget *parent)
 : QLabel(parent), dMode( FrameDisplay )
{
    	timeDisplay_font = font();

    	timeDisplay_font.setPointSize( 15 );
    	timeDisplay_font.setBold( TRUE );

    	setFont( timeDisplay_font );

    	setFrameShape( QLabel::Panel );
    	setFrameShadow( QLabel::Sunken );
        setAlignment( Qt::AlignCenter );
	reset();
}

timeDisplay::~timeDisplay()
{
}

void timeDisplay::reset()
{
    setText( "--------" );
    curPos = QTime(0,0,0,0);
    length = QTime(0,0,0,0);
}

void timeDisplay::setPosition( QTime position )
{
	curPos = position;
	refresh();
}

void timeDisplay::setTotal( QTime length )
{
	this->length = length;
	refresh();
}

void timeDisplay::setDisplayMode( displayMode dMode )
{
	this->dMode = dMode;
	refresh();
}

void timeDisplay::setPreroll( QTime length )
{
	preroll = length;
	refresh();
}

void timeDisplay::refresh( )
{
    QTime remain = QTime(0,0,0,0).addMSecs(curPos.msecsTo( length ));

    QPalette pal = palette();
    if( remain < QTime(0,0,15) && remain != QTime(0,0,0,0) )
        pal.setColor( QPalette::Text, Qt::red );
    else if( curPos < preroll )
        pal.setColor( QPalette::Text, QColor( 255,255,0,0 ) );
    else
        pal.setColor( QPalette::Text, Qt::black );

    switch( dMode )
    {
	case FrameDisplay:
	case SecordDisplay:
		QString remString = remain.toString("mm:ss.zzz");
		if( remString == QString() )
		    reset();
		else
		    setText(QString("-")+remString);
		break;
    }
}

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
#include "timedisplay.h"

timeDisplay::timeDisplay(QWidget *parent, const char *name)
 : QLabel(parent, name), dMode( FrameDisplay )
{
    	timeDisplay_font = font();

    	timeDisplay_font.setPointSize( 15 );
    	timeDisplay_font.setBold( TRUE );

    	setFont( timeDisplay_font );

    	setFrameShape( QLabel::Panel );
    	setFrameShadow( QLabel::Sunken );
    	setAlignment( int( QLabel::AlignCenter ) );
	setText( "-0:00.00" );

}


timeDisplay::~timeDisplay()
{
}

void timeDisplay::setPosition_Samples( float samples )
{
	curPos = samples;
	refresh();
}

void timeDisplay::setPosition_Seconds( float seconds )
{
	curPos = seconds;
}

void timeDisplay::setTotal_Samples( float samples )
{
	saMax = samples;
}

void timeDisplay::setTotal_Seconds( float seconds )
{
	seMax = seconds;
}

void timeDisplay::refresh( )
{
	switch( dMode )
	{
		case FrameDisplay:
			if( saMax == 0 && frMax == 0 )
			{
				setText("ERR");
			}else
			{
				QString sekString;
				QString framString;

				unsigned int frames = frMax?( (frMax<curPos)?0:(unsigned int)(frMax-curPos) ):(unsigned int)(((saMax-curPos)/sampleRate)*25);
				unsigned int dMin = frames/1500;
				unsigned int dSec = (frames/25)%60;
				if( dSec < 10 )
					sekString = "0"+QString::number( dSec );
				else
					sekString = QString::number( dSec );
				unsigned int dFra = frames%25;
				if( dFra < 10 )
					framString = "0"+QString::number( dFra );
				else
					framString = QString::number( dFra );

				if( dSec < 16 && dMin == 0)
					setPaletteForegroundColor( QColor( 255, 0, 0 ) );
				else
					setPaletteForegroundColor( QColor( 0, 0, 0 ) );

				setText("-"+QString::number( dMin)+":"+sekString+"."+framString);
			}
			break;
		case SecordDisplay:
			if( seMax == 0 )
			{
				setText("ERR");
			}else
			{
			}
			break;
	}
}

void timeDisplay::setDisplayMode( displayMode dMode )
{
	this->dMode = dMode;
}

void timeDisplay::setSamplerate( unsigned int sRate )
{
	sampleRate = sRate;
}

void timeDisplay::setPosition_Frames( float frames )
{
	curPos = frames;
	refresh();
}

void timeDisplay::setTotal_Frames( float frames )
{
	frMax = frames;
}



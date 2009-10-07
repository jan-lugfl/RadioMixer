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
#include "vumeter.h"
//Added by qt3to4:
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPixmap>

vuMeter::vuMeter(QWidget *parent, const char *name)
 : QWidget(parent, name), level(0.f), canRepaint(FALSE)
{
	bgColor = parent->paletteBackgroundColor();
	buffer = new QPixmap();
	vuTimer = new QTimer(this, "vu Timer");

        // sets some usefull minimum sizes for this widget...
        setMinimumWidth( 10 );
        setMinimumHeight( 100 );
//       connect(vuTimer, SIGNAL(timeout()), this , SLOT(timeSlot()));
}


vuMeter::~vuMeter()
{
}

void vuMeter::paintEvent( QPaintEvent * event)
{
        //Q_UNUSED( event );
        paint();
        // bitBlt(this, 0, 0, buffer);
}

void vuMeter::setLevel( float level )
{
	this->level = level;
        repaint();
}

void vuMeter::drawLevelMeter( )
{
	canRepaint = TRUE;
	if(!buffer)
		qWarning("NULL pixmap buffer!");
/*
    for(int i = 0; i < 2 + 2; i++) {
       peak[i] = hold[i] = 1;
       holdTime[i] = 20;
    }
*/
    numLED = height()/10;
    for( int i = 0; i < numLED; i++) {
	if( i < numLED*0.75)
		color[i] = Qt::green;
	else if( i < numLED*0.9)
		color[i] = Qt::yellow;
	else if( i <= numLED)
		color[i] = Qt::red;
	else
		color[i] = Qt::blue;
    }

    setBackgroundMode(Qt::NoBackground);
    buffer->fill( bgColor );
}

void vuMeter::paint( )
{
    QPainter painter( buffer );
    for(int i = numLED; i > 0; i--) {
	if(level >= 1)
	{
             painter.setBrush(color[i]);
	}
	else
	{
             painter.setBrush(color[i].dark(300));
	}
        painter.drawRect( 1, height()-10*i, width()-2, 8);
	level *= 1.25;
    }
    painter.end();
    bitBlt(this, 0, 0, buffer);
}

void vuMeter::timeSlot( )
{
int i;
//paint();
for(i = 0; i < 2 + 2; i++) {
	peak[i] /= 2;
}
}

void vuMeter::resizeEvent( QResizeEvent * event )
{
	buffer->resize(event->size());
	drawLevelMeter();
}

void vuMeter::reset( )
{
	level=0.f;
}

void vuMeter::setPaletteBackgroundColor( const QColor & newColor)
{
	bgColor = newColor;
	if(canRepaint)
		drawLevelMeter( );
}

const QColor & vuMeter::paletteBackgroundColor( ) const
{
	return bgColor;
}



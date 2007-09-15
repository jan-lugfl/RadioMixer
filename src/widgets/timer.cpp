/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2007 by Jan Boysen                                *
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
#include "timer.h"

timer::timer(QWidget *parent, const char *name)
 : QFrame(parent, name)
{
	setFixedHeight( 90 );
	setFrameShape( StyledPanel );
	setFrameShadow( Raised );

//	timerName = new QLabel( this );
//	timerName->setText( tr( "time onAir" ) );

	start = new QToolButton( this );
	start->setText( tr("Start") );
	connect( start, SIGNAL( clicked() ), this, SLOT( startTimer() ) );
	pause = new QToolButton( this );
	pause->setText( tr("Pause") );
	connect( pause, SIGNAL( clicked() ), this, SLOT( pauseTimer() ) );
	stop = new QToolButton( this );
	stop->setText( tr("Stop") );
	connect( stop, SIGNAL( clicked() ), this, SLOT( stopTimer() ) );
	setup = new QToolButton( this );
	setup->setText( tr("Setup") );
	setup->setDisabled( TRUE );
	connect( setup, SIGNAL( clicked() ), this, SLOT( showSettings() ) );

	masterLayout = new QVBoxLayout( this, 4, 3, "Timer Layout" );

	buttonLayout = new QHBoxLayout( this, 2, 4, "Button Layout" );
	buttonLayout->addWidget( start );
	buttonLayout->addWidget( pause );
	buttonLayout->addWidget( stop );
	buttonLayout->addWidget( setup );

	label = new QLabel( this );
//	masterLayout->addWidget( timerName );
	masterLayout->addLayout( buttonLayout );
	masterLayout->addWidget( label );

	label->setFrameShape( QLabel::Panel );
	label->setFrameShadow( QLabel::Sunken );
	label->setAlignment( QLabel::AlignCenter );

	timeDisplay_font = font();
	timeDisplay_font.setPointSize( 22 );
	timeDisplay_font.setBold( TRUE );
	label->setFont( timeDisplay_font );

	label->setText( timerState.toString("hh:mm:ss.zzz").left(10) );
	mainTimer = new QTimer();
	connect( mainTimer, SIGNAL( timeout() ), this, SLOT( refreshTimer() ) );
}


timer::~timer()
{
}

void timer::startTimer( )
{
	mainTimer->start( 100 );
}

void timer::pauseTimer( )
{
	mainTimer->stop();
}

void timer::stopTimer( )
{
	mainTimer->stop();
	timerState = QTime( 0, 0 );
	label->setText( timerState.toString("hh:mm:ss.zzz").left(10) );
}

void timer::showSettings( )
{
	qWarning( "not implemented yet..." );
}

void timer::refreshTimer( )
{
	timerState = timerState.addMSecs( 100 );
	label->setText( timerState.toString("hh:mm:ss.zzz").left(10) );
}



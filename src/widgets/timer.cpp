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
 : QFrame(parent, name), reverse( FALSE )
{
	setFixedHeight( 100 );
	setFrameShape( StyledPanel );
	setFrameShadow( Raised );

	QFont labelFont;
	labelFont.setPointSize(8);
	labelFont.setBold( TRUE );

	timerName = new QLabel( this );
	timerName->setFrameShape( QLabel::Panel );
	timerName->setFrameShadow( QLabel::Sunken );
	timerName->setFont( labelFont );
	timerName->setText( tr( "timer" ) );

	closeButton = new QToolButton( this );
	closeButton->setPaletteBackgroundColor( red );
	closeButton->setFixedSize( QSize(15,15) );
	connect( closeButton, SIGNAL( clicked() ), this, SLOT( close() ) );

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
	connect( setup, SIGNAL( clicked() ), this, SLOT( showSettings() ) );

	masterLayout = new QVBoxLayout( this, 4, 3, "Timer Layout" );
	headerLayout = new QHBoxLayout( masterLayout, 2 );
	buttonLayout = new QHBoxLayout( masterLayout, 4 );

	buttonLayout->addWidget( start );
	buttonLayout->addWidget( pause );
	buttonLayout->addWidget( stop );
	buttonLayout->addWidget( setup );

	headerLayout->addWidget( timerName );
	headerLayout->addWidget( closeButton );

	label = new QLabel( this );
	masterLayout->addWidget( label );

	label->setFrameShape( QLabel::Panel );
	label->setFrameShadow( QLabel::Sunken );
	label->setAlignment( QLabel::AlignCenter );

	timeDisplay_font = font();
	timeDisplay_font.setPointSize( 22 );
	timeDisplay_font.setBold( TRUE );
	label->setFont( timeDisplay_font );

	label->setText( "00:00:00.0" );
	mainTimer = new QTimer();
	connect( mainTimer, SIGNAL( timeout() ), this, SLOT( timerEvent() ) );
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
	timerState = 0;
	label->setText( "00:00:00.0" );
}

void timer::showSettings( )
{
	timerSettings* settings = new timerSettings( this, "timer settings");
	settings->timerName->setText( timerName->text() );
	if( reverse )
	{
		settings->forward->setChecked(FALSE);
		settings->reverse->setChecked(TRUE);
	}

	if( settings->exec() == QDialog::Accepted )
	{
		timerName->setText( settings->timerName->text() );
		if( settings->reverse->isChecked() )
		{
			reverse = TRUE;
			timerState = settings->hoursBox->value()*3600000 + settings->minutesBox->value()*600000+settings->secondsBox->value()*1000;
		}else
			reverse = FALSE;
	}
	refreshTimer( );

	delete settings;
}

void timer::refreshTimer( )
{
	int hours = timerState/3600000;
	int minutes = (timerState%3600000)/60000;
	int seconds = (timerState%60000)/1000;
	int usec = (timerState%1000)/100;
	label->setText( QString(
			((hours<10)?QString("0")+QString::number(hours):QString::number(hours))+
			":"+
			((minutes<10)?QString("0")+QString::number(minutes):QString::number(minutes))+
			":"+
			((seconds<10)?QString("0")+QString::number(seconds):QString::number(seconds))+
			"."+
			QString::number(usec)
		) );
}

void timer::timerEvent()
{
	if( reverse )
		timerState -= 100;
	else
		timerState += 100;

	if(timerState <= 0)
		stopTimer();

	refreshTimer();
}



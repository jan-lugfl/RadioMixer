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
#ifndef TIMER_H
#define TIMER_H

//#include "timerSettings.h"
#include <qlineedit.h>
#include <qspinbox.h>

#include <q3frame.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class timer : public Q3Frame
{
Q_OBJECT
public:
	timer(QWidget *parent = 0, const char *name = 0);
	~timer();

protected:
	QTimer* mainTimer;
	bool reverse;
	int timerState;

	Q3VBoxLayout* masterLayout;
	Q3HBoxLayout* buttonLayout;
	Q3HBoxLayout* headerLayout;
	QToolButton* start;
	QToolButton* stop;
	QToolButton* pause;
	QToolButton* setup;
	QToolButton* closeButton;
	QLabel* label;
	QLabel* timerName;
	QFont timeDisplay_font;

protected slots:
	virtual void startTimer();
	virtual void pauseTimer();
	virtual void stopTimer();
	virtual void refreshTimer();
	virtual void timerEvent();
	virtual void showSettings();
};

#endif

/* $Id$ */
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
#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <QLabel>
#include <QTime>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class timeDisplay : public QLabel
{
Q_OBJECT
public:
    timeDisplay(QWidget *parent = 0);
    ~timeDisplay();

    enum displayMode { FrameDisplay, SecordDisplay };

public slots:
	virtual void setPosition( QTime position );
	virtual void setTotal( QTime length );
	virtual void setPreroll( QTime preroll );
	virtual void setDisplayMode( displayMode dMode );
	virtual void reset();
	virtual void refresh();

protected:
	QFont timeDisplay_font;
	displayMode dMode;
	QTime length;
	QTime preroll;
	QTime curPos;
};

#endif

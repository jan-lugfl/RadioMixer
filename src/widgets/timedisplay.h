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

#include <qlabel.h>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class timeDisplay : public QLabel
{
Q_OBJECT
public:
    timeDisplay(QWidget *parent = 0, const char *name = 0);
    ~timeDisplay();

    enum displayMode { FrameDisplay, SecordDisplay };

public slots:
	virtual void setPosition_Samples( float samples );
	virtual void setPosition_Seconds( float seconds );
	virtual void setPosition_Frames( float frames );
	virtual void setTotal_Samples( float samples );
	virtual void setTotal_Seconds( float seconds );
	virtual void setTotal_Frames( float frames );
	virtual void setPreroll_Frames( float frames );
	virtual void setSamplerate( unsigned int sRate );
	virtual void setDisplayMode( displayMode dMode );
	virtual void refresh();

protected:
	QFont timeDisplay_font;
	displayMode dMode;
	float saMax;
	float seMax;
	float frMax;
	float curPos;
	float preroll;
	unsigned int sampleRate;

};

#endif

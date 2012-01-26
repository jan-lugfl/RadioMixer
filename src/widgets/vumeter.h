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
#ifndef VUMETER_H
#define VUMETER_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

#include <QPaintEvent>
#include <QResizeEvent>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class vuMeter : public QWidget
{
Q_OBJECT
public:
    vuMeter(QWidget *parent = 0);
    ~vuMeter();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent*event);
    virtual void drawLevelMeter();
    virtual void paint();

   QPixmap* buffer;
   QTimer* vuTimer;

   QColor bgColor;
   QColor fgColorLo;
   QColor fgColorMi;
   QColor fgColorHi;

private:
    float level;
    int x, y, dx, dy, ox, oy, sx, sy, colorScheme;
    float peak[], hold[4];
    int holdTime[4];
   QColor color[255];
    int numLED;
    bool canRepaint;

protected slots:
    virtual void timeSlot();

public slots:
     virtual void reset();
     virtual void setLevel( float level );

};

#endif

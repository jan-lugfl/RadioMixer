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
#ifndef GLOWBUTTON_H
#define GLOWBUTTON_H

#include <QToolButton>

/**
@author Jan Boysen
*/
class glowButton : public QToolButton
{
Q_OBJECT
public:
    glowButton(QWidget *parent = 0);
    ~glowButton();

	virtual void setPaletteBackgroundColor ( const QColor & newCol);
	virtual void setActivatedColor ( const QColor &newCol );
	virtual void setDeactivatedColor ( const QColor &newCol );

public slots:
        virtual void toggle();
        virtual void setOn();
        virtual void setOff();
        virtual void setState( bool state );

protected:
	bool state;
	QColor actColor;
	QColor deactColor;

        virtual void mouseReleaseEvent ( QMouseEvent * e );

signals:
        void rightClicked();
};

#endif

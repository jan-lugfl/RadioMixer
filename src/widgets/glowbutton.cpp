/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
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
#include "glowbutton.h"

#include <QMouseEvent>

glowButton::glowButton(QWidget *parent)
 : QToolButton(parent), state(FALSE)
{
	actColor = QColor( 255, 0, 0 );
        deactColor = palette().color( QPalette::Button );
        setMinimumHeight( 32 );
        setMinimumWidth( 32 );
        setAutoFillBackground( true );
        setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}


glowButton::~glowButton()
{
}

void glowButton::toggle( )
{
	if( state )
	{
		state = FALSE;
		setOff();
	}
	else
	{
		state = TRUE;
		setOn();
	}
}

void glowButton::setOn( )
{
	state = TRUE;
        QPalette tempPal = this->palette();
        tempPal.setColor( QPalette::Button, actColor );
        this->setPalette( tempPal );
}

void glowButton::setOff( )
{
	state = FALSE;
        QPalette tempPal = this->palette();
        tempPal.setColor( QPalette::Button, deactColor );
        this->setPalette( tempPal );
}

void glowButton::setPaletteBackgroundColor( const QColor & newCol )
{
	actColor = newCol;
        if(state)
        {
            QPalette tempPal = this->palette();
            tempPal.setColor( QPalette::Button, actColor );
            this->setPalette( tempPal );
        }
}

void glowButton::setActivatedColor( const QColor & newCol )
{
	actColor = newCol;
        if(state)
        {
            QPalette tempPal = this->palette();
            tempPal.setColor( QPalette::Button, actColor );
            this->setPalette( tempPal );
        }
}

void glowButton::setDeactivatedColor( const QColor & newCol )
{
	deactColor = newCol;
        if(!state)
        {
            QPalette tempPal = this->palette();
            tempPal.setColor( QPalette::Button, actColor );
            this->setPalette( tempPal );
        }
}

void glowButton::setState( bool state )
{
	if( state )
		setOn();
	else
		setOff();
}

void glowButton::mouseReleaseEvent ( QMouseEvent * e )
{
    if( e->button() == Qt::RightButton )
        emit( rightClicked() );
    QAbstractButton::mouseReleaseEvent( e );
}

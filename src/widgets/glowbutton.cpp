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
#include "glowbutton.h"

glowButton::glowButton(QWidget *parent, const char *name)
 : QToolButton(parent, name), state(FALSE)
{
	actColor = QColor( 255, 0, 0 );
        deactColor = paletteBackgroundColor();
        setMinimumHeight( 32 );
        setMinimumWidth( 32 );
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
        QToolButton::setPaletteBackgroundColor( actColor );
}

void glowButton::setOff( )
{
	state = FALSE;
        QToolButton::setPaletteBackgroundColor( deactColor );
}

void glowButton::setPaletteBackgroundColor( const QColor & newCol )
{
	actColor = newCol;
        QToolButton::setPaletteForegroundColor( newCol );
}

void glowButton::setActivatedColor( const QColor & newCol )
{
	actColor = newCol;
}

void glowButton::setDeactivatedColor( const QColor & newCol )
{
	deactColor = newCol;
}

void glowButton::setState( bool state )
{
	if( state )
		setOn();
	else
		setOff();
}



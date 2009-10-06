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
#include "blinkbutton.h"

blinkButton::blinkButton(QWidget *parent, const char *name)
 : glowButton(parent, name), colState(FALSE)
{
	connect( &blinkTimer, SIGNAL(timeout()), this, SLOT( blinker()));
	blinkTimer.start( BLINKSPEED );
}


blinkButton::~blinkButton()
{
}

void blinkButton::blinker( )
{
	if(state)
		if( colState )
		{
                        QToolButton::setPaletteBackgroundColor( actColor );
			colState = FALSE;
		}
		else
		{
                        QToolButton::setPaletteBackgroundColor( deactColor );
			colState = TRUE;
		}
        else if( !colState )
		glowButton::setOff();
}

void blinkButton::setState( bool state )
{
	this->state = state;
}

void blinkButton::toggle( )
{
        state = !state;
}

void blinkButton::setOff( )
{
	state = FALSE;
}

void blinkButton::setOn( )
{
	state = TRUE;
}

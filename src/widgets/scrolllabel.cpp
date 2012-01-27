/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2012 by Jan Boysen                                      *
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
#include "scrolllabel.h"

scrollLabel::scrollLabel(QWidget *parent)
    : QLabel(parent), scroll_position(0), display_chars(12), scroll_left(false)
{
    display_font = font();
    display_font.setPointSize( 12 );
    display_font.setBold( TRUE );

    setFont( display_font );
    setAlignment( Qt::AlignCenter );

    setFrameShape( QLabel::Panel );
    setFrameShadow( QLabel::Sunken );

    connect( &scrollTimer, SIGNAL(timeout()), this, SLOT( scroll_text()));
}

scrollLabel::~scrollLabel()
{
}

void scrollLabel::scroll_text()
{
    if(scroll_left)
        scroll_position--;
    else
        scroll_position++;
    if(scroll_position > full_text.length()-display_chars)
        scroll_left = true;
    if(scroll_position < 1)
        scroll_left = false;

    QLabel::setText( full_text.mid( scroll_position, display_chars) );
}

void scrollLabel::setText(const QString& new_text)
{
    full_text = new_text;
    if(full_text.length() <= display_chars)
    {
        scrollTimer.stop();
        QLabel::setText(full_text);
    }
    else
    {
        scroll_position = 0;
        scroll_left = false;
        scrollTimer.start( 200 );
        scroll_text();
    }
}

QString scrollLabel::text() const
{
    return full_text;
}

void scrollLabel::setDisplayedChars( unsigned int displayed_chars )
{
    display_chars = displayed_chars;
}

unsigned int scrollLabel::displayedChars() const
{
    return display_chars;
}

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
#ifndef SCROLLLABEL_H
#define SCROLLLABEL_H

#include <QLabel>
#include <QTimer>

/**
    @author Jan Boysen <jan@mediaweb-it.net>
*/
class scrollLabel : public QLabel
{
Q_OBJECT
public:
    scrollLabel(QWidget *parent = 0);
    ~scrollLabel();
    void setDisplayedChars( unsigned int );
    unsigned int displayedChars() const;
    QString text() const; // reimplented so we return the full text instead of what is currently displayed...

protected:
    QString full_text;
    QFont display_font;
    QTimer scrollTimer;
    unsigned int scroll_position;
    unsigned int display_chars;
    bool scroll_left;

public slots:
    virtual void setText(const QString& new_text); // reimplement this to store the full original text we scroll..

protected slots:
    virtual void scroll_text();

};

#endif // SCROLLLABEL_H

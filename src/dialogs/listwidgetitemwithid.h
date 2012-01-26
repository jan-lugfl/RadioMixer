/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2010 by Jan Boysen                                      *
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
#ifndef LISTWIDGETITEMWITHID_H
#define LISTWIDGETITEMWITHID_H

#include <QUuid>
#include <QListWidget>
#include <QListWidgetItem>

class listWidgetItemWithId : public QListWidgetItem
{
public:
    static int const type;
    explicit listWidgetItemWithId( QListWidget * parent = 0, QUuid uuid = QUuid(), int type = type );

    virtual void setUuid( QUuid id );
    QUuid getUuid();

protected:
    QUuid uuid; // a hidden field identifier...
};

#endif // LISTWIDGETITEMWITHID_H

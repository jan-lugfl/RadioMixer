/* $Id:$ */
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
#include "remotecontrol.h"

// init static controls list
QList<remoteControl*> remoteControl::allControls;

remoteControl::remoteControl(QObject *parent, QString name, bool bidirectional) :
    QObject(parent), bidirectional(bidirectional)
{
    allControls.append(this);
    setName( name );
}

remoteControl::~remoteControl()
{
    qWarning("destructor called...");
    allControls.removeOne( this );
}

void remoteControl::setName(QString newName)
{
    name = newName;
}

QString remoteControl::getName()
{
    return name;
}

QList<remoteControl*> remoteControl::getAllControls()
{
    qWarning(QString::number( allControls.count()));
    return allControls;
}

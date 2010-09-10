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
#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include "remotecontrolchannel.h"
#include <QObject>
#include <QVector>

class remoteControl : public QObject
{
    Q_OBJECT
public:
    remoteControl(QObject *parent = 0, QString name = QString("Remote Control"), bool bidirectional = true );

    // needs to be public as maybe existing threads needs access to iterate channels
    QVector<remoteControlChannel*> channels;

protected:
    bool bidirectional;

signals:

public slots:

};

#endif // REMOTECONTROL_H

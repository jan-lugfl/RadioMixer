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

#ifndef AUTOMATION_H
#define AUTOMATION_H

#include "mixerchannel.h"
#include <QString>
#include <QMetaMethod>
#include <QVector>

class Automation
{
public:
    static void add(mixerChannel* sender, mixerChannel* receiver, QString sender_action, QString recv_action);
    static void loadSettings();
    static void saveSettings();

    struct Action {
        mixerChannel* sender;
        QMetaMethod signal;
        mixerChannel* receiver;
        QMetaMethod action;
    };
    static QVector<Action> actions;
};

#endif // AUTOMATION_H

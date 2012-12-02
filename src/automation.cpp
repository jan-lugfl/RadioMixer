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

#include "automation.h"
#include "settings.h"
#include <QVariant>
#include <QList>
#include <QStringList>

#include "mixerchannelmanager.h"

// Init static storage
QVector<Automation::Action> Automation::actions = QVector<Automation::Action>();

void Automation::add(mixerChannel* sender, mixerChannel* receiver, QString sender_action, QString recv_action)
{
    Action action;

    action.sender = sender;
    action.receiver = receiver;
    action.signal = sender->metaObject()->method(sender->metaObject()->indexOfMethod(sender_action.toStdString().c_str()));
    action.action = receiver->metaObject()->method(receiver->metaObject()->indexOfMethod(recv_action.toStdString().c_str()));

    sender->connect(action.sender, action.signal, action.receiver, action.action);
    actions.append(action);
}

void Automation::saveSettings()
{
    QList<QVariant> automations;
    Action itm;
    foreach( itm, actions )
    {
        QStringList setting;
        setting.append( itm.sender->getUuid() );
        setting.append( itm.receiver->getUuid() );
        setting.append( itm.signal.signature() );
        setting.append( itm.action.signature() );
        automations.append( setting );
    }

    Settings::set("Automation", QVariant(automations));
}

void Automation::loadSettings()
{
    QList<QVariant> automations = Settings::get("Automation").toList();
    QVariant itm;
    foreach( itm, automations)
    {
        QStringList setting = itm.toStringList();
        mixerChannel* sender = mixerChannelManager::getChannelByUuid( setting.at(0) );
        mixerChannel* receiver = mixerChannelManager::getChannelByUuid( setting.at(1) );
        add(sender, receiver, setting.at(2), setting.at(3));
    }
}

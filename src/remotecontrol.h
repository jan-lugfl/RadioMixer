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
#include <QMap>

class remoteControl : public QObject
{
    Q_OBJECT
public:
    explicit remoteControl(QObject *parent = 0, QString name = QString("Remote Control"), bool bidirectional = true );
    virtual ~remoteControl();

    // needs to be public as maybe existing threads needs access to iterate channels
    QVector<remoteControlChannel*> channels;

    void setName( QString newName );
    QString getName();

    static QList<remoteControl*> getAllControls();

    typedef QMap<remoteControlChannel::RemoteControlerEvent, QVariant> channelConfig;
    void setChannelConfig( QUuid channel, channelConfig config );
    void removeChannelConfig( QUuid uuid );
    remoteControl::channelConfig getChannelConfig( QUuid channel );

    virtual remoteControlChannel* createChannel(QUuid uuid = QUuid()) = 0;

    void saveConfig();
    static remoteControl* createFromConfig( QString nameInConfig );

protected:
    QString name;
    bool bidirectional;
    QMap<QUuid, channelConfig> controller_config;

private:
    static QList<remoteControl*> allControls; // used to get all existing controls...

signals:

public slots:

};

#endif // REMOTECONTROL_H

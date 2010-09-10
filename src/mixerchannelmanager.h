/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2009 by Jan Boysen                                      *
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

#ifndef MIXERCHANNELMANAGER_H
#define MIXERCHANNELMANAGER_H

#include "mixerchannel.h"

#include <QVector>
#include <QThread>

class mixerChannelManager
{
public:
    static void registerChannel( mixerChannel* );
    static void unregisterChannel( mixerChannel* );
    static void init_supportedChannels();
    static mixerChannel* getChannelByUuid( QUuid uuid );
    static QStringList getChannelTypes();
    static QStringList getChannelTypeNames();
    static QString getChannelTypeFromTypeName( QString typeName );

    static QMap<QString,QString> supportedChannelTypes;

    typedef QVector<mixerChannel*> storageType;

    static  storageType allChannels;
    static  storageType inChannels;
    static  storageType outChannels;
};

#endif // MIXERCHANNELMANAGER_H

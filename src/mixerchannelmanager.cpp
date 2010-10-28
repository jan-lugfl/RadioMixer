/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2009-2010 by Jan Boysen                                 *
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

#include "mixerchannelmanager.h"

// init static members.
mixerChannelManager::storageType mixerChannelManager::allChannels = mixerChannelManager::storageType();
mixerChannelManager::storageType mixerChannelManager::inChannels = mixerChannelManager::storageType();
mixerChannelManager::storageType mixerChannelManager::outChannels = mixerChannelManager::storageType();
QMap<QString,QString> mixerChannelManager::supportedChannelTypes = QMap<QString,QString>();
metaInfo mixerChannelManager::metaManager;

void mixerChannelManager::registerChannel( mixerChannel* newChann )
{
    if(!newChann) // registering NULL pointer makes no sence...
        return;

    allChannels.append( newChann );
    metaManager.connect( newChann, SIGNAL(newMeta(metaTag)), &metaManager, SLOT(setMeta(metaTag)));

    if( newChann->getAudioDataType() == mixerChannel::AudioDataIn )
        inChannels.append(newChann );

    if( newChann->getAudioDataType() == mixerChannel::AudioDataOut )
        outChannels.append(newChann );
}

void mixerChannelManager::unregisterChannel( mixerChannel* channel )
{
    if(!channel) // unregistering null pointer makes no sence :D
        return;

    metaManager.disconnect( channel, SIGNAL(newMeta(metaTag)), &metaManager, SLOT(setMeta(metaTag)));
    removeChannelFromVector( &allChannels, channel );
    removeChannelFromVector( &inChannels, channel );
    removeChannelFromVector( &outChannels, channel );
}

void mixerChannelManager::removeChannelFromVector( storageType* stor, mixerChannel* chan )
{
    mixerChannelManager::storageType::iterator it;
    for( it = stor->begin(); it != stor->end(); it++ )
        if( (*it) == chan )
        {
            stor->erase( it );
            return;
        }
}

mixerChannel* mixerChannelManager::getChannelByUuid( QUuid uuid )
{
    foreach( mixerChannel* chan, allChannels )
        if( chan->getUuid() == uuid )
            return chan;
    return 0;
}

void mixerChannelManager::init_supportedChannels()
{
    supportedChannelTypes.insert("PLAYER", QObject::tr("File player"));
#ifdef HAVE_ALSA
    supportedChannelTypes.insert("ALSAMIX", QObject::tr("ALSA Mixer"));
#endif
#ifdef HAVE_JACK
    supportedChannelTypes.insert("JACKIN", QObject::tr("Jack input"));
    supportedChannelTypes.insert("JACKOUT", QObject::tr("Jack output"));
#endif
}

QStringList mixerChannelManager::getChannelTypes()
{
    if(supportedChannelTypes.isEmpty())
        init_supportedChannels();
    return supportedChannelTypes.keys();
}

QStringList mixerChannelManager::getChannelTypeNames()
{
    if(supportedChannelTypes.isEmpty())
        init_supportedChannels();
    return supportedChannelTypes.values();
}

QString mixerChannelManager::getChannelTypeFromTypeName( QString typeName )
{
    if(supportedChannelTypes.isEmpty())
        init_supportedChannels();
    return supportedChannelTypes.key( typeName );
}

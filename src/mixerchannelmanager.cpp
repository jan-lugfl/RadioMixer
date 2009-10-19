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

#include "mixerchannelmanager.h"

// init static members.
mixerChannelManager::storageType mixerChannelManager::allChannels = mixerChannelManager::storageType();
mixerChannelManager::storageType mixerChannelManager::inChannels = mixerChannelManager::storageType();
mixerChannelManager::storageType mixerChannelManager::outChannels = mixerChannelManager::storageType();

void mixerChannelManager::registerChannel( mixerChannel* newChann )
{
    allChannels.append( newChann );

    if( newChann->getAudioDataType() == mixerChannel::AudioDataIn )
        inChannels.append(newChann );

    if( newChann->getAudioDataType() == mixerChannel::AudioDataOut )
        outChannels.append(newChann );
}

void mixerChannelManager::unregisterChannel( mixerChannel* channel )
{
    mixerChannelManager::storageType::iterator it;
    for( it = allChannels.begin(); it != allChannels.end(); it++ )
        if( (*it) == channel )
            allChannels.erase( it );
    for( it = inChannels.begin(); it != inChannels.end(); it++ )
        if( (*it) == channel )
            inChannels.erase( it );
    for( it = outChannels.begin(); it != outChannels.end(); it++ )
        if( (*it) == channel )
            outChannels.erase( it );
}

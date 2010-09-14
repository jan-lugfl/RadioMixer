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
#include "settings.h"

#include "remotecontrol_midi.h"
#include "mixerchannelmanager.h"

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
    return allControls;
}

void remoteControl::setChannelConfig( QUuid channel, remoteControl::channelConfig config )
{
    controller_config[channel] = config;
}

remoteControl::channelConfig remoteControl::getChannelConfig( QUuid channel )
{
    return controller_config[channel];
}

void remoteControl::removeChannelConfig(QUuid channel)
{
    controller_config.remove( channel );
}

void remoteControl::saveConfig()
{
    // remove the config for this remote control first..
    Settings::remove("remote_controls/"+name);

    // generate new config file content...
    foreach(QUuid uuid, controller_config.keys())
    {
        foreach(remoteControlChannel* channel, channels)
            if(channel->getUuid() == uuid)
            {
                Settings::set( "remote_controls/"+name+"/"+uuid.toString()+"/attached_mixers", channel->getAttachedTo() );
                Settings::set( "remote_controls/"+name+"/"+uuid.toString()+"/name", channel->getName() );
            }

        foreach(remoteControlChannel::RemoteControlerEvent key, controller_config[uuid].keys())
            Settings::set("remote_controls/"+name+"/"+uuid.toString()+"/"+QString::number(key), controller_config[uuid][key] );
    }
}

remoteControl* remoteControl::createFromConfig( QString nameInConfig )
{
    remoteControl* control = new remoteControl_MIDI( 0, nameInConfig );

    foreach(QString uuid, Settings::getSubGroups("remote_controls/"+nameInConfig) )
    {
        remoteControlChannel* chan = control->createChannel( uuid );
        chan->setName( Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/name" ).toString() );
        channelConfig config;
        config[remoteControlChannel::event_volumeFader] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_volumeFader) );
        config[remoteControlChannel::event_balanceFader] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_balanceFader) );
        config[remoteControlChannel::event_playButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_playButton) );
        config[remoteControlChannel::event_stopButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_stopButton) );
        config[remoteControlChannel::event_pauseButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_pauseButton) );
        config[remoteControlChannel::event_queueButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_queueButton) );
        config[remoteControlChannel::event_repeatButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_repeatButton) );
        config[remoteControlChannel::event_openButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_openButton) );
        config[remoteControlChannel::event_muteButton] = Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/"+QString::number(remoteControlChannel::event_muteButton) );
        control->setChannelConfig( uuid, config );
        // auto attach to mixers....
        foreach( QString mixerUuid, Settings::get("remote_controls/"+nameInConfig+"/"+uuid+"/attached_mixers" ).toStringList() )
        {
            mixerChannel* mixer = mixerChannelManager::getChannelByUuid( QUuid( mixerUuid) );
            if( mixer )
                chan->associateToChannel( mixer );
        }
    }
    return control;
}

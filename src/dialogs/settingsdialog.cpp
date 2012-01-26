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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mixerchannelmanager.h"
#include "listwidgetitemwithid.h"
#include "remotecontrol_midi.h"

#include "mixerchannel_jackin.h"
#include "mixerchannel_jackout.h"

#include "mainWindow.h"
#include "settings.h"

#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    // fill the channel list...
    listWidgetItemWithId* item;
    foreach(mixerChannel* channel, mixerChannelManager::allChannels)
    {
        settingsCache.insert( channel->getUuid(), channel->getSettings() );

        item = new listWidgetItemWithId( ui->channelList, channel->getUuid() );
        item->setText( channel->getName() );
    }
    ui->channelList->setCurrentRow(0);

    // fill the remote controller list
    foreach( remoteControl* controller, remoteControl::getAllControls() )
        ui->remoteControllerList->insertItem( ui->remoteControllerList->count(), controller->getName() );

    // set Song database options
    ui->songDBURL->setText( Settings::get("songDBURL", "http://localhost/xmlctrl.pl").toString() );
    ui->songDBBasePath->setText( Settings::get("songDBBasePath", "/songs/").toString() );

    // set metadata options
    ui->meta_script->setText( Settings::get("meta/command").toString() );
    ui->meta_enable->setChecked(Settings::get("meta/enabled", true).toBool() );

    // Set the icons for the buttons..
    ui->upButton->setIcon( QApplication::style()->standardIcon( QStyle::SP_ArrowUp ));
    ui->upButton->setText("");
    ui->downButton->setIcon( QApplication::style()->standardIcon( QStyle::SP_ArrowDown ));
    ui->downButton->setText("");
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void settingsDialog::accept()
{
    // upcast our parent which always should be mainWindow...
    mainWindow* mainWin = dynamic_cast<mainWindow*>(parent());
    if(!mainWin)
    {
        qWarning("not called from mainWindow class.....");
        return;
    }

    // save last changes..
    on_channelList_currentItemChanged( ui->channelList->currentItem(), ui->channelList->currentItem() );

    // create new channels...
    foreach(QUuid uuid, settingsCache.keys())
        if( !settingsCache[uuid]["create_channel_type"].isNull() )
        {
            QString new_uuid = mainWin->addNewChannel(  settingsCache[uuid]["create_channel_type"].toString() );
            // Update UUID
            settingsCache[new_uuid] = settingsCache[uuid];
            settingsCache[new_uuid].remove("create_channel_type");
            settingsCache.remove( uuid );

            // update uuid in channel list too...
            foreach(QListWidgetItem* item, ui->channelList->findItems("*", Qt::MatchWildcard))
            {
                listWidgetItemWithId* itm = dynamic_cast<listWidgetItemWithId*>(item);
                if( itm->getUuid() == uuid )
                    itm->setUuid( new_uuid );
            }
        }

    // apply settings
    foreach(mixerChannel* channel, mixerChannelManager::allChannels)
        channel->updateSettings( settingsCache[channel->getUuid()] );

    // assign channel list to var that is accessible from the outside..
    // its used to change the channel order..
    foreach(QListWidgetItem* item, ui->channelList->findItems("*", Qt::MatchWildcard))
    {
        listWidgetItemWithId* itm = dynamic_cast<listWidgetItemWithId*>(item);
        channels.append( itm->getUuid() );
    }

    // save song database api settings
    Settings::set( "songDBURL", ui->songDBURL->text() );
    Settings::set( "songDBBasePath", ui->songDBBasePath->text() );

    // save metadata options
    Settings::set( "meta/command", ui->meta_script->text() );
    Settings::set( "meta/enabled", ui->meta_enable->isChecked() );

    QDialog::accept();
}

void settingsDialog::on_upButton_clicked()
{
    int oldPos = ui->channelList->currentRow();
    if(oldPos == 0)
        return;
    ui->channelList->insertItem( oldPos-1, ui->channelList->takeItem(oldPos) );
    ui->channelList->setCurrentRow( oldPos-1 );
}

void settingsDialog::on_downButton_clicked()
{
    int oldPos = ui->channelList->currentRow();
    if(oldPos == ui->channelList->count()-1)
        return;
    ui->channelList->insertItem( oldPos+1, ui->channelList->takeItem(oldPos) );
    ui->channelList->setCurrentRow( oldPos+1 );
}

void settingsDialog::on_channel_remove_clicked()
{
    ui->channelList->takeItem( ui->channelList->currentRow() );
}

void settingsDialog::on_channelList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    QPalette pal = ui->colorChooser->palette();

    if(previous)
    {
        // save current setings to cache...
        QUuid prev_uuid = (dynamic_cast<listWidgetItemWithId*>(previous))->getUuid();

        // general settings
        settingsCache[prev_uuid]["name"] = ui->name->text();
        previous->setText( ui->name->text() );
        settingsCache[prev_uuid]["color"] = pal.color( QPalette::Button );
        // save type dependant settings..
        if(settingsCache[prev_uuid]["type"] == mixerChannel_filePlayer::Type)
        {
            settingsCache[prev_uuid]["autoload_fromplaylist"] = ui->fileplayer_autoload->isChecked();
            settingsCache[prev_uuid]["ignore_metadata"] = ui->fileplayer_ignoremeta->isChecked();
        }
        else if(settingsCache[prev_uuid]["type"] == mixerChannel_jackIn::Type || settingsCache[prev_uuid]["type"] == mixerChannel_jackOut::Type)
        {
            settingsCache[prev_uuid]["mute_button_mode"] = ui->mute_button->isChecked();
        }
    }

    // get the settings from the new selected item from cache
    QUuid cur_uuid = (dynamic_cast<listWidgetItemWithId*>(current))->getUuid();

    // assign general settings
    ui->name->setText( settingsCache[cur_uuid]["name"].toString() );
    pal.setColor( QPalette::Button, settingsCache[cur_uuid]["color"].value<QColor>() );
    ui->colorChooser->setPalette( pal );

    // show/hide channel type dependant settings
    ui->channelSettings->setItemEnabled(2, false);
    ui->channelSettings->setItemEnabled(3, false);
    ui->channelSettings->setItemEnabled(4, false);

    if( settingsCache[cur_uuid]["type"] == mixerChannel_filePlayer::Type )
    {
            ui->channelSettings->setItemEnabled(2, true);
            ui->fileplayer_autoload->setChecked(settingsCache[cur_uuid]["autoload_fromplaylist"].toBool());
            ui->fileplayer_ignoremeta->setChecked(settingsCache[cur_uuid]["ignore_metadata"].toBool());
    }
    if( settingsCache[cur_uuid]["type"] == mixerChannel_ALSA::Type )
            ui->channelSettings->setItemEnabled(3, true);
    if( settingsCache[cur_uuid]["type"] == mixerChannel_jackIn::Type || settingsCache[cur_uuid]["type"] == mixerChannel_jackOut::Type )
    {
        ui->channelSettings->setItemEnabled(4, true);
        ui->mute_button->setChecked(settingsCache[cur_uuid]["mute_button_mode"].toBool());
        ui->onair_button->setChecked(!settingsCache[cur_uuid]["mute_button_mode"].toBool());
    }
}

void settingsDialog::on_colorChooser_clicked()
{
    QPalette palette = ui->colorChooser->palette();
    QColorDialog color_dialog(palette.color( QPalette::Button ), this);
    color_dialog.exec();
    palette.setColor( QPalette::Button, color_dialog.selectedColor() );
    ui->colorChooser->setPalette( palette );
}

void settingsDialog::on_channel_add_clicked()
{
    bool ok;
    QString type = QInputDialog::getItem( this, tr("Type of the new channel"), tr("Please select the typr for the new channel"), mixerChannelManager::getChannelTypeNames(), 0, false, &ok);

    if(!ok)
        return;

    QUuid newChannelUuid = QUuid::createUuid();
    mixerChannel::settingsType newChannelSettings;
    newChannelSettings["create_channel_type"] = mixerChannelManager::getChannelTypeFromTypeName( type );
    newChannelSettings["name"] = tr("New Channel");
    newChannelSettings["color"] = palette().background().color();
    settingsCache.insert( newChannelUuid, newChannelSettings );
    listWidgetItemWithId* item;
    item = new listWidgetItemWithId( ui->channelList, newChannelUuid );
    item->setText( tr("New channel") );
    ui->channelList->setCurrentItem( item );
}

void settingsDialog::on_add_controller_clicked()
{
    bool ok;
    QString type = QInputDialog::getItem( this, tr("Type of the new controller"), tr("Please select the typr for the new controller"), QStringList(tr("MIDI Device")), 0, false, &ok);
    if(!ok)
        return;
    QString name = QInputDialog::getText( this, tr("Name of new controller"), tr("Please enter the name for the new controller"), QLineEdit::Normal, tr("New Controller"), &ok);
    if(!ok)
        return;

    if( type == tr("MIDI Device") )
    {
        remoteControl_MIDI* cntrl = new remoteControl_MIDI();
        cntrl->setName( name );
        ui->remoteControllerList->insertItem( ui->remoteControllerList->count(), cntrl->getName() );
    }
}

void settingsDialog::on_remove_controller_clicked()
{
    // get controller
    foreach(remoteControl* control, remoteControl::getAllControls())
        if(control->getName() == ui->remoteControllerList->currentItem()->text())
        {
            ui->remoteControllerList->takeItem( ui->remoteControllerList->currentRow() );
            control->disconnect();
            delete control;
            return; // only remove a single item...
        }
}

void settingsDialog::on_remoteControllerList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if(!current)
        return;

    // get controller
    foreach(remoteControl* controlIt, remoteControl::getAllControls())
        if(controlIt->getName() == current->text())
            selected_controller = controlIt;

    // empty the list...
    ui->remoteController_channels->clear();

    // set the configured channels for the controller up
    if(selected_controller)
    {
        foreach(remoteControlChannel* chan, selected_controller->channels)
            ui->remoteController_channels->insertItem( ui->remoteControllerList->count(), chan->getName() );
    }
}

void settingsDialog::on_attachToMixer_clicked()
{
    // abort if no controller is selected..
    if(!selected_controller || !selected_controlChannel)
    {
        qWarning("could not attach to channel due to missing selection...");
        return;
    }

    bool ok;
    QStringList mixers;
    foreach(mixerChannel* channel, mixerChannelManager::allChannels)
        mixers.append( channel->getName() );

    QString chan = QInputDialog::getItem( this, tr("Attach to mixer"), tr("Please select the mixer to attach to"), mixers, 0, false, &ok);
    if(!ok)
        return;

    foreach(mixerChannel* channel, mixerChannelManager::allChannels)
        if( chan == channel->getName() )
            selected_controlChannel->associateToChannel( channel );
}

void settingsDialog::on_remoteController_channels_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    // assign values to onject
    if(previous && selected_controlChannel && selected_controller)
    {
        remoteControl_MIDI* midiContr = dynamic_cast<remoteControl_MIDI*>(selected_controller);
        if(!midiContr)
        {
            qWarning("only MIDI controller supported right now...");
            return;
        }
        remoteControl_MIDI::channelConfig config = midiContr->getChannelConfig( selected_controlChannel->getUuid() );
        config[remoteControlChannel::event_volumeFader] = ui->volumeMIDIId->value();
        config[remoteControlChannel::event_balanceFader] = ui->balanceMIDIId->value();
        config[remoteControlChannel::event_playButton] = ui->playMIDIId->value();
        config[remoteControlChannel::event_stopButton] = ui->stopMIDIId->value();
        config[remoteControlChannel::event_pauseButton] = ui->pauseMIDIId->value();
        config[remoteControlChannel::event_queueButton] = ui->cueMIDIId->value();
        config[remoteControlChannel::event_repeatButton] = ui->repeatMIDIId->value();
        config[remoteControlChannel::event_openButton] = ui->openMIDIId->value();
        config[remoteControlChannel::event_muteButton] = ui->muteMIDIId->value();
        midiContr->setChannelConfig(selected_controlChannel->getUuid(), config);
    }

    foreach(remoteControlChannel* chan, selected_controller->channels)
        if( chan->getName() == current->text() )
            selected_controlChannel = chan;

    if(!selected_controlChannel)
    {
        qWarning("could not find control channel...");
        return;
    }

    remoteControl_MIDI* midiContr = dynamic_cast<remoteControl_MIDI*>(selected_controller);
    if(!midiContr)
    {
        qWarning("only MIDI controller supported right now...");
        return;
    }

    // set data..
    remoteControl_MIDI::channelConfig config = midiContr->getChannelConfig( selected_controlChannel->getUuid() );
    ui->volumeMIDIId->setValue( config[remoteControlChannel::event_volumeFader].toInt() );
    ui->balanceMIDIId->setValue( config[remoteControlChannel::event_balanceFader].toInt() );
    ui->playMIDIId->setValue( config[remoteControlChannel::event_playButton].toInt() );
    ui->stopMIDIId->setValue( config[remoteControlChannel::event_stopButton].toInt() );
    ui->pauseMIDIId->setValue( config[remoteControlChannel::event_pauseButton].toInt() );
    ui->cueMIDIId->setValue( config[remoteControlChannel::event_queueButton].toInt() );
    ui->repeatMIDIId->setValue( config[remoteControlChannel::event_repeatButton].toInt() );
    ui->openMIDIId->setValue( config[remoteControlChannel::event_openButton].toInt() );
    ui->muteMIDIId->setValue( config[remoteControlChannel::event_muteButton].toInt() );

    selected_controller->saveConfig();
}

void settingsDialog::on_add_controller_channel_clicked()
{
    bool ok;
    QString name = QInputDialog::getText( this, tr("Name of new channel"), tr("Please enter the name for the new channel"), QLineEdit::Normal, tr("New Channel"), &ok);
    if(!ok)
        return;
    remoteControlChannel* newChan = selected_controller->createChannel();
    newChan->setName( name );
    selected_controller->saveConfig();
    ui->remoteController_channels->insertItem( ui->remoteControllerList->count(), newChan->getName() );
}

void settingsDialog::on_remove_controller_channel_clicked()
{
    selected_controlChannel->disconnect();
    selected_controller->channels.remove( selected_controller->channels.indexOf(selected_controlChannel));
    selected_controller->removeChannelConfig( selected_controlChannel->getUuid() );
    selected_controller->saveConfig();
    delete selected_controlChannel;
    ui->remoteController_channels->takeItem(ui->remoteController_channels->currentRow() );
    selected_controlChannel = NULL;
}

void settingsDialog::on_select_script_clicked()
{
    QString s = QFileDialog::getOpenFileName(
                        this,
                        tr("select meta script"),
                        "");
    if( s.length() >0)
        ui->meta_script->setText( s );
}

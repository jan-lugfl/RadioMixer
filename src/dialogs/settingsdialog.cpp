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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "mixerchannelmanager.h"
#include "listwidgetitemwithid.h"

#include "mixerchannel_jackin.h"
#include "mixerchannel_jackout.h"

#include "mainWindow.h"

#include <QColorDialog>
#include <QInputDialog>

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
            ui->channelSettings->setItemEnabled(2, true);
    if( settingsCache[cur_uuid]["type"] == mixerChannel_ALSA::Type )
            ui->channelSettings->setItemEnabled(3, true);
    if( settingsCache[cur_uuid]["type"] == mixerChannel_jackIn::Type || settingsCache[cur_uuid]["type"] == mixerChannel_jackOut::Type )
            ui->channelSettings->setItemEnabled(4, true);
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

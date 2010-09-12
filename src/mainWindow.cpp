/* $Id:$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
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

#include "mainWindow.h"
#include "ui_radiomixer.h"

#include "mixerguialsamix.h"
#include "mixerguiplayer.h"
#include "mixerguijackport.h"
#include "mixerchannel_jackout.h"
#include "settings.h"

#include "dialogs/settingsdialog.h"
#include "dialogs/aboutdialog.h"

#include <QSettings>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    rm_ui(new Ui::RadioMixer)
{
    // sets up the QTDesigner GUI components...
    rm_ui->setupUi(this);

    // connect aboutQT menu....
    connect( rm_ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()) );

    // connect settings dialog....
    connect( rm_ui->action_Settings, SIGNAL(triggered()), this, SLOT(showSettings()));

    Jack::connect();

    // create one remote control
    rc = new remoteControl_MIDI(this, "BCF2000");

    // create some test playlists..
    playList* pls1 = new playList();
    pls1->rename("Playlist 1");
    pls1->addItem( new playListItem("/mp3/inmyhand.mp3") );
    playList* pls2 = new playList();
    pls2->rename("Playlist 2");
    playList* pls3 = new playList();
    pls3->rename("Playlist 3");

    // load channels from Config...
    foreach(QString itm, Settings::get("channels").toStringList())
    {
        mixerChannel::settingsType settings = Settings::get("channels/"+itm).value<mixerChannel::settingsType>();
        addNewChannel( settings["type"].toString(), QUuid( itm ) );
        mixerChannel* newChan = mixerChannelManager::getChannelByUuid( QUuid(itm) );
        if(newChan)
            newChan->updateSettings( settings );
    }

    // create default layout...
    if( mixerChannelManager::allChannels.isEmpty() )
    {
        mixerChannelManager::getChannelByUuid(addNewChannel( "PLAYER" ))->setName(tr("Player 1"));
        mixerChannelManager::getChannelByUuid(addNewChannel( "PLAYER" ))->setName(tr("Player 2"));
        mixerChannelManager::getChannelByUuid(addNewChannel( "PLAYER" ))->setName(tr("Player 3"));
        mixerChannelManager::getChannelByUuid(addNewChannel( "PLAYER" ))->setName(tr("Player 4"));
        mixerChannelManager::getChannelByUuid(addNewChannel( "PLAYER" ))->setName(tr("Player 5"));
#ifdef HAVE_JACK
        mixerChannelManager::getChannelByUuid(addNewChannel( "JACKOUT" ))->setName(tr("Master (Jack out)"));
#endif

    }

    // start the mixer thread...
    mixer = new channelMixer();
    mixer->start();
}

mainWindow::~mainWindow()
{
    Settings::sync();
    delete rm_ui;
}

void mainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        rm_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void mainWindow::aboutQt()
{
    emit( showAboutQt() );
}

void mainWindow::showSettings()
{
    settingsDialog* win = new settingsDialog( this );
    win->exec();

    // apply channel order..
    // first remove all items...
    QMap<QUuid,QLayoutItem*> temp;
    while( rm_ui->horizontalLayout->count() )
    {
        QLayoutItem* item = rm_ui->horizontalLayout->takeAt(0);
        mixerGUI* mixer = dynamic_cast<mixerGUI*>( item->widget() );
        temp[mixer->getUuid()] = item;
    }

    QStringList channelOrder;
    foreach( QUuid item, win->channels )
    {
        rm_ui->horizontalLayout->addItem( temp[item] );
        channelOrder.append( item.toString() );
        temp.remove( item );
    }
    Settings::set("channels", channelOrder);

    // destroy all remaining channels in temp as they seems to have been deleted...
    foreach( QUuid item, temp.keys() )
    {
        // delete channels GUI
        temp[item]->widget()->disconnect();
        delete temp[item]->widget();
        temp.remove( item );

        // delete channel too..
        foreach( mixerChannel* channel, mixerChannelManager::allChannels )
            if(channel->getUuid() == item)
            {
                channel->disconnect();
                delete channel;
            }
    }
}

QUuid mainWindow::addNewChannel( QString type, QUuid uuid )
{
    mixerChannel* newChan;
    mixerGUI* newGui;
    if(type == mixerChannel_ALSA::Type)
    {
        newChan = new mixerChannel_ALSA( "", uuid );
        newGui = new mixerGuiAlsaMix();
    }
    else if(type == mixerChannel_filePlayer::Type)
    {
        newChan = new mixerChannel_filePlayer( "", uuid );
        newGui = new mixerGuiPlayer();
    }
    else if(type == mixerChannel_jackIn::Type)
    {
        newChan = new mixerChannel_jackIn( "", uuid );
        newGui = new mixerGuiJackport();
    }
    else if(type == mixerChannel_jackOut::Type)
    {
        newChan = new mixerChannel_jackOut( "", uuid );
        newGui = new mixerGuiJackport();
    }
    else
    {
        qWarning("unknown mixerchannel");
        return 0;
    }
    newGui->associateToChannel( newChan );
    rm_ui->horizontalLayout->addWidget( newGui );

    return newChan->getUuid();
}

void mainWindow::on_action_About_triggered()
{
    aboutDialog win(this);
    win.exec();
}

void mainWindow::on_action_Playlist_Manager_triggered()
{
    playlistDlg = new playlistDialog(this);
    playlistDlg->show();
}

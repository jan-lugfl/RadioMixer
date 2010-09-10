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

    // load channels from Config...
    foreach(QString itm, Settings::get("channels").toStringList())
    {
        qWarning( itm );
        mixerChannel::settingsType settings = Settings::get("channels/"+itm).value<mixerChannel::settingsType>();
        addNewChannel( settings["type"].toString(), QUuid( itm ) );
        mixerChannel* newChan = mixerChannelManager::getChannelByUuid( QUuid(itm) );
        if(newChan)
            newChan->updateSettings( settings );
    }
/*
    dummy1_ = new mixerChannel_ALSA();
    dummy2_ = new mixerChannel_filePlayer("test123");
    dummy3_ = new mixerChannel_filePlayer("ch4");
    dummy4_ = new mixerChannel_filePlayer("ch5");
    dummy6_ = new mixerChannel_jackIn("ch6");
    dummy_out = new mixerChannel_jackOut("master");

    dummy = new mixerGuiAlsaMix();
    dummy->associateToChannel( dummy1_ );
    rc->channels[0]->associateToChannel( dummy1_);
    rm_ui->horizontalLayout->addWidget( dummy );

    dummy2 = new mixerGuiPlayer();
    dummy2->associateToChannel( dummy2_ );
    rc->channels[1]->associateToChannel( dummy2_);
    rm_ui->horizontalLayout->addWidget( dummy2 );

    dummy3 = new mixerGuiPlayer();
    dummy3->associateToChannel( dummy2_ );
    rc->channels[2]->associateToChannel( dummy2_);
    rm_ui->horizontalLayout->addWidget( dummy3 );

    dummy4 = new mixerGuiPlayer();
    dummy4->associateToChannel( dummy3_ );
    rc->channels[3]->associateToChannel( dummy3_);
    rm_ui->horizontalLayout->addWidget( dummy4 );

    dummy5 = new mixerGuiPlayer();
    dummy5->associateToChannel( dummy4_ );
    rc->channels[4]->associateToChannel( dummy4_);
    rm_ui->horizontalLayout->addWidget( dummy5 );

    dummy6 = new mixerGuiJackport();
    dummy6->associateToChannel( dummy6_ );
    rc->channels[5]->associateToChannel( dummy6_);
    rm_ui->horizontalLayout->addWidget( dummy6 );

    dummy7 = new mixerGuiJackport();
    dummy7->associateToChannel( dummy_out );
    rc->channels[7]->associateToChannel( dummy_out );
    rm_ui->horizontalLayout->addWidget( dummy7 );
*/
/*
    dummy8 = new mixerGuiJackport();
    rm_ui->horizontalLayout->addWidget( dummy8 );
*/
/*    // read config and set up last saved state
    QSettings* config = new QSettings();
    if( config->readBoolEntry( "/radiomixer/meta/enable", FALSE ) )
        meta.enable();
    else
        meta.disable();
    switch( config->readNumEntry( "/radiomixer/meta/mode", 0 ) )
    {
        case 0:
            meta.setNormalMode();
            break;
        case 1:
            meta.setStationMode( config->readEntry( "/radiomixer/meta/stationID", "" ) );
            break;
    }
    meta.setMetaMode( config->readNumEntry( "/radiomixer/meta/metaMode", 1 ));

    if( config->readEntry( "/radiomixer/sound/mode", "jack" ) == "jack" )
    {
#ifdef HAVE_JACK
        player = new soundPlayerJack();
#else
        qWarning( tr("JackD is not supported by this system..") );
        config->writeEntry( "/radiomixer/sound/mode", "alsa" );
#endif
    }
    if( config->readEntry( "/radiomixer/sound/mode", "jack" ) == "alsa" )
    {
#ifdef HAVE_ALSA
        player = new soundPlayerALSA();
#else
        qWarning( tr("ALSA is not supported by this system..") );
#endif
    }

    for(int i=1;i<=config->readNumEntry( "/radiomixer/General/numChannels", 8 ) ;i++)
    {
        QString type = config->readEntry( "/radiomixer/channel_"+QString::number(i)+"/type", "PLAYER" );
        if( type == "PLAYER" )
            addNewFilePlayer( );
#ifdef HAVE_ALSA
        else if( type == "ALSAMIX" )
            addNewMixerChannel( );
#endif
#ifdef HAVE_JACK
        else if( type == "JACK" && (config->readEntry( "/radiomixer/sound/mode", "jack" ) == "jack" ) )
            addNewJackChannel( config->readEntry( "/radiomixer/channel_"+QString::number(i)+"/name", "JACKPORT_"+QString::number(i+1) ) );
#endif
    }
    delete config;
    */

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

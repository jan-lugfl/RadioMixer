/* $Id:$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2009 by Jan Boysen                                *
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
#include "soundplayerjack.h"

#include <QSettings>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    rm_ui(new Ui::RadioMixer)
{
    // sets up the QTDesigner GUI components...
    rm_ui->setupUi(this);

    // connect aboutQT menu....
    connect( rm_ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()) );

    dummy2_ = new mixerChannel_filePlayer("test123");
    dummy3_ = new mixerChannel_filePlayer("ch4");
    dummy4_ = new mixerChannel_filePlayer("ch5");

    dummy = new mixerGuiAlsaMix();
    rm_ui->horizontalLayout->addWidget( dummy );

    dummy2 = new mixerGuiPlayer();
    dummy2->associateToChannel( dummy2_ );
    rm_ui->horizontalLayout->addWidget( dummy2 );

    dummy3 = new mixerGuiPlayer();
    dummy3->associateToChannel( dummy2_ );
    rm_ui->horizontalLayout->addWidget( dummy3 );

    dummy4 = new mixerGuiPlayer();
    dummy4->associateToChannel( dummy3_ );
    rm_ui->horizontalLayout->addWidget( dummy4 );

    dummy5 = new mixerGuiPlayer();
    dummy5->associateToChannel( dummy4_ );
    rm_ui->horizontalLayout->addWidget( dummy5 );

    dummy6 = new mixerGuiPlayer();
    rm_ui->horizontalLayout->addWidget( dummy6 );

    dummy7 = new mixerGuiPlayer();
    rm_ui->horizontalLayout->addWidget( dummy7 );

    dummy8 = new mixerGuiPlayer();
    rm_ui->horizontalLayout->addWidget( dummy8 );

    player = new soundPlayerJack();
    player->open("RadioMixer");

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
}

mainWindow::~mainWindow()
{
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

void mainWindow::addNewFilePlayer()
{
//        mixerGuiPlayer* channel = new mixerGuiPlayer( playerGuis.count()+1, player, channelLayout, "channelXY");
//        playerGuis.append( channel );
//        reorderChannels();
//	connect( channel, SIGNAL(getNextTrack( unsigned int )), playListMgr, SLOT(cueNewTrack( unsigned int ) ));
//	connect( playListMgr, SIGNAL(cueTrack( unsigned int, playListItem* )), channel, SLOT(cueTrack( unsigned int, playListItem* ) ));
#ifdef ENABLE_HWMIXER
//	connect( miPu, SIGNAL( butPres( int, int )), channel, SLOT( buttonPressed( int, int ) ) );
//	connect( miPu, SIGNAL( sliderMove( int, int )), channel, SLOT( setSlider( int, int ) ) );
#endif
//	connect( channel, SIGNAL(newMeta( metaTag )), &meta, SLOT( setMeta( metaTag ) ) );

//        playListManager::filePlayer playerInfo = { channel->getPlayerID(), channel->getName(), channel->getColor() };
//        playListMgr->addPlayer( playerInfo );
}

void mainWindow::addNewMixerChannel()
{

}

void mainWindow::addNewJackChannel( QString chName )
{

}

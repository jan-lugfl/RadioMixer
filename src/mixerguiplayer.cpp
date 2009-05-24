/* $Id$ */
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
#include "mixerguiplayer.h"
#include <QFileDialog>
#include <QDropEvent>
#include <QDragEnterEvent>

mixerGuiPlayer::mixerGuiPlayer( QWidget* parent , const char* name , Qt::WFlags fl )
 : mixerGUI( parent, name, fl)
{
//	QSettings* config = new QSettings;

        levelMeterLeft = new vuMeter( this, "levelMeterLeft");
	levelMeterLeft->setGeometry( QRect( 5, 60, 11, 321 ) );
	levelMeterLeft->setPaletteBackgroundColor( paletteBackgroundColor () );
	
	levelMeterRight = new vuMeter( this, "levelMeterRight");
	levelMeterRight->setGeometry( QRect( 19, 60, 11, 321 ) );
	levelMeterRight->setPaletteBackgroundColor( paletteBackgroundColor () );

        meterLayout = new QGridLayout();
        meterLayout->addWidget( levelMeterLeft, 1, 1 );
        meterLayout->addWidget( levelMeterRight, 1, 2 );
        layout->addLayout( meterLayout, 3, 1, 2, 1 );

        player = new playerChannelFile(this, "soundPlayer");
        mixer = player;
//	soundplayer->addChannel(player);

	//connect some Signal of the Player....
/*	connect( vuSlider, SIGNAL(valueChanged( int )), player, SLOT(setVolume( int )) );
	connect( player, SIGNAL( nameChanged( QString ) ), this, SLOT( changeName( QString) ) );
	connect( player, SIGNAL( refreshed() ) , this, SLOT( refresh() ) );
	connect( player, SIGNAL(newMeta( metaTag )), this, SLOT(setMeta( metaTag )));
	connect( player, SIGNAL(cued( metaTag )), this, SLOT(cued( metaTag )));
	connect( player, SIGNAL( stopped()), levelMeterLeft, SLOT( reset()));
	connect( player, SIGNAL( paused()), levelMeterLeft, SLOT( reset()));
	connect( player, SIGNAL( stopped()), levelMeterRight, SLOT( reset()));
	connect( player, SIGNAL( paused()), levelMeterRight, SLOT( reset()));
*/
        playButton = new blinkButton( this, "playButton" );
	playButton->setActivatedColor( QColor(150, 255, 150) );
        playButton->setIcon(QIcon(":/buttons/play_icon"));
        actionButtons->addWidget( playButton );
//	connect( playButton, SIGNAL(clicked()), player, SLOT(play()) );

	stopButton = new glowButton( this, "stopButton" );
	stopButton->setActivatedColor( QColor(255, 150, 150) );
        stopButton->setIcon(QIcon(":/buttons/stop_icon"));
        actionButtons->addWidget( stopButton );
//	connect( stopButton, SIGNAL(clicked()), player, SLOT(stop()) );

	pauseButton = new blinkButton( this, "pauseButton" );
	pauseButton->setActivatedColor( QColor(150, 150, 255) );
        pauseButton->setIcon(QIcon(":/buttons/pause_icon"));
        actionButtons->addWidget( pauseButton );
//	connect( pauseButton, SIGNAL(clicked()), player, SLOT(pause()) );

	cueButton = new blinkButton( this, "cueButton" );
	cueButton->setActivatedColor( QColor(255, 180, 100) );
        cueButton->setIcon(QIcon(":/buttons/cue_icon"));
        actionButtons->addWidget( cueButton );
//	connect( cueButton, SIGNAL(clicked()), this, SLOT(cueNewTrack()) );
	
	openButton = new blinkButton( this, "openButton" );
	openButton->setGeometry( QRect( 60, 250, 39, 26 ) );
        toolButtons->addWidget( openButton, 1, 1 );
        connect( openButton, SIGNAL(clicked()), this, SLOT(fileOpen()) );

	tDisplay = new timeDisplay( this, "timeDisplay");
        tDisplay->setMaximumHeight( 23 );
        layout->addWidget( tDisplay, 2, 1, 1, -1 );

	loopButton = new glowButton( this, "loopButton" );
	loopButton->setActivatedColor( QColor(255, 240, 0) );
        actionButtons->addWidget( loopButton );
//	connect( loopButton, SIGNAL(clicked()), player, SLOT(toggleLoop()) );

/*	player->setName( config->readEntry( "/radiomixer/channel_"+QString::number( channelID )+"/name", tr("Kanal")+" "+QString::number(channelID) ) );

	if( config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/autoRecue", 0 ) )
		connect( player, SIGNAL(trackEnded()), this, SLOT(cueNewTrack()) );

	delete config;
*/
        languageChange();

}


mixerGuiPlayer::~mixerGuiPlayer()
{
}

void mixerGuiPlayer::buttonBlinker( )
{
    /*
	playButton->setState( player->isPlaying() );
	stopButton->setState( player->isStopped() );
	pauseButton->setState( player->isPaused() );
	cueButton->setState( player->isCued() );
	loopButton->setState( player->isLooping() );
	
	if( player->isFileOpen())
	{
		tDisplay->setPosition_Frames( player->getPlayedFrames() );
		tDisplay->setTotal_Frames( player->getTotalFrames() );
		tDisplay->setPreroll_Frames( player->getPrerollFrames() );
		tDisplay->setSamplerate( player->getSmplRate() );
	}

	if(mixer->isPlaying())
	{
		levelMeterLeft->setLevel( mixer->getLevelMeterLeft() );
		levelMeterRight->setLevel( mixer->getLevelMeterRight() );
	}

        mixerGUI::buttonBlinker();
        */
}

void mixerGuiPlayer::cueNewTrack( )
{
//	if( player->isStopped() )
//		emit getNextTrack( channelID );
}

void mixerGuiPlayer::buttonPressed( int hwChannel, int button )
{
	if( hwChannel == this->hwChannel )
	{
                mixerGUI::buttonPressed( hwChannel, button );
		switch( button )
		{
			case 0x10 :
				cueNewTrack();
				break;
			case 0x20 :
//				player->play();
				break;
			case 0x40 :
//				player->pause();
				break;
			case 0x80 :
//				player->stop();
				break;
		}
	}
}

void mixerGuiPlayer::languageChange()
{
    mixerGUI::languageChange();
//    chName->setText( player->getName() );
    playButton->setText( tr( "Play" ) );
    stopButton->setText( tr( "Stop" ) );
    pauseButton->setText( tr( "Paus" ) );
    cueButton->setText( tr( "CUE" ) );
    openButton->setText( tr( "Open" ) );
    loopButton->setText( QChar(0x221e) );
}

void mixerGuiPlayer::fileOpen( )
{
   QString extensions = ""
#ifdef HAVE_VORBIS
   +QString(" *.ogg")
#endif
#ifdef HAVE_MAD
   +QString(" *.mp2 *.mp3")
#endif
   ;
   QString fileTypes = tr("all supported soundfiles (%1)").arg(extensions)
#ifdef HAVE_VORBIS
   +QString(";;OGG/Vorbis (*.ogg)")
#endif
#ifdef HAVE_MAD
   +QString(";;MPEG-1 Layer III (*.mp2 *.mp3)")
#endif
   ;
   QString s = QFileDialog::getOpenFileName(
                    this,
                    tr("load file dialog"),
                    "",
                    fileTypes);
    if( s.length() >0)
        emit openFile( new playListItem(s) );
}

void mixerGuiPlayer::showPrefs( )
{
    /*
        mixerGUI::createPrefDlg( );

	QCheckBox* autoRecue = new QCheckBox( prefDlg->DynamicBox, "autoRecue" );
	autoRecue->setText( tr("Auto Recue ?") );
	autoRecue->setGeometry( QRect( 10, 90, 250, 25) );

	if( config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/autoRecue", 0 ) )
		autoRecue->setChecked(1);

        if( mixerGUI::execPrefDlg() == QDialog::Accepted)
	{	
		player->setName( prefDlg->EditName->text());
		if( autoRecue->isChecked() )
		{
			config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/autoRecue", 1 );
			connect( player, SIGNAL(trackEnded()), this, SLOT(cueNewTrack()) );
		}
		else
		{
			config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/autoRecue", 0 );
			disconnect( player, SIGNAL(trackEnded()), this, SLOT(cueNewTrack()) );
		}
		levelMeterLeft->setPaletteBackgroundColor( paletteBackgroundColor () );
		levelMeterRight->setPaletteBackgroundColor( paletteBackgroundColor () );
	}
        mixerGUI::finishPrefDlg( );
        */
}

void mixerGuiPlayer::setMeta( metaTag meta )
{
	if( refreshMeta )
		switch( metaMode )
		{
			case 0:
				emit( newMeta( meta ) );
				break;
			case 1:
				emit( newMeta( metaTag( staticMetaText )) );
				break;
		}
}

QString mixerGuiPlayer::getType( )
{
	return "PLAYER";
}

void mixerGuiPlayer::cued( metaTag meta )
{
	emit( onCue( meta, getName() ) );
}

void mixerGuiPlayer::cueTrack( unsigned int playerID, playListItem * song )
{
	if( this->channelID == playerID )
	{
//		player->open( song );
	}
}

void mixerGuiPlayer::dragEnterEvent(QDragEnterEvent * evt)
{
	if( evt->provides("application/x-radiomixer-playlist") || evt->provides("application/x-radiomixer-playlistitem") || evt->provides("text/uri-list"))
		evt->accept();
}

void mixerGuiPlayer::dropEvent(QDropEvent * evt)
{
    /*
	playListItem* playListEntry = 0;
	if( evt->provides("application/x-radiomixer-playlistitem") )
	{
		QDomDocument doc;
		doc.setContent( evt->encodedData("application/x-radiomixer-playlistitem") );
		if( !doc.isDocument() )
		{
			qWarning( QObject::tr("Received an invalid Document") );
			return;
		}

		QDomElement item = doc.documentElement();
		if( item.tagName() == "playListEntry" )
		{
			playListEntry = new playListItem( item.attribute( "file" ));
			player->open( playListEntry );
		}
#ifdef ENABLE_SONGDB
		else if( item.tagName() == "songdbEntry" )
		{
			playListItemSongDB* entry = new playListItemSongDB( );
			connect( entry, SIGNAL( ready( playListItem*)), player, SLOT(open( playListItem*)) );
			entry->load( item.attribute("id").toInt() );
			playListEntry = entry;
		}
#endif
	}else if( evt->provides("application/x-radiomixer-playlist") && evt->source() ) // TODO add support for playlist that are not yet loaded into the playlist manager
	{
		Q3ListView* sender = dynamic_cast<Q3ListView*>(evt->source());
		playList* plst = dynamic_cast<playList*>(sender->selectedItem());
		if( plst )
			plst->cueInChannel( channelID );
		cueNewTrack( );
	}else if( evt->provides("text/uri-list") )
	{
		if( Q3UriDrag::canDecode( evt ) )
		{
			QStringList uriList;
			Q3UriDrag::decodeLocalFiles( evt, uriList );
			if( !uriList.first().isEmpty() && ( uriList.first().contains(".ogg" ) || uriList.first().contains(".mp3" ) ) )
			{
				playListEntry = new playListItem(  uriList.first() );
				player->open( playListEntry );
			}
		}
	}
        */
}

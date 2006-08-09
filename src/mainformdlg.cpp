/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005, 2006 by Jan Boysen                                *
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


#include "mainformdlg.h"

mainFormDlg::mainFormDlg(QWidget* parent, const char* name, WFlags fl)
    : RadioMixer(parent,name,fl)
{
	//Setup statusBar
	statusBar()->setSizeGripEnabled( false );
	soundStatus = new QLabel( statusBar(), "SoundStatus");
	soundStatus->setText( tr("Sounddevice status: %1").arg(tr("disconnected")) );
	soundStatus->setPaletteBackgroundColor(red.dark(120));
	statusBar()->addWidget( soundStatus, 0, true );

#ifdef ENABLE_HWMIXER
	mixerStatus = new QLabel( statusBar(), "MixerStatus");
	mixerStatus->setText(tr("Mixer device status: %1").arg(tr("disconnected")));
	mixerStatus->setPaletteBackgroundColor(red.dark(120) );
	statusBar()->addWidget( mixerStatus, 0, true );

	miPu = new mipuCom(this);
	connect( miPu, SIGNAL(error(int)), this, SLOT(handleNetworkDeviceErrors(int)));
	connect( miPu, SIGNAL(connected()), this, SLOT(networkDeviceStatusRefresh( )));
	connect( miPu, SIGNAL(connectionClosed()), this, SLOT(networkDeviceStatusRefresh( )));
#else
	// delete the Mixer Menu if no support for it is compiled....
	delete Mixer;
#endif
	
#ifndef ENABLE_SONGDB
	// hide the SongDB Menuitem if no support for it is compiled....
	delete fensterSongDBAction;
#endif

	config = new QSettings();
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
		delete Alsa; // delete Alsa MainMenuItem
#else
		qWarning( tr("JackD is not supported by this system..") );
		config->writeEntry( "/radiomixer/sound/mode", "alsa" );
#endif
	}
	if( config->readEntry( "/radiomixer/sound/mode", "jack" ) == "alsa" )
	{
#ifdef HAVE_ALSA
		player = new soundPlayerALSA();
		delete Jack; // delete Jack MainMenuItem
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

	playListMgr = new playListManager(this);

	delete config;
}

mainFormDlg::~mainFormDlg()
{
	QValueVector<mixerChannelGUI*>::iterator playerIt;
       	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		delete (*playerIt);

#ifdef ENABLE_HWMIXER
	delete miPu;
#endif
	delete player;
}

/*$SPECIALIZATION$*/
void mainFormDlg::showPreferences()
{
	config = new QSettings();

	prefDlg* preferencesDialog  = new prefDlg(this);

	if( config->readEntry( "/radiomixer/sound/mode", "jack" ) == "jack")
		preferencesDialog->jackMode->setChecked(true);
	else if( config->readEntry( "/radiomixer/sound/mode", "jack" ) == "alsa" )
		preferencesDialog->alsaMode->setChecked(true);

	// set ALSA Device
	for( int i=0;i<preferencesDialog->sndDevice->count();i++)
	{
		if( config->readEntry( "/radiomixer/sound/alsaDevice", "default" ) == preferencesDialog->sndDevice->text(i).section(" ",0,0))
		{
			preferencesDialog->sndDevice->setCurrentItem(i);
			break;
		}
	}

#ifdef ENABLE_HWMIXER
	// set Mixerdevice Options
	preferencesDialog->Input_IP->setText( config->readEntry( "/radiomixer/network/ip", "192.168.12.30" ) );
	preferencesDialog->Input_Device_Port->setText( QString::number( config->readNumEntry( "/radiomixer/network/port", 6231 )) );
#else
	preferencesDialog->Einstellungen->removePage( preferencesDialog->Einstellungen->page( 1 ) );
#endif

#ifdef ENABLE_SONGDB
	// set SongDB Options
	preferencesDialog->songDBHostname->setText( config->readEntry( "/radiomixer/network/songDBHostname", "localhost" ) );	
	preferencesDialog->songDBScriptname->setText( config->readEntry( "/radiomixer/network/songDBScriptname", "/xmlctrl.pl" ) );
	preferencesDialog->songDBBasePath->setText( config->readEntry( "/radiomixer/network/songDBBasePath", "/songs/" ) );
#else
	preferencesDialog->songDBBox->setDisabled(TRUE);
#endif

	preferencesDialog->metaEnable->setChecked( config->readBoolEntry( "/radiomixer/meta/enable", FALSE ) );

	switch( config->readNumEntry( "/radiomixer/meta/mode", 0 ) )
	{
		case 0:
			preferencesDialog->metaNormalMode->setChecked(TRUE);
			break;
		case 1:
			preferencesDialog->metaStationMode->setChecked(TRUE);
			preferencesDialog->metaNormalMode->setChecked(FALSE);
			break;
	}
	switch( config->readNumEntry( "/radiomixer/meta/metaMode", 1 ) )
	{
		case 1:
			preferencesDialog->metaFormatIces2->setChecked(TRUE);
			break;
		case 2:
			preferencesDialog->metaFormatOddcast3->setChecked(TRUE);
			preferencesDialog->metaFormatIces2->setChecked(FALSE);
			break;
	}
	preferencesDialog->metaStationText->setText( config->readEntry( "/radiomixer/meta/stationID", "" ) );
	preferencesDialog->metaPath->setText( config->readEntry( "/radiomixer/meta/file", "/tmp/radiomixer.current" ) );
	preferencesDialog->metaCommand->setText( config->readEntry( "/radiomixer/meta/command", "/bin/killall -sUSR1 ices" ) );

	// save Settings if OK is clicked....
	if( preferencesDialog->exec() == QDialog::Accepted )
	{
		if( preferencesDialog->jackMode->isChecked() )
			config->writeEntry( "/radiomixer/sound/mode", "jack" );
		else if( preferencesDialog->alsaMode->isChecked() )
		{
			config->writeEntry( "/radiomixer/sound/mode", "alsa" );
			config->writeEntry( "/radiomixer/sound/alsaDevice", preferencesDialog->sndDevice->currentText().section(" ",0,0) );
		}

#ifdef ENABLE_HWMIXER
		// set Mixerdevice Options
		config->writeEntry( "/radiomixer/network/ip", preferencesDialog->Input_IP->text() );
		config->writeEntry( "/radiomixer/network/port", preferencesDialog->Input_Device_Port->text().toInt() );
#endif

#ifdef ENABLE_SONGDB
		// set SongDB Options
		config->writeEntry( "/radiomixer/network/songDBHostname", preferencesDialog->songDBHostname->text() );
		config->writeEntry( "/radiomixer/network/songDBScriptname", preferencesDialog->songDBScriptname->text() );
		config->writeEntry( "/radiomixer/network/songDBBasePath", preferencesDialog->songDBBasePath->text() );
#endif

		config->writeEntry( "/radiomixer/meta/enable", preferencesDialog->metaEnable->isChecked() );
		if( preferencesDialog->metaEnable->isChecked() )
			meta.enable();
		else
			meta.disable();

		if(preferencesDialog->metaNormalMode->isChecked() )
		{
			config->writeEntry( "/radiomixer/meta/mode", 0 );
			meta.setNormalMode();
		}
		else if(preferencesDialog->metaStationMode->isChecked())
		{
			config->writeEntry( "/radiomixer/meta/mode", 1 );
			config->writeEntry( "/radiomixer/meta/stationID", preferencesDialog->metaStationText->text() );
			meta.setStationMode( preferencesDialog->metaStationText->text() );
		}
		config->writeEntry( "/radiomixer/meta/file", preferencesDialog->metaPath->text() );
		config->writeEntry( "/radiomixer/meta/command", preferencesDialog->metaCommand->text() );
		if(preferencesDialog->metaFormatIces2->isChecked() )
		{
			config->writeEntry( "/radiomixer/meta/metaMode", 1 );
			meta.setMetaMode(1);
		}
		else if(preferencesDialog->metaFormatOddcast3->isChecked())
		{
			config->writeEntry( "/radiomixer/meta/metaMode", 2 );
			meta.setMetaMode(2);
		}
	}
	delete preferencesDialog;
	delete config;
}

// Class title is depricated
/*
void mainFormDlg::showPlaylist()
{
	playListDlg* playlist = new playListDlg(this);
	playlist->setPlaylistPointers(&globalPlaylist, &localPlaylistPointers);
	
	playlist->refreshPlaylists();
	
	// Connect Each Player to the Refresh of the Playlist so name Changes of Channes wil be shown in the Playlist to...
	QValueVector<mixerChannelGUI*>::iterator playerIt;
       	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
	{
		connect( (*playerIt), SIGNAL( refreshed() ), playlist, SLOT( refreshPlaylists() ) );
	}
	connect( &globalPlaylist, SIGNAL( changed( ) ), playlist, SLOT( refreshPlaylists() ) );
	
	playlist->show();
}
*/

void mainFormDlg::addNewFilePlayer( )
{
	mixerGuiPlayer* channel = new mixerGuiPlayer( playerGuis.count()+1, player, frame10, "channelXY");
	playerGuis.append( channel );
//	localPlaylistPointers.append( &(channel->player->playList) );
	connect( channel, SIGNAL(global_getNextTrack( QString )), this, SLOT(globalPlaylist_getNextTrack( QString )) );
#ifdef ENABLE_HWMIXER
	connect( miPu, SIGNAL( butPres( int, int )), channel, SLOT( buttonPressed( int, int ) ) );
	connect( miPu, SIGNAL( sliderMove( int, int )), channel, SLOT( setSlider( int, int ) ) );
#endif
	connect( channel, SIGNAL(newMeta( metaTag )), &meta, SLOT( setMeta( metaTag ) ) );
	windowResize();
}

void mainFormDlg::addNewMixerChannel( )
{
#ifdef HAVE_ALSA
	mixerGuiAlsaMix* channel = new mixerGuiAlsaMix( playerGuis.count()+1, frame10, "channelXY");
	playerGuis.append( channel );
#ifdef ENABLE_HWMIXER
	connect( miPu, SIGNAL( butPres( int, int )), channel, SLOT( buttonPressed( int, int ) ) );
	connect( miPu, SIGNAL( sliderMove( int, int )), channel, SLOT( setSlider( int, int ) ) );
#endif //ENABLE_HWMIXER
	windowResize();
#endif//HAVE_ALSA
}

void mainFormDlg::addNewJackChannel( QString chName )
{
#ifdef HAVE_JACK
	mixerGuiJackport* channel = new mixerGuiJackport( dynamic_cast<soundPlayerJack*>(player), playerGuis.count()+1, frame10, chName);
	playerGuis.append( channel );
#ifdef ENABLE_HWMIXER
	connect( miPu, SIGNAL( butPres( int, int )), channel, SLOT( buttonPressed( int, int ) ) );
	connect( miPu, SIGNAL( sliderMove( int, int )), channel, SLOT( setSlider( int, int ) ) );
#endif //ENABLE_HWMIXER
	windowResize();
#endif //HAVE_JACK
}

void mainFormDlg::globalPlaylist_getNextTrack( QString player )
{
// Class title is depricated
//	cueTitle( player, globalPlaylist.getNextTrack() );
}


void mainFormDlg::handleNetworkDeviceErrors( int error )
{
#ifdef ENABLE_HWMIXER
	if( error == mipuCom::ErrPeerProtVersionOld)
		QMessageBox::critical( this, tr("RadioMixer - Networkmixer Device Manager"), tr("The device is using a deprecated version\nof the protocoll...\n\nPlease update the Mixer device.") );
#endif
}

void mainFormDlg::refreshSliders( )
{
#ifdef ENABLE_HWMIXER
	 miPu->getAllChannels();
#endif
}

void mainFormDlg::keyPressEvent( QKeyEvent *k )
{
	/*
	// Handling für Tastatur Modus.....
    switch ( tolower(k->ascii()) ) {
        case '1':                               // Play Channel 1
            playerGuis[0]->player->play();
	    break;
        case '2':                               // Play Channel 2
            playerGuis[1]->player->play();
	    break;
        case '3':                               // Play Channel 3
            playerGuis[2]->player->play();
	    break;
        case '4':                               // Play Channel 4
            playerGuis[3]->player->play();
	    break;
        case '5':                               // Play Channel 5
            playerGuis[4]->player->play();
	    break;
        case '6':                               // Play Channel 6
            playerGuis[5]->player->play();
	    break;
        case '7':                               // Play Channel 7
            playerGuis[6]->player->play();
	    break;
        case '8':                               // Play Channel 8
            playerGuis[7]->player->play();
	    break;
        case 'q':                               // Stop Channel 1
            playerGuis[0]->player->stop();
	    break;
        case 'w':                               // Stop Channel 2
            playerGuis[1]->player->stop();
	    break;
        case 'e':                               // Stop Channel 3
            playerGuis[2]->player->stop();
	    break;
        case 'r':                               // Stop Channel 4
            playerGuis[3]->player->stop();
	    break;
        case 't':                               // Stop Channel 5
            playerGuis[4]->player->stop();
	    break;
        case 'z':                               // Stop Channel 5
            playerGuis[5]->player->stop();
	    break;
        case 'u':                               // Stop Channel 6
            playerGuis[6]->player->stop();
	    break;
        case 'i':                               // Stop Channel 7
            playerGuis[7]->player->stop();
	    break;
        case 'a':                               // Pause Channel 1
            playerGuis[0]->player->pause();
	    break;
        case 's':                               // Pause Channel 2
            playerGuis[1]->player->pause();
	    break;
        case 'd':                               // Pause Channel 3
            playerGuis[2]->player->pause();
	    break;
        case 'f':                               // Pause Channel 4
            playerGuis[3]->player->pause();
	    break;
        case 'g':                               // Pause Channel 5
            playerGuis[4]->player->pause();
	    break;
        case 'h':                               // Pause Channel 6
            playerGuis[5]->player->pause();
	    break;
        case 'j':                               // Pause Channel 7
            playerGuis[6]->player->pause();
	    break;
        case 'k':                               // Pause Channel 8
            playerGuis[7]->player->pause();
	    break;
        case 'y':                               // CUE Channel 1
            playerGuis[0]->cueNewTrack();
	    break;
        case 'x':                               // CUE Channel 2
            playerGuis[1]->cueNewTrack();
	    break;
        case 'c':                               // CUE Channel 3
            playerGuis[2]->cueNewTrack();
	    break;
        case 'v':                               // CUE Channel 4
            playerGuis[3]->cueNewTrack();
	    break;
        case 'b':                               // CUE Channel 5
            playerGuis[4]->cueNewTrack();
	    break;
        case 'n':                               // CUE Channel 6
            playerGuis[5]->cueNewTrack();
	    break;
        case 'm':                               // CUE Channel 7
            playerGuis[6]->cueNewTrack();
	    break;
        case ',':                               // CUE Channel 8
            playerGuis[7]->cueNewTrack();
	    break;   
    }
*/
}

void mainFormDlg::netDeviceSliderLoop()
{
#ifdef ENABLE_HWMIXER
	refTimer.start(100);
	connect( &refTimer, SIGNAL( timeout() ), this, SLOT(refreshSliders()) );
#endif
}

void mainFormDlg::connectNetDevice()
{
#ifdef ENABLE_HWMIXER
	config = new QSettings();
	miPu->open( config->readEntry( "/radiomixer/network/ip", "192.168.12.30" ), config->readNumEntry( "/radiomixer/network/port", 6231 ));
	delete config;
#endif
}

void mainFormDlg::windowResize( )
{
	int minWidth = (playerGuis.count()*120)+10;
	frame10->setFixedWidth( minWidth );
	setFixedWidth( minWidth+20 );
}

void mainFormDlg::showMoveChannel()
{
	ch_move_dlg moveDialog;
	QValueVector<mixerChannelGUI*>::iterator playerIt;
       	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
	{
		moveDialog.ch_source->insertItem( (*playerIt)->getName() );
		moveDialog.ch_destination->insertItem( (*playerIt)->getName() );
	}
	
	if( moveDialog.exec() )
	{
		mixerChannelGUI* tmpPlayer1 = NULL;
		mixerChannelGUI* tmpPlayer2 = NULL;
		for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		{
			if( moveDialog.ch_source->currentText() == (*playerIt)->getName() )
				tmpPlayer1 = (*playerIt);
			if( moveDialog.ch_destination->currentText() == (*playerIt)->getName() )
				tmpPlayer2 = (*playerIt);
		}
		if( tmpPlayer1 && tmpPlayer2)
		{
			if( tmpPlayer1->currentPosition() < tmpPlayer2->currentPosition() )
			{
				for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
					if( (*playerIt)->currentPosition() >tmpPlayer1->currentPosition() && (*playerIt)->currentPosition() < tmpPlayer2->currentPosition() )
						(*playerIt)->changePos( (*playerIt)->currentPosition()-1);
				if( moveDialog.after->isChecked() )
				{
					tmpPlayer1->changePos( tmpPlayer2->currentPosition());
					tmpPlayer2->changePos( tmpPlayer2->currentPosition()-1);
				}else
				{
					tmpPlayer1->changePos( tmpPlayer2->currentPosition() -1);
				}
			}
			else if( tmpPlayer1->currentPosition() > tmpPlayer2->currentPosition() )
			{
				for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
					if( (*playerIt)->currentPosition() < tmpPlayer1->currentPosition() && (*playerIt)->currentPosition() > tmpPlayer2->currentPosition() )
						(*playerIt)->changePos( (*playerIt)->currentPosition()+1);
				if( moveDialog.after->isChecked() )
				{
					tmpPlayer1->changePos( tmpPlayer2->currentPosition()+1);
				}else
				{
					tmpPlayer1->changePos( tmpPlayer2->currentPosition() );
					tmpPlayer2->changePos( tmpPlayer2->currentPosition()+1);
				}
			}
		}
	}
	saveSettings( );
}


void mainFormDlg::showDelChannel()
{
	ch_del_dlg delDialog;
	QValueVector<mixerChannelGUI*>::iterator playerIt;
	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		delDialog.ch_source->insertItem( (*playerIt)->getName() );

	if( delDialog.exec() )
	{
		int oldPos;
		for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
			if( delDialog.ch_source->currentText() == (*playerIt)->getName() )
			{
				oldPos = (*playerIt)->currentPosition();
				delete (*playerIt);
				playerGuis.erase( playerIt );
				break;
			}
		for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		{
			if( (*playerIt)->currentPosition() > oldPos)
				(*playerIt)->changePos( (*playerIt)->currentPosition()-1 );
		}

		saveSettings( );
		windowResize();
	}
}


void mainFormDlg::showAddChannel()
{
	ch_add_dlg addDialog;
	addDialog.ch_add_type->insertItem( tr("MediaPlayer channel") );
#ifdef HAVE_ALSA
	addDialog.ch_add_type->insertItem( tr("ALSA Mixer channel") );
#endif
#ifdef HAVE_JACK
	addDialog.ch_add_type->insertItem( tr("Jack channel") );
#endif

	QValueVector<mixerChannelGUI*>::iterator playerIt;
       	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		addDialog.ch_source->insertItem( (*playerIt)->getName() );

	if( addDialog.exec() )
	{
		frame10->hide();
		switch( addDialog.ch_add_type->currentItem() )
		{
			case 0:
				addNewFilePlayer( );
				break;
#ifdef HAVE_ALSA
			case 1:
				addNewMixerChannel( );
				break;
#endif
#ifdef HAVE_JACK
			case 2:
				addNewJackChannel( addDialog.ch_name->text() );
				break;
#endif
			default:
				addNewFilePlayer( );
		}
		mixerChannelGUI* newChannel = playerGuis[ playerGuis.count()-1 ];
		newChannel->changeName( addDialog.ch_name->text().isEmpty()?tr("unnamed"):addDialog.ch_name->text() );
		int newPos = -1;
		if( addDialog.before->isChecked() )
			newPos = addDialog.ch_source->currentItem()+1;
		else if( addDialog.after->isChecked() )
			newPos = addDialog.ch_source->currentItem()+2;
		newChannel->changePos( newPos );

		QValueVector<mixerChannelGUI*>::iterator playerIt;
	       	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
			if((*playerIt)->currentPosition() >= newPos && (*playerIt) != newChannel )
				(*playerIt)->changePos( (*playerIt)->currentPosition()+1 );

		frame10->show();
		saveSettings();
	}
}

void mainFormDlg::saveSettings( )
{
	config = new QSettings();
	config->writeEntry( "/radiomixer/General/numChannels", (int)playerGuis.count() );
	delete config;

	QValueVector<mixerChannelGUI*>::iterator playerIt;
	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		(*playerIt)->saveSettings();
}

void mainFormDlg::showCuesWindow()
{
	cueListDlg* cueListDialog = new cueListDlg(this);

	QValueVector<mixerChannelGUI*>::iterator playerIt;
	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
	{
		if( (*playerIt)->getType() == "PLAYER" )
		{
			connect( dynamic_cast<mixerGuiPlayer*>(*playerIt), SIGNAL( onCue( metaTag, QString ) ), cueListDialog, SLOT( update( metaTag, QString ) ) );
		}
	}

	cueListDialog->show();
}

void mainFormDlg::showSongDBDialog()
{
#ifdef ENABLE_SONGDB
	songDBDlg* songDBDialog = new songDBDlg(this);
	connect( songDBDialog, SIGNAL( cueTrack( QString, title) ), this, SLOT(cueTitle( QString, title) ) );
	connect( songDBDialog, SIGNAL( addToPlaylist( QString, title) ), this, SLOT(playlistAddTitle( QString, title) ) );

	QValueVector<mixerChannelGUI*>::iterator playerIt;
       	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		if( (*playerIt)->getType() == "PLAYER" )
		{
			songDBDialog->cueChannel->insertItem( (*playerIt)->getName() );
			songDBDialog->playlistChannel->insertItem( (*playerIt)->getName() );
		}
	songDBDialog->show();
#endif
}

// Class title is depricated
/*
void mainFormDlg::cueTitle(QString player, title song )
{
	QValueVector<mixerChannelGUI*>::iterator playerIt;
	for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
		if( (*playerIt)->getName() == player && (*playerIt)->getType() == "PLAYER" )
			(dynamic_cast<mixerGuiPlayer*>(*playerIt))->player->open( song );
}

void mainFormDlg::playlistAddTitle( QString player, title song )
{

	if( player == "Global Playlist")
		globalPlaylist.append( song );
	else
	{
		QValueVector<mixerChannelGUI*>::iterator playerIt;
		for( playerIt = playerGuis.begin(); playerIt != playerGuis.end(); ++playerIt )
			if( (*playerIt)->getName() == player && (*playerIt)->getType() == "PLAYER" )
				(dynamic_cast<mixerGuiPlayer*>(*playerIt))->player->playList.append( song );
	}
}
*/

void mainFormDlg::showAbout()
{
	aboutDialog	aboutDlg;
	aboutDlg.versionInfo->setText(
			QString("<h1><p align=\"center\">Radiomixer ")+
			QString(VERSION_MAJOR)+
			QString(".")+
			QString(VERSION_MINOR)+
			QString(VERSION_SUFFIX)+
			QString("</p></h1>")
		);
	aboutDlg.exec();
}

void mainFormDlg::startALSA()
{
#ifdef HAVE_ALSA
	config = new QSettings();
	player->open( config->readEntry( "/radiomixer/sound/alsaDevice", "default" ) );
	soundStatus->setText(tr("Sound device status: %1 %2").arg("ALSA").arg( player->isConnected()?tr("connected"):tr("disconnected") ));
	soundStatus->setPaletteBackgroundColor( (player->isConnected()?green:red).dark(120) );
	delete config;
#endif
}

void mainFormDlg::disconnectJackd()
{
#ifdef HAVE_JACK
	player->close();
	soundStatus->setText(tr("Sound device status: %1 %2").arg("JACK").arg(tr("disconnected")));
	soundStatus->setPaletteBackgroundColor( red.dark(120) );
#endif
}


void mainFormDlg::connectJackd()
{
#ifdef HAVE_JACK
	player->open("");
	soundStatus->setText(tr("Sound device status: %1 %2").arg("JACK").arg( player->isConnected()?tr("connected"):tr("disconnected") ));
	soundStatus->setPaletteBackgroundColor( (player->isConnected()?green:red).dark(120) );
#endif
}

void mainFormDlg::networkDeviceStatusRefresh( )
{
#ifdef ENABLE_HWMIXER
	mixerStatus->setText(tr("Mixer device status: %1").arg( miPu->isConnected()?tr("connected"):tr("disconnected") ));
	mixerStatus->setPaletteBackgroundColor( (miPu->isConnected()?green:red).dark(120) );
#endif
}

void mainFormDlg::show( )
{
	playListMgr->show();
	RadioMixer::show();
}

void mainFormDlg::showPlaylistManager( bool state )
{
	if( state )
		playListMgr->show();
	else
		playListMgr->hide();
}



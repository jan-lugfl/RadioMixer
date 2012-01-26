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
#include "mixerguiplayer.h"
#include "playlistmanager.h"
#include <QFileDialog>
#include <QDropEvent>
#include <QDragEnterEvent>

mixerGuiPlayer::mixerGuiPlayer( QWidget* parent, Qt::WFlags fl )
 : mixerGUI( parent, fl)
{
        levelMeterLeft = new vuMeter( this );
	levelMeterLeft->setGeometry( QRect( 5, 60, 11, 321 ) );
	
        levelMeterRight = new vuMeter( this );
	levelMeterRight->setGeometry( QRect( 19, 60, 11, 321 ) );

        meterLayout = new QGridLayout();
        meterLayout->addWidget( levelMeterLeft, 1, 1 );
        meterLayout->addWidget( levelMeterRight, 1, 2 );
        layout->addLayout( meterLayout, 3, 1, 2, 1 );

        playButton = new glowButton( this );
	playButton->setActivatedColor( QColor(150, 255, 150) );
        playButton->setIcon(QIcon(":/buttons/play_icon"));
        actionButtons->addWidget( playButton );

        stopButton = new glowButton( this );
	stopButton->setActivatedColor( QColor(255, 150, 150) );
        stopButton->setIcon(QIcon(":/buttons/stop_icon"));
        stopButton->setState( true );
        actionButtons->addWidget( stopButton );

        pauseButton = new blinkButton( this );
	pauseButton->setActivatedColor( QColor(150, 150, 255) );
        pauseButton->setIcon(QIcon(":/buttons/pause_icon"));
        actionButtons->addWidget( pauseButton );

        cueButton = new blinkButton( this );
	cueButton->setActivatedColor( QColor(255, 180, 100) );
        cueButton->setIcon(QIcon(":/buttons/cue_icon"));
        actionButtons->addWidget( cueButton );
	
        openButton = new blinkButton( this );
	openButton->setGeometry( QRect( 60, 250, 39, 26 ) );
        toolButtons->addWidget( openButton, 1, 1 );
        connect( openButton, SIGNAL(clicked()), this, SLOT(fileOpen()) );

        tDisplay = new timeDisplay( this );
        tDisplay->setMaximumHeight( 23 );
        layout->addWidget( tDisplay, 2, 1, 1, -1 );

        loopButton = new glowButton( this );
	loopButton->setActivatedColor( QColor(255, 240, 0) );
        actionButtons->addWidget( loopButton );

        playlist = new QComboBox( this );
        reloadPlaylists();
        connect( playlistManager::getInstance(), SIGNAL(changed()), this, SLOT(reloadPlaylists()));
        connect( playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(changePlaylist(int)));
        layout->addWidget( playlist, 5, 1, 1, -1 );

        languageChange();
}

mixerGuiPlayer::~mixerGuiPlayer()
{
}

void mixerGuiPlayer::cueNewTrack( )
{
//	if( player->isStopped() )
//		emit getNextTrack( channelID );
}

void mixerGuiPlayer::languageChange()
{
    mixerGUI::languageChange();
    openButton->setText( tr( "Open" ) );
    loopButton->setText( QChar(0x221e) );
}

void mixerGuiPlayer::fileOpen( )
{
   QString extensions = ""
#ifdef HAVE_OGG
   +QString(" *.ogg")
#endif
#ifdef HAVE_MAD
   +QString(" *.mp2 *.mp3")
#endif
   ;
   QString fileTypes = tr("all supported soundfiles (%1)").arg(extensions)
#ifdef HAVE_OGG
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

void mixerGuiPlayer::cued( playListItem* track )
{
	tDisplay->setTotal( track->getLength() );
	tDisplay->setPreroll( track->getPreLength() );
}

void mixerGuiPlayer::cueTrack( unsigned int playerID, playListItem * song )
{
//	if( this->channelID == playerID )
//	{
//		player->open( song );
//	}
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

void mixerGuiPlayer::setState( int newState )
{
    playButton->setOff();
    stopButton->setOff();
    pauseButton->setOff();
    cueButton->setOff();

    switch( newState )
    {
    case mixerChannel_filePlayer::Stopped:
           stopButton->setOn();
	   tDisplay->reset();
	   levelMeterLeft->reset();
	   levelMeterRight->reset();
	   break;
    case mixerChannel_filePlayer::Playing:
           playButton->setOn();
           break;
    case mixerChannel_filePlayer::Paused:
           pauseButton->setOn();
           levelMeterLeft->reset();
           levelMeterRight->reset();
           break;
    case mixerChannel_filePlayer::Cued:
           cueButton->setOn();
           break;
    }
}

void mixerGuiPlayer::associateToChannel( mixerChannel* channel )
{
    mixerGUI::associateToChannel( channel );
    connect( this, SIGNAL(openFile(playListItem*)), channel, SLOT(open(playListItem*)) );
    connect( channel, SIGNAL(cued(playListItem*)), this, SLOT(cued(playListItem*)) );
    connect( channel, SIGNAL(stateChanged(int)), this, SLOT(setState(int)) );
    connect( channel, SIGNAL(loopChanged(bool)), loopButton, SLOT(setState(bool)) );
    connect( channel, SIGNAL(positionChanged( QTime )), tDisplay, SLOT(setPosition( QTime )));
    connect( this, SIGNAL(playlistChanged(playList*)), channel, SLOT(attachToPlaylist(playList*)));

    // connect button actions
    connect( playButton, SIGNAL(clicked()), channel, SLOT(play()));
    connect( stopButton, SIGNAL(clicked()), channel, SLOT(stop()));
    connect( pauseButton, SIGNAL(clicked()), channel, SLOT(pause()));
    connect( cueButton, SIGNAL(clicked()), channel, SLOT(cue()));
    connect( loopButton, SIGNAL(clicked()), channel, SLOT(toggleLoop()));

    // connect levelMeters
    connect( channel, SIGNAL(vuMeterChanged_left(float)), levelMeterLeft, SLOT(setLevel(float)) );
    connect( channel, SIGNAL(vuMeterChanged_right(float)), levelMeterRight, SLOT(setLevel(float)) );
}

void mixerGuiPlayer::reloadPlaylists()
{
    QVariant selected = playlist->itemData( playlist->currentIndex() );
    playlist->clear();
    playlist->addItem( tr("-- none --") );

    foreach(playList* plst, playlistManager::getInstance()->getAllPlaylists())
        playlist->addItem( plst->getName(), plst->getUuid().toString() );

    playlist->setCurrentIndex( playlist->findData( selected ));
}

void mixerGuiPlayer::changePlaylist( int index )
{
    if( index <= 0 ) // none ( detach )
        emit( playlistChanged( NULL ) );
    else
    {
        // get selected playlist and attach it...
        playList* plst = playlistManager::getInstance()->getAllPlaylists()[index-1];
        if(plst)
            emit( playlistChanged( plst ) );
    }
}

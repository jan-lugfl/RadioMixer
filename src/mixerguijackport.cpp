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
#include "mixerguijackport.h"

mixerGuiJackport::mixerGuiJackport( QWidget* parent, Qt::WFlags fl )
 : mixerGUI( parent, fl), mode_onAir(false)
{
        muteBut = new glowButton( this );
	muteBut->setActivatedColor( QColor( 180, 50, 50 ) );
	actionButtons->addWidget( muteBut );

        levelMeterLeft = new vuMeter( this );
	levelMeterLeft->setGeometry( QRect( 5, 60, 11, 321 ) );

        levelMeterRight = new vuMeter( this );
	levelMeterRight->setGeometry( QRect( 19, 60, 11, 321 ) );

	meterLayout = new QGridLayout();
        meterLayout->addWidget( levelMeterLeft, 1, 1 );
        meterLayout->addWidget( levelMeterRight, 1, 2 );
	layout->addLayout( meterLayout, 3, 1, 2, 1 );

	languageChange();
}


mixerGuiJackport::~mixerGuiJackport()
{
}

void mixerGuiJackport::languageChange( )
{
        mixerGUI::languageChange();
        if(mode_onAir)
            muteBut->setText( tr( "onAir" ) );
        else
            muteBut->setText( tr( "Mute" ) );
}

void mixerGuiJackport::changeName( QString newName )
{
        mixerGUI::changeName( newName );
//	mixer->setName( newName );
}

QString mixerGuiJackport::getType( )
{
	return "JACK";
}

void mixerGuiJackport::associateToChannel( mixerChannel* channel )
{
    if(channel->getAudioDataType() == mixerChannel::AudioDataOut )
        automationButton->hide();
    mixerGUI::associateToChannel( channel );

    connect( channel, SIGNAL(muteChanged(bool)), this, SLOT(muteChanged(bool)));

    // connect button actions
    connect( muteBut, SIGNAL(clicked()), channel, SLOT(toggleMute()));

    // connect levelMeters
    connect( channel, SIGNAL(vuMeterChanged_left(float)), levelMeterLeft, SLOT(setLevel(float)) );
    connect( channel, SIGNAL(vuMeterChanged_right(float)), levelMeterRight, SLOT(setLevel(float)) );
}

void mixerGuiJackport::muteChanged( bool state )
{
    if(mode_onAir)
        muteBut->setState( !state );
    else
        muteBut->setState( state );
}

void mixerGuiJackport::channelSettingsChanged( mixerChannel::settingsType settings )
{
    setMuteButtonMode( settings["mute_button_mode"].toBool() );
    mixerGUI::channelSettingsChanged( settings );
}

void mixerGuiJackport::setMuteButtonMode( bool mute_button = true )
{
    mode_onAir = !mute_button;
    if(mode_onAir)
    {
        muteBut->setActivatedColor( QColor( 255, 0, 0 ) );
        muteBut->toggle();
    }else
    {
        muteBut->setActivatedColor( QColor( 180, 50, 50 ) );
        muteBut->toggle();
    }
    languageChange();
}

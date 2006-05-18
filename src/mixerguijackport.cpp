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
#include "mixerguijackport.h"

mixerGuiJackport::mixerGuiJackport( soundPlayerJack* jackPlayer, int chID, QWidget* parent, const char* name, WFlags fl )
 : mixerChannelGUI(chID, parent, name, fl), mute(0)
{
	jackMixer = new playerChannelJackport( jackPlayer, name, this );
	mixer = jackMixer;
	connect( vuSlider, SIGNAL(valueChanged( int )), mixer, SLOT(setVolume( int )) );

	muteBut = new glowButton( this, "muteBut" );
	muteBut->setGeometry( QRect( 60, 60, 39, 26 ) );
	muteBut->setActivatedColor( QColor( 180, 50, 50 ) );
	connect( muteBut, SIGNAL(clicked()), this, SLOT( toggleMute() ) );

	connect( trebleSlider, SIGNAL( valueChanged( int )), this, SLOT(setBalance( int )) );
	languageChange();

	levelMeterLeft = new vuMeter( this, "levelMeterLeft");
	levelMeterLeft->setGeometry( QRect( 5, 60, 11, 321 ) );
	levelMeterLeft->setPaletteBackgroundColor( paletteBackgroundColor () );
	
	levelMeterRight = new vuMeter( this, "levelMeterRight");
	levelMeterRight->setGeometry( QRect( 19, 60, 11, 321 ) );
	levelMeterRight->setPaletteBackgroundColor( paletteBackgroundColor () );
}


mixerGuiJackport::~mixerGuiJackport()
{
	disconnect( trebleSlider, SIGNAL( valueChanged( int )), this, SLOT(setBalance( int )) );
	disconnect( vuSlider, SIGNAL(valueChanged( int )), mixer, SLOT(setVolume( int )) );
	disconnect( muteBut, SIGNAL(clicked()), this, SLOT( toggleMute() ) );
	delete mixer;
	delete muteBut;
}

void mixerGuiJackport::languageChange( )
{
	mixerChannelGUI::languageChange();
	muteBut->setText( tr( "Mute" ) );
}

void mixerGuiJackport::toggleMute( )
{
	if( mute )
	{
		muteBut->setOff();
		jackMixer->unMute();
		mute = 0 ;
	}
	else
	{
		muteBut->setOn();
		jackMixer->mute();
		mute = 1;
		levelMeterLeft->reset();
		levelMeterRight->reset();
	}
}

void mixerGuiJackport::changeName( QString newName )
{
	mixerChannelGUI::changeName( newName );
	mixer->setName( newName );
}

QString mixerGuiJackport::getType( )
{
	return "JACK";
}

void mixerGuiJackport::buttonBlinker( )
{
	if( vuSlider->value() < 100 && !mute)
	{
		levelMeterLeft->setLevel( mixer->getLevelMeterLeft() );
		levelMeterRight->setLevel( mixer->getLevelMeterRight() );
	}else{
		levelMeterLeft->reset();
		levelMeterRight->reset();
	}
	mixerChannelGUI::buttonBlinker();
}

void mixerGuiJackport::showPrefs( )
{
	mixerChannelGUI::createPrefDlg( );
	if( mixerChannelGUI::execPrefDlg() == QDialog::Accepted)
	{
		levelMeterLeft->setPaletteBackgroundColor( paletteBackgroundColor () );
		levelMeterRight->setPaletteBackgroundColor( paletteBackgroundColor () );
	}
	mixerChannelGUI::finishPrefDlg( );
}

void mixerGuiJackport::buttonPressed( int hwChannel, int button )
{
	if( hwChannel == this->hwChannel )
	{
		mixerChannelGUI::buttonPressed( hwChannel, button );
		switch( button )
		{
			case 0x10 :
				toggleMute();
				break;
		}
	}
}



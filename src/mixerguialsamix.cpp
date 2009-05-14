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
#include "mixerguialsamix.h"
//Added by qt3to4:
#include <QLabel>

mixerGuiAlsaMix::mixerGuiAlsaMix(  int chID, QWidget* parent , const char* name , Qt::WFlags fl )
 : mixerGUI( chID, parent, name, fl)
{
	alsaMixer = new playerChannelAlsaMix();
	mixer = alsaMixer;
	mute = 0;

	connect( vuSlider, SIGNAL(valueChanged( int )), mixer, SLOT(setVolume( int )) );

	muteBut = new glowButton( this, "muteBut" );
	muteBut->setGeometry( QRect( 60, 60, 39, 26 ) );
	connect( muteBut, SIGNAL(clicked()), this, SLOT( toggleMute() ) );

	languageChange();
}


mixerGuiAlsaMix::~mixerGuiAlsaMix()
{
}

void mixerGuiAlsaMix::toggleMute( )
{
	if( mute )
	{
		muteBut->setOn();
		alsaMixer->unMute();
		mute = 0 ;
	}
	else
	{
		muteBut->setOff();
		alsaMixer->mute();
		mute = 1;
	}
}

void mixerGuiAlsaMix::languageChange( )
{
        mixerGUI::languageChange();
	muteBut->setText( tr( "Mute" ) );
}

void mixerGuiAlsaMix::showPrefs( )
{
        mixerGUI::createPrefDlg( );
	prefDlg->EditName->setText( mixer->getName() );

	QLabel* soundCardLabel = new QLabel( prefDlg->DynamicBox ,  "soundCardLabel" );
	soundCardLabel->setText( tr("Soundkarte:") );
	soundCardLabel->setGeometry( QRect( 10, 20, 200, 20 ) );
	QComboBox* soundCardSelect = new QComboBox( prefDlg->DynamicBox ,  "soundCardSelect" );
	soundCardSelect->setGeometry( QRect( 10, 40, 200, 25 ) );

	soundCardLabel->setText( tr("Mixer Kanal:") );
	soundCardLabel->setGeometry( QRect( 10, 80, 200, 20 ) );
	QComboBox* channelSelect = new QComboBox( prefDlg->DynamicBox ,  "channelSelect" );
	channelSelect->setGeometry( QRect( 10, 100, 200, 25 ) );


        if( mixerGUI::execPrefDlg() == QDialog::Accepted)
	{	
		mixer->setName( prefDlg->EditName->text());
	}
        mixerGUI::finishPrefDlg( );
}

QString mixerGuiAlsaMix::getType( )
{
	return "ALSAMIX";
}

void mixerGuiAlsaMix::buttonPressed( int hwChannel, int button )
{
	if( hwChannel == this->hwChannel )
	{
                mixerGUI::buttonPressed( hwChannel, button );
		switch( button )
		{
			case 0x10 :
				toggleMute();
				break;
		}
	}
}

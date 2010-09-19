/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2009 by Jan Boysen                                 *
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
#include <QLabel>

mixerGuiAlsaMix::mixerGuiAlsaMix( QWidget* parent , Qt::WFlags fl )
 : mixerGUI( parent, fl)
{
        muteBut = new glowButton( this );
	muteBut->setGeometry( QRect( 60, 60, 39, 26 ) );
        actionButtons->addWidget( muteBut );
	languageChange();
}


mixerGuiAlsaMix::~mixerGuiAlsaMix()
{
	delete muteBut;
}

void mixerGuiAlsaMix::languageChange( )
{
        mixerGUI::languageChange();
	muteBut->setText( tr( "Mute" ) );
}

/*void mixerGuiAlsaMix::showPrefs( )
{
        mixerGUI::createPrefDlg( );

	prefDlg->typeDependantSettings->show();
	prefDlg->typeDependantSettings->setTitle( tr("ALSA Settings") );

	QLabel* soundCardLabel = new QLabel( prefDlg->typeDependantSettings ,  "soundCardLabel" );
	soundCardLabel->setText( tr("Device:") );
	soundCardLabel->setGeometry( QRect( 20, 35, 80, 20 ) );
	soundCardSelect = new QComboBox( prefDlg->typeDependantSettings ,  "soundCardSelect" );
	soundCardSelect->setGeometry( QRect( 90, 30, 150, 25 ) );
	connect( soundCardSelect, SIGNAL(activated(int)), this, SLOT(cardChanged(int)));

	QLabel* channelLabel = new QLabel( prefDlg->typeDependantSettings ,  "soundCardLabel" );
	channelLabel->setText( tr("Channel:") );
	channelLabel->setGeometry( QRect( 20, 65, 80, 20 ) );
	channelSelect = new QComboBox( prefDlg->typeDependantSettings ,  "channelSelect" );
	channelSelect->setGeometry( QRect( 90, 60, 150, 25 ) );

	// get ALSA devices...
#ifdef HAVE_ALSA
	int card=-1;
	if (( snd_card_next(&card)) != 0)
		qWarning("getDevices( ): snd_next_card() failed: ");

	while( card > -1)
	{
		char* card_name;

		if ( snd_card_get_name(card, &card_name) != 0)
		{
			qWarning("getDevices( ): snd_card_get_name() failed: ");
			card_name = "Unknown soundcard";
		}
		soundCardSelect->addItem( QString(card_name), QVariant(card) );
		snd_card_next(&card);
	}
#endif
	soundCardSelect->setCurrentIndex( settings["alsa_card"].toInt() );
	cardChanged( settings["alsa_card"].toInt() );

	if( mixerGUI::execPrefDlg() == QDialog::Accepted)
	{
	    settings["alsa_card"] = soundCardSelect->itemData( soundCardSelect->currentIndex() );
	    settings["alsa_channel"] = channelSelect->itemData( channelSelect->currentIndex() );
	    emit( updateSettings(settings) );
	}

	// clean up
	delete soundCardSelect;
	delete channelSelect;
	mixerGUI::finishPrefDlg( );
}
*/

void mixerGuiAlsaMix::cardChanged( int card )
{
#ifdef HAVE_ALSA
	channelSelect->clear();
	// get the cards channels
	snd_mixer_t* alsaDev;
	snd_mixer_open( &alsaDev, 0);
	char device[16];
	sprintf(device, "hw:%i", card);
	snd_mixer_attach(alsaDev, device );
	snd_mixer_selem_register(alsaDev, NULL, NULL);
	snd_mixer_load(alsaDev);  //load up the mixer

	snd_mixer_elem_t* mixerElem = snd_mixer_first_elem(alsaDev);
	while( mixerElem != snd_mixer_last_elem(alsaDev) )
	{
	    if(snd_mixer_selem_has_playback_volume(mixerElem) || snd_mixer_selem_has_capture_volume(mixerElem))
		channelSelect->addItem( QString( snd_mixer_selem_get_name(mixerElem)), QVariant( snd_mixer_selem_get_name(mixerElem)) );
	    mixerElem=snd_mixer_elem_next( mixerElem );
	}

	for(int i=0; i<= channelSelect->count(); i++ )
	    if( channelSelect->itemData(i) == settings["alsa_channel"].toString() )
		channelSelect->setCurrentIndex( i );
#endif
}

QString mixerGuiAlsaMix::getType( )
{
	return "ALSAMIX";
}

void mixerGuiAlsaMix::associateToChannel( mixerChannel* channel )
{
    mixerGUI::associateToChannel( channel );

    connect( channel, SIGNAL(muteChanged(bool)), muteBut, SLOT(setState(bool)));

    // connect button actions
    connect( muteBut, SIGNAL(clicked()), channel, SLOT(toggleMute()));
}

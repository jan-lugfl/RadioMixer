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
#include "mixergui.h"
#include <QFrame>
#include <QLabel>

mixerGUI::mixerGUI( QWidget* parent , const char* name , Qt::WFlags fl )
 : QFrame( parent, name, fl)
{
    layout = new QGridLayout( this );
    actionButtons = new QGridLayout();
    toolButtons = new QGridLayout();

    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Raised );
    setAcceptDrops( TRUE );

//	config = new QSettings( );

        // TODO: hash of name ???
//        channelID = 0;

//	playerPos = config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/position", channelID );

	vuSlider = new QSlider( this, "volumeSlider" );
        vuSlider->setMinimumHeight( 150 );
        vuSlider->setOrientation( Qt::Vertical );
	vuSlider->setTickmarks( QSlider::Both );
	vuSlider->setMinValue( 0 );
	vuSlider->setMaxValue( 100 );
	vuSlider->setLineStep( 1 );
	vuSlider->setPageStep( 10 );
	vuSlider->setValue( 0 );
	vuSlider->setTickInterval( 10 );

	trebleSlider = new QDial( this, "trebleSlider" );
        trebleSlider->setMaxValue( 100 );
	trebleSlider->setMinValue( -100 );
	trebleSlider->setValue( 0 );
	trebleSlider->setLineStep(1);
	trebleSlider->setPageStep(10);

	chName = new QLabel( this, "chName" );
        chName->setMaximumHeight( 23 );
        chName->setFrameShape( QLabel::Panel );
    	chName->setFrameShadow( QLabel::Sunken );
    	chName->setAlignment( int( Qt::AlignCenter ) );
//	changeName( config->readEntry( "/radiomixer/channel_"+QString::number( channelID )+"/name", tr("channel")+" "+QString::number(channelID) ) );
	
	prefButton = new QToolButton( this, "prefButton" );
        connect( prefButton, SIGNAL(clicked()), this, SLOT(showPrefs()) );

	buttonBlinkTimer = new QTimer( this );
	connect( buttonBlinkTimer, SIGNAL(timeout()), this, SLOT(buttonBlinker()));
	buttonBlinkTimer->start( 30, FALSE );

        layout->addWidget( chName, 1, 1, 1, -1 );
        layout->addLayout( actionButtons, 3, 3, Qt::AlignTop );
        layout->addWidget( vuSlider, 3, 2, 2, 1 );
        layout->addLayout( toolButtons, 4, 3, Qt::AlignBottom );

        toolButtons->addWidget( prefButton, 9, 1, Qt::AlignBottom );
        toolButtons->addWidget( trebleSlider, 10, 1, Qt::AlignBottom );

//	languageChange();
	
	// get default colors......
//	int red, green, blue;
//	paletteBackgroundColor().getRgb( &red, &green, &blue );

//	refreshMeta = config->readBoolEntry( "/radiomixer/channel_"+QString::number( channelID )+"/setMeta", FALSE );
//	metaMode = config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/metaMode", 0 );

//	setPaletteBackgroundColor( QColor(
//		config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/color_red", red ),
//		config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/color_green", green ),
//		config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/color_blue", blue )
//	 		) );

//	hwChannel = config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/hwControl", 1 );
//	delete config;
}


mixerGUI::~mixerGUI()
{
	delete trebleSlider;
	delete vuSlider;
	delete prefButton;
	delete chName;
	delete buttonBlinkTimer;
}

void mixerGUI::languageChange()
{
    prefButton->setText( tr( "Pref" ) );
}

void mixerGUI::buttonBlinker()
{
}

void mixerGUI::createPrefDlg( )
{
	config = new QSettings;
	int red, green, blue;
	paletteBackgroundColor().getRgb( &red, &green, &blue );
	
	prefDlg = new chanPrefDlg( this, "ChannelPrefDlg");

	prefDlg->EditName->setText( getName() );

	prefDlg->Rot_Slider->setValue( red );
	prefDlg->Gruen_Slider->setValue( green );
	prefDlg->Blau_Slider->setValue( blue );
	prefDlg->colorChanged();
	
	prefDlg->metaEnable->setChecked( refreshMeta );
	prefDlg->metaAutoCheck->setChecked( FALSE );
	prefDlg->metaAutoCheck->setChecked( FALSE );

#ifdef ENABLE_HWMIXER
	prefDlg->hardwareKanal->insertItem( tr("- none -") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 1") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 2") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 3") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 4") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 5") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 6") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 7") );
	prefDlg->hardwareKanal->insertItem( tr("Channel 8") );
	prefDlg->hardwareKanal->setCurrentItem( config->readNumEntry( "/radiomixer/channel_"+QString::number( channelID )+"/hwControl", channelID ) );
#else
	delete prefDlg->hwChannelBox;
#endif

	switch( metaMode )
	{
		case 0:
			prefDlg->metaAutoCheck->setChecked( TRUE);
			break;
		case 1:
			prefDlg->metaManualCheck->setChecked( TRUE);
			break;
	}
	prefDlg->metaManualText->setText( staticMetaText );
}

int mixerGUI::execPrefDlg( )
{
	int result = prefDlg->exec();
	if( result == QDialog::Accepted )
	{
		changeName( prefDlg->EditName->text() );
		setPaletteBackgroundColor( QColor( prefDlg->Rot_Slider->value(), prefDlg->Gruen_Slider->value(), prefDlg->Blau_Slider->value() ) );
#ifdef ENABLE_HWMIXER
		hwChannel = prefDlg->hardwareKanal->currentItem();
#endif

		config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/color_red", prefDlg->Rot_Slider->value() );
		config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/color_green", prefDlg->Gruen_Slider->value() );
		config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/color_blue", prefDlg->Blau_Slider->value() );

		refreshMeta = prefDlg->metaEnable->isChecked();

		if(prefDlg->metaEnable->isChecked())
		{
			if( prefDlg->metaManualCheck->isChecked() )
				metaMode = 1;
			else
				metaMode = 0;
			staticMetaText = prefDlg->metaManualText->text();
		}
		save();
	}
	return result;
}

void mixerGUI::showPrefs( )
{
	createPrefDlg();
	execPrefDlg();
	finishPrefDlg();	
}

void mixerGUI::setSlider( int hwChannel, int value )
{
/*	if( hwChannel == this->hwChannel-1 )
	{
		vuSlider->setValue(100-value );
	}
        */
}

void mixerGUI::changeName( QString newName )
{
	chName->setText( newName );
}

void mixerGUI::buttonPressed( int hwChannel, int button )
{
	qDebug("Channel: "+QString::number(hwChannel)+"Button: "+QString::number(button));
}

void mixerGUI::finishPrefDlg( )
{
	delete config;
	delete prefDlg;
}

void mixerGUI::refresh( )
{
	emit( refreshed() );
}

QString mixerGUI::getName( )
{
	return chName->text();
}

void mixerGUI::changePos( int newPos )
{
	playerPos = newPos>=0?newPos:channelID;
	refresh();
}

void mixerGUI::save( )
{
	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/setMeta", refreshMeta );
	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/metaMode", metaMode );
	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/metaText", staticMetaText );
	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/position", playerPos );
	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/name", getName() );
//	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/hwControl", hwChannel );
	config->writeEntry( "/radiomixer/channel_"+QString::number( channelID )+"/type", getType() );
}

int mixerGUI::currentPosition( )
{
	return playerPos;
}

void mixerGUI::setBalance( int balance )
{
/*	if( balance > 0)
	{
                mixer->setLevelLeft( 100-balance );
		mixer->setLevelRight( 100 );
	}else if( balance < 0)
	{
		mixer->setLevelLeft( 100 );
		mixer->setLevelRight( 100+balance );
	}else
	{
		mixer->setLevelLeft( 100 );
		mixer->setLevelRight( 100 );
	}
        */
}

int mixerGUI::getPlayerID( )
{
	return channelID;
}

void mixerGUI::saveSettings( )
{
	config = new QSettings();
	save();
	delete config;
}

QColor mixerGUI::getColor( )
{
	return paletteBackgroundColor();
}

void mixerGUI::associateToChannel( mixerChannel* channel )
{
    connect( channel, SIGNAL(nameChanged(QString)), this, SLOT(changeName(QString)) );
    connect( channel, SIGNAL(volumeChanged( int )), vuSlider, SLOT( setValue( int ) ) );
    connect( vuSlider, SIGNAL(valueChanged( int )), channel, SLOT( setVolume( int ) ) );
}

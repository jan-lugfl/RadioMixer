/* $Id$ */
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
#include "mixergui.h"
#include <QMessageBox>

mixerGUI::mixerGUI( QWidget* parent , const char* name , Qt::WFlags fl )
 : QFrame( parent, name, fl)
{
    layout = new QGridLayout( this );
    actionButtons = new QGridLayout();
    toolButtons = new QGridLayout();

    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Raised );
    setAutoFillBackground( TRUE );
    setAcceptDrops( TRUE );

    vuSlider = new QSlider( this, "volumeSlider" );
    vuSlider->setMinimumHeight( 150 );
    vuSlider->setOrientation( Qt::Vertical );
    vuSlider->setTickmarks( QSlider::Both );
    vuSlider->setMinValue( 0 );
    vuSlider->setMaxValue( 100 );
    vuSlider->setLineStep( 1 );
    vuSlider->setPageStep( 10 );
    vuSlider->setValue( 100 );
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
	
    layout->addWidget( chName, 1, 1, 1, -1 );
    layout->addLayout( actionButtons, 3, 3, Qt::AlignTop );
    layout->addWidget( vuSlider, 3, 2, 2, 1 );
    layout->addLayout( toolButtons, 4, 3, Qt::AlignBottom );

    toolButtons->addWidget( trebleSlider, 10, 1, Qt::AlignBottom );
}


mixerGUI::~mixerGUI()
{
    delete trebleSlider;
    delete vuSlider;
    delete chName;
}

void mixerGUI::languageChange()
{
}

QUuid mixerGUI::getUuid()
{
    return uuid;
}

void mixerGUI::changeName( QString newName )
{
	chName->setText( newName );
}

void mixerGUI::refresh( )
{
	emit( refreshed() );
}

QString mixerGUI::getName( )
{
	return chName->text();
}

QColor mixerGUI::getColor( )
{
	return paletteBackgroundColor();
}

void mixerGUI::channelSettingsChanged( mixerChannel::settingsType settings )
{
    this->settings = settings;

    changeName( settings["name"].toString() );
    setPaletteBackgroundColor( settings["color"].value<QColor>() );

}

void mixerGUI::associateToChannel( mixerChannel* channel )
{
    this->uuid = channel->getUuid();
    connect( channel, SIGNAL(volumeChanged( int )), vuSlider, SLOT( setValue( int ) ) );
    connect( vuSlider, SIGNAL(valueChanged( int )), channel, SLOT( setVolume( int ) ) );
    connect( trebleSlider, SIGNAL(valueChanged( int )), channel, SLOT( setTreble( int ) ) );
    connect( channel, SIGNAL(trebleChanged( int )), trebleSlider, SLOT( setValue( int ) ) );
    connect( channel, SIGNAL(settingsChanged( mixerChannel::settingsType ) ), this, SLOT( channelSettingsChanged( mixerChannel::settingsType ) ) );
    connect( this, SIGNAL(updateSettings( mixerChannel::settingsType ) ), channel, SLOT( updateSettings( mixerChannel::settingsType ) ) );
    // fetch settings..
    channelSettingsChanged( channel->getSettings() );
}

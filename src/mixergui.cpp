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

mixerGUI::mixerGUI( QWidget* parent, Qt::WFlags fl )
 : QFrame( parent, fl)
{
    layout = new QGridLayout( this );
    actionButtons = new QGridLayout();
    toolButtons = new QGridLayout();

    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Raised );
    setAutoFillBackground( TRUE );
    setAcceptDrops( TRUE );

    vuSlider = new QSlider( this );
    vuSlider->setMinimumHeight( 150 );
    vuSlider->setOrientation( Qt::Vertical );
    vuSlider->setTickPosition( QSlider::TicksBothSides );
    vuSlider->setRange( 0, 100 );
    vuSlider->setSingleStep( 1 );
    vuSlider->setPageStep( 10 );
    vuSlider->setValue( 100 );
    vuSlider->setTickInterval( 10 );

    trebleSlider = new QDial( this );
    trebleSlider->setRange( -100, 100 );
    trebleSlider->setValue( 0 );
    trebleSlider->setSingleStep(1);
    trebleSlider->setPageStep(10);

    chName = new scrollLabel( this );
    chName->setMaximumHeight( 23 );
    chName->setFrameShape( QLabel::Panel );
    chName->setFrameShadow( QLabel::Sunken );

    automationButton = new glowButton( this );
    automationButton->setActivatedColor( QColor(255, 255, 150) );
    automationButton->setMaximumHeight( 17 );

    layout->addWidget( chName, 1, 1, 1, -1 );
    layout->addLayout( actionButtons, 3, 3, Qt::AlignTop );
    layout->addWidget( vuSlider, 3, 2, 2, 1 );
    layout->addLayout( toolButtons, 4, 3, Qt::AlignBottom );

    toolButtons->addWidget( trebleSlider, 10, 1, Qt::AlignBottom );
    toolButtons->addWidget( automationButton, 11, 1, Qt::AlignBottom);

}


mixerGUI::~mixerGUI()
{
    delete trebleSlider;
    delete vuSlider;
    delete chName;
}

void mixerGUI::languageChange()
{
    automationButton->setText( tr("AUTO") );
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

void mixerGUI::channelSettingsChanged( mixerChannel::settingsType settings )
{
    this->settings = settings;

    changeName( settings["name"].toString() );
    QPalette pal = palette();
    pal.setColor( QPalette::Window, settings["color"].value<QColor>() );
    setPalette( pal );
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
    connect( automationButton, SIGNAL(clicked()), channel, SLOT(toggleAutomation()) );
    connect( channel, SIGNAL(automationChanged(bool)), automationButton, SLOT(setState(bool)) );

    // fetch settings..
    channelSettingsChanged( channel->getSettings() );
}

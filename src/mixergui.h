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
#ifndef MIXERGUI_H
#define MIXERGUI_H

#include <qframe.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <q3valuevector.h>
#include <qsettings.h>
#include <qdial.h>
#include <q3groupbox.h>
#include <qthread.h>

#include "widgets/vumeter.h"
#include "mixerchannel_fileplayer.h"
#include "dialoge/chanprefdlg.h"
#include "metatag.h"

/**
@author Jan Boysen
*/
class mixerGUI : public QFrame
{
  Q_OBJECT

public:
    mixerGUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~mixerGUI();

	virtual void languageChange();
	int currentPosition();
        virtual QString getName();
	virtual int getPlayerID();
	virtual QColor getColor();
	virtual QString getType() = 0;
        virtual void associateToChannel( mixerChannel* channel );

protected:
	// Gui Elements
        QGridLayout* layout;
        QGridLayout* actionButtons;
        QGridLayout* toolButtons;

        QDial* trebleSlider;
	QSlider* vuSlider;
	QToolButton* prefButton;

	QLabel* chName;
	
	QTimer* buttonBlinkTimer;

	// channel settings Storage
	mixerChannel::settingsType settings;

	unsigned short channelID;
	unsigned short playerPos;
	bool refreshMeta;
	int metaMode;
	QString staticMetaText;

	//Dialogs
	chanPrefDlg* prefDlg;
	virtual void createPrefDlg();
	virtual int execPrefDlg();
	virtual void finishPrefDlg();
			
protected slots:
	virtual void buttonBlinker();
	virtual void showPrefs();
	virtual void refresh();
	virtual void save();
	virtual void channelSettingsChanged( mixerChannel::settingsType );

public slots:
	virtual void changeName( QString newName );
	virtual void changePos( int newPos );
	virtual void setSlider( int hwChannel, int value );
	virtual void buttonPressed( int hwChannel, int button );
	virtual void saveSettings();

signals:
	void refreshed();
	void newMeta( metaTag );
	void updateSettings( mixerChannel::settingsType );
};

#endif

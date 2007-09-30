/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2007 by Jan Boysen                                *
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
#ifndef MIXERCHANNELGUI_H
#define MIXERCHANNELGUI_H

#include <q3frame.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qtimer.h>
#include <q3filedialog.h>
#include <qmessagebox.h>
#include <q3valuevector.h>
#include <qsettings.h>
#include <qdial.h>
#include <q3groupbox.h>

#include "vumeter.h"
#include "playerchannelfile.h"
#include "chanprefdlg.h"
#include "metatag.h"

/**
@author Jan Boysen
*/
class mixerChannelGUI : public Q3Frame
{
  Q_OBJECT

public:
    mixerChannelGUI( int chID, QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~mixerChannelGUI();

	virtual void languageChange();
	int currentPosition();
	virtual QString getName();
	virtual int getPlayerID();
	virtual QColor getColor();
	virtual QString getType() = 0;

protected:
	// Gui Elements
	QDial* trebleSlider;
	QSlider* vuSlider;
	QToolButton* prefButton;

	QLabel* chName;
	
	QTimer* buttonBlinkTimer;

	// Config Storage
	QSettings* config;

	unsigned short channelID;
	unsigned short playerPos;
	bool refreshMeta;
	int metaMode;
	QString staticMetaText;
	playerChannelStd*	mixer;
	int hwChannel;

	//Dialogs
	chanPrefDlg* prefDlg;
	virtual void createPrefDlg();
	virtual int execPrefDlg();
	virtual void finishPrefDlg();
			
protected slots:
	virtual void buttonBlinker();
	virtual void showPrefs();
	virtual void refresh();
	virtual void setBalance( int balance );	
	virtual void save();

public slots:
	virtual void changeName( QString newName );
	virtual void changePos( int newPos );
	virtual void setSlider( int hwChannel, int value );
	virtual void buttonPressed( int hwChannel, int button );
	virtual void saveSettings();

signals:
	void refreshed();
	void newMeta( metaTag );
};

#endif

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
#ifndef MIXERGUI_H
#define MIXERGUI_H

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QTimer>
#include <QDial>
#include <QSlider>

#include "widgets/vumeter.h"
#include "mixerchannel_fileplayer.h"
#include "metatag.h"

/**
@author Jan Boysen
*/
class mixerGUI : public QFrame
{
  Q_OBJECT

public:
    explicit mixerGUI( QWidget* parent = 0, Qt::WFlags fl = 0 );
    virtual ~mixerGUI();

    QUuid getUuid();

	virtual void languageChange();
        virtual QString getName();
//	virtual QColor getColor();
	virtual QString getType() = 0;
        virtual void associateToChannel( mixerChannel* channel );

protected:
	// Gui Elements
        QGridLayout* layout;
        QGridLayout* actionButtons;
        QGridLayout* toolButtons;

        QDial* trebleSlider;
	QSlider* vuSlider;

	QLabel* chName;
	
	// channel settings Storage
	mixerChannel::settingsType settings;

    QUuid uuid;

	bool refreshMeta;
	int metaMode;
	QString staticMetaText;
			
protected slots:
	virtual void refresh();
	virtual void channelSettingsChanged( mixerChannel::settingsType );

public slots:
	virtual void changeName( QString newName );

signals:
	void refreshed();
	void newMeta( metaTag );
	void updateSettings( mixerChannel::settingsType );
};

#endif

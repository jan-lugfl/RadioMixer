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
#ifndef MIXERGUIJACKPORT_H
#define MIXERGUIJACKPORT_H

#include <QRadioButton>

#include "mixergui.h"
#include "glowbutton.h"
#include "mixerchannel_jackin.h"

/**
@author Jan Boysen
*/
class mixerGuiJackport : public mixerGUI
{
Q_OBJECT
public:
    mixerGuiJackport(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
    ~mixerGuiJackport();

    virtual void languageChange();
    virtual QString getType();

protected:
	QGridLayout* meterLayout;
	vuMeter* levelMeterLeft;
	vuMeter* levelMeterRight;
	glowButton*	muteBut;

protected slots:
        virtual void switchMuteButton();

private:
        bool mode_onAir;

public slots:
	virtual void changeName( QString newName );
	virtual void associateToChannel( mixerChannel* channel );
        virtual void muteChanged( bool state );

};

#endif

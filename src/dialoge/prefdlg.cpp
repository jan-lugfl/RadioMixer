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


#include "prefdlg.h"

// include ALSA Soundplayer to get possible ALSA Playback devices....
#ifdef HAVE_ALSA
#include "soundplayeralsa.h"
#endif

prefDlg::prefDlg(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : PreferencesDlg(parent,name, modal,fl)
{
#ifdef HAVE_ALSA
	sndDevice->insertStringList( soundPlayerALSA::getDevices() );
#else
	alsaMode->setDisabled(true);
#endif

#ifndef HAVE_JACK
	jackMode->setDisabled(true);
	alsaMode->setChecked(true);
#endif
}

prefDlg::~prefDlg()
{}



/*$SPECIALIZATION$*/



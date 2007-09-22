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

#ifndef CHANPREFDLG_H
#define CHANPREFDLG_H

#include "kanalpreferences.h"
#include <qlineedit.h>
#include <qslider.h>
#include <qframe.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>

class chanPrefDlg : public KanalPref_dlg
{
  Q_OBJECT

public:
  chanPrefDlg(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~chanPrefDlg();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
    virtual void colorChanged();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/


};

#endif


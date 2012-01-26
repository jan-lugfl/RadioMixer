/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2007 by Jan Boysen                                *
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
#ifndef SONGLISTVIEW_H
#define SONGLISTVIEW_H

#include "playlistviewitem.h"
#include "playlist.h"

#include <q3listview.h>
#include <q3header.h>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class songListView : public Q3ListView
{
Q_OBJECT
public:
	songListView(QWidget *parent = 0, const char *name = 0);
	~songListView();

	virtual void setAcceptDrops( bool state );

protected:
	virtual Q3DragObject* dragObject ();

};

#endif

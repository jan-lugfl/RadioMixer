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
#include "cuelistdlg.h"
#include <QtGui>

cueListDlg::cueListDlg(QWidget *parent, const char *name)
 :QDialog(parent, name), cuelist()
{
	setupUi( this );
}

void cueListDlg::update( metaTag meta, QString channelName )
{
	Q3ListViewItem *item = cueList->findItem( channelName, 0 );
	if( item == 0 )
		cueList->insertItem( new Q3ListViewItem( cueList, channelName, meta.getSong() ) );
	else
		item->setText(1, meta.getSong() );
}

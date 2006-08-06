/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005, 2006 by Jan Boysen                                *
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
#ifndef SONGDBDLG_H
#define SONGDBDLG_H

#include "title.h"
#include "songDBDialog.h"
#include "playlistitemsongdb.h"

#include <qhttp.h>
#include <qurloperator.h>
#include <qdom.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qvaluevector.h>
#include <qsettings.h>
#include <qmessagebox.h>


class songDBDlg: public songDBDialog {
Q_OBJECT
public:
	songDBDlg(QWidget *parent = 0, const char *name = 0);
	~songDBDlg();

protected:
	QHttp*	songDBHndl;
	QHttpRequestHeader* songDB;
	struct Genre {
		QString id;
		QString name;
	};
	QValueVector<Genre>	genreList;
	QString getGenreId( QString genre );
	virtual void resizeEvent ( QResizeEvent * );

	//temporary Playlistitem Storage
	playList* songDBPlaylist;

private:
	int state;

protected slots:
	virtual void displayData( bool );
	virtual void requestData( QString query );
	virtual void playListAdd();
	virtual void cue();
	virtual void search();
	virtual void updateLastPlayed( QString filename ); 

signals:
	void cueTrack( QString , title );
	void addToPlaylist( QString , title );
};

#endif

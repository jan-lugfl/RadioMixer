/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006-2007 by Jan Boysen                                *
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
#ifndef PLAYLISTITEMSONGDB_H
#define PLAYLISTITEMSONGDB_H

#include <playlistitem.h>

#include <qsettings.h>
#include <qdom.h>
#include <qhttp.h>
#include <qurloperator.h>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class playListItemSongDB : public playListItem
{
Q_OBJECT
public:
	playListItemSongDB( const unsigned int id=0,  unsigned  int lastPlayedTS=0, QObject *parent = 0, const char *name = 0);
	~playListItemSongDB();

	virtual void load( const unsigned int id );
	virtual const QString getId();
	virtual void setFile(QString file);
	virtual QDomElement toDomElement( QDomDocument* doc );
	virtual bool hasCostumBackgroundColor();
	virtual QColor getBackgroundColor();
	virtual void refreshMeta();

protected:
	unsigned int songDBId;
	unsigned int lastPlayed;
	QHttp*	songDBHndl;
	QHttpRequestHeader* songDB;

	virtual void readMeta();

protected slots:
	virtual void receiveData( bool );

public slots:
	virtual void startPlaying();


};

#endif

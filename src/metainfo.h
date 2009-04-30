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
#ifndef METAINFO_H
#define METAINFO_H

#include <qobject.h>
#include <qstring.h>
#include <qfile.h>
#include <unistd.h>

#include "metatag.h"

/**
@author Jan Boysen
*/
class metaInfo : public QObject
{
Q_OBJECT
public:
    metaInfo(QObject *parent = 0);
    ~metaInfo();

protected:
	QString command;
	QString cmdparms;

	QString artist;
	QString title;
	QString station;
	QString path;

	metaTag		meta;

	bool enabled;
	bool stationMode;
	int metaMode;

	void runCmd();
	

public slots:
	virtual void refresh();
	virtual void enable();
	virtual void disable();
	//virtual void setMeta( QString artist, QString title );
	virtual void setMeta( metaTag newMeta );
	virtual void setStationMode( QString stationID );
	virtual void setNormalMode();
	virtual void setMetaMode( const int metaMode );

signals:
	void refreshed();

};

#endif

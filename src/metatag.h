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
#ifndef METATAG_H
#define METATAG_H

#include <qobject.h>
#include <qdatetime.h>
#include <qstring.h>

/**
@author Jan Boysen
*/
class metaTag : public QObject
{
Q_OBJECT
public:
	metaTag(QString Title="", QObject *parent = 0, const char *name = 0);
	metaTag(const QString artist, const QString title, const QString genre="", const QTime length=QTime());
	metaTag( const metaTag& oldMeta ); //copyconstructor
	~metaTag();

	virtual metaTag& operator=(const metaTag oldMeta);

	virtual void setArtist( QString artist );
	virtual void setTitle( QString title);
	virtual QString getArtist();
	virtual QString getTitle();
	virtual QString getSong();
	virtual QString getGenre();
	virtual QTime getLength();
	virtual QString getType(){ return "SIMPLE";}

protected:
	QString Artist;
	QString Title;
	QString Genre;
	QTime Length;

};

#endif

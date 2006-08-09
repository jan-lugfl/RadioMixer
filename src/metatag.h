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

#include <qdatetime.h>
#include <qstring.h>

/**
@author Jan Boysen
*/
class metaTag
{
public:
	metaTag(QString Title="");
	metaTag(const QString artist, const QString title, const QString genre="", const QTime length=QTime());
	virtual ~metaTag();

	virtual void setArtist( QString artist );
	virtual QString getArtist();

	virtual void setTitle( QString title);
	virtual QString getTitle();

	virtual QString getSong();

	virtual void setGenre( QString genre );
	virtual QString getGenre();

	virtual void setLength( QTime length );
	virtual void setLength( QString length );
	virtual QTime getLength();

protected:
	QString Artist;
	QString Title;
	QString Genre;
	QTime Length;

};

#endif

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
#ifndef TITLE_H
#define TITLE_H

#include <qobject.h>
#include <qregexp.h>
#include "metatag.h"

/**
@author Jan Boysen
*/
class title : public metaTag
{
Q_OBJECT
public:
	title(QString file="", QObject *parent = 0, const char *name = 0);
	title( const title& oldTitle ); //copyconstructor
	~title();

	virtual title& operator=(const title newTitle);

	virtual QString getFilepath();
	virtual QString getFilename();
	virtual QString getTotalTime();
	virtual QString getFile();
	virtual unsigned int getChannels();
	virtual void setChannels( unsigned int chans);
	virtual unsigned int getSamplerate();
	virtual void setSamplerate( unsigned int rate );

	virtual QString getType(){ return "FILE";}

protected:
	QString Path;
	QString Filename;
	QString Format;
	QString Time;
	unsigned int channels;
	unsigned int Samplerate;
};

#endif
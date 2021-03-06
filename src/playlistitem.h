/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2006-2010 by Jan Boysen                                *
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
#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include "metatag.h"

#include <QObject>
#include <QRegExp>
#include <QColor>
#include <QXmlStreamAttributes>
#include <QUuid>

/**
	@author Jan Boysen <trekkie@media-mission.de>
*/
class playListItem : public QObject, public metaTag
{
Q_OBJECT
public:
	playListItem( QString fileName, QObject *parent = 0 );
	~playListItem();

	enum songState { Loading, Normal, Cued, Playing, Played };

	virtual const QString getId();
	virtual const QString getFilepath();
	virtual const QString getFilename();
	virtual void setFile(QString file);
	virtual const QString getFile();
	virtual unsigned int getChannels();
	virtual void setChannels( unsigned int chans);
	virtual unsigned int getSamplerate();
	virtual void setSamplerate( unsigned int rate );

	virtual void resetState();
	virtual songState getState();

	virtual QString getType(){ return "STD";}

	virtual bool hasCostumBackgroundColor();
	virtual QColor getBackgroundColor();

	virtual int getVote();
	virtual void setVote( int vote );

    virtual QUuid getUuid();

	virtual QXmlStreamAttributes toXmlStreamAttributes();

protected:
    QUuid uuid;
	songState state;
	QString Path;
	QString Filename;
	QString Format;
	unsigned int Samplerate;
	unsigned int channels;
	int vote;

	virtual void readMeta();
	virtual void parseAbsFile( QString file );

public slots:
	virtual void startPlaying();
	virtual void cueing();
	virtual void stopped();

signals:
	void startToPlay( playListItem* );
	void ready( playListItem* );
	void refreshed();
};

#endif

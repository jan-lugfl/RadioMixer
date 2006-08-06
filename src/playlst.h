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
#ifndef PLAYLST_H
#define PLAYLST_H

#include <qvaluevector.h>
#include <qobject.h>
#include <qtextstream.h>
#include <qfile.h>
#include "title.h"

/**
@author Jan Boysen
*/
class playLST : public QObject
{
  Q_OBJECT
public:
    playLST(QObject *parent = 0, const char *name = 0);
    ~playLST();

    const QString getPlayerName();
    title const getNextTrack();
    const int getcurrentTrackIndex();
    const QString getFileName( int index );
    
    const int count();
    const title* at( int index );
//    void append( playListItem newItem );
    void append( title newItem );

public slots:    
    virtual void setPlayerName( QString name );
    virtual void itemUp( int index );
    virtual void itemDown( int index );
    virtual void delItem( int index );
    virtual void clear();
    virtual void loadFromFile( QString fileName );
    virtual void saveToFile( QString fileName );
    virtual void cueNext( int index );

private:
	QValueVector<title> playList;
	
	QString playerName;
	int nextTrack;
	
signals:
	void changed();
};

#endif

/* $Id:$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2010 by Jan Boysen                                      *
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
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QString>
#include <QList>
#include <QUuid>

#include "playlistitem.h"

class playList : public QObject
{
Q_OBJECT
public:
    explicit playList(QUuid uuid = QUuid(), QObject *parent = 0);
    virtual ~playList();

    QString getName();
    QList<playListItem*> getItems();
    QUuid getUuid();

    virtual playListItem* getNext();

    // load save functions for the paylist...
    virtual bool loadFromFile( QString filename );
    virtual bool loadFromXML( QString xml );
    virtual void saveToFile( QString filename );
    virtual QString getAsXML();

protected:
    QString name;
    int currentIndex;
    QList<playListItem*> items;

private:
    QUuid uuid;

signals:
    void renamed( QString );
    void changed();

protected slots:
    virtual void itemChanged();

public slots:
    virtual void reset();
    virtual void rename( QString name );
    virtual void addItem( playListItem* newItem );
};

#endif // PLAYLIST_H

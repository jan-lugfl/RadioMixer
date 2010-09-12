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
#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QList>

#include "playlist.h"

class playlistManager : public QObject
{
Q_OBJECT
private:
    explicit playlistManager(QObject *parent = 0);  // This is a singleton...
    playlistManager( playlistManager& ){}  // disable copy contructor

    static playlistManager* instance; // singleton

    // normal object stuff below...
    QList<playList*> playlists;

public:
    static playlistManager* getInstance(); // get the singleton...

    // normal object stuff below...
    void registerPlaylist( playList* newPlaylist );
    QList<playList*> getAllPlaylists();

signals:

public slots:

};

#endif // PLAYLISTMANAGER_H

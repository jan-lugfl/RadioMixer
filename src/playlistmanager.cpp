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
#include "playlistmanager.h"

// init static singleton...
playlistManager* playlistManager::instance = NULL;

playlistManager::playlistManager(QObject *parent) :
    QObject(parent)
{
}

playlistManager* playlistManager::getInstance()
{
    if(playlistManager::instance == NULL)
        playlistManager::instance = new playlistManager();
    return playlistManager::instance;
}

void playlistManager::registerPlaylist( playList* newPlaylist )
{
    playlists.append( newPlaylist );
    connect( newPlaylist, SIGNAL(renamed(QString)), this, SLOT(onPlaylistChange()) );
    emit( changed() );
}

void playlistManager::unregisterPlaylist( playList* playlist )
{
    disconnect( playlist, SIGNAL(renamed(QString)), this, SLOT(onPlaylistChange()) );
    playlists.removeOne( playlist );
    emit( changed() );
}

QList<playList*> playlistManager::getAllPlaylists()
{
    return playlists;
}

void playlistManager::onPlaylistChange()
{
    emit( changed() );
}

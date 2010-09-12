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
#include "playlist.h"
#include "playlistmanager.h"

playList::playList(QObject *parent) :
    QObject(parent)
{
    // get manager an register myself... The manager connects some signals ans slots...
    playlistManager::getInstance()->registerPlaylist( this );
}

playList::~playList()
{
    emit( destroyed() );

    // destroy all items...
    foreach(playListItem* item, items)
        delete item;
}

QString playList::getName()
{
    return name;
}

void playList::rename(QString name)
{
    qWarning("renaming playlist to "+name);
    if(name != this->name)
    {
        this->name = name;
        emit( renamed( this->name ) );
    }
}

void playList::addItem( playListItem* newItem )
{
    items.append( newItem );
}

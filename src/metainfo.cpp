/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2010 by Jan Boysen                                 *
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
#include "metainfo.h"
#include "settings.h"

metaInfo::metaInfo(QObject *parent)
 : QObject(parent)
{
    enabled = Settings::get("/meta/enabled", TRUE).toBool();
    stationMode = FALSE;
    proc = new QProcess( this );
}


metaInfo::~metaInfo()
{
}

void metaInfo::runCmd( )
{
    if( enabled )
    {
        if( proc->state() == QProcess::NotRunning )
        {
            QStringList args;
            args.append( artist );
            args.append( title );
            proc->start( Settings::get("/meta/command").toString(), args );
        }
        else
            qWarning("Could not call meta update command as its still running...");
    }
}

void metaInfo::refresh( )
{
    runCmd();
    emit( refreshed() );
}

void metaInfo::setMeta( metaTag newMeta )
{
    meta = newMeta;
    if( enabled )
    {
        if( stationMode )
        {
            this->title = newMeta.getSong();
            this->artist = station;
        }
        else
        {
            this->artist = newMeta.getArtist();
            this->title = newMeta.getTitle();
        }
        refresh();
    }
}

void metaInfo::enable( )
{
    enabled = TRUE;
}

void metaInfo::disable( )
{
    enabled = FALSE;
}

void metaInfo::setStationMode( QString stationID )
{
    stationMode = TRUE;
    station = stationID;
}

void metaInfo::setNormalMode( )
{
    stationMode = FALSE;
}


/* $Id$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2007 by Jan Boysen                                *
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

metaInfo::metaInfo(QObject *parent, const char *name)
 : QObject(parent, name)
{
	enabled = FALSE;
	stationMode = FALSE;
	metaMode = 1; // default we use Ices2 !!
}


metaInfo::~metaInfo()
{
}

void metaInfo::runCmd( )
{
	QSettings config;
	QString exec = config.readEntry( "/radiomixer/meta/command", "/usr/local/bin/refreshMeta.sh" );
	if( enabled && metaMode==1) // Ices2 Mode !!
	{
		if (!fork())
		{
			execl( exec, exec, NULL);
			exit(0);
		}
	}
}

void metaInfo::refresh( )
{
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

		QSettings config;
		QFile file( config.readEntry( "/radiomixer/meta/file", "/tmp/radiomixer.current" ) );
		
		if ( file.open( IO_WriteOnly | IO_Truncate ) ) {
			QTextStream stream( &file );

			switch ( metaMode )  // which Streaming client is used ??
			{
				case 1: // Ices 2
					stream << "artist=" << artist << "\n";
					stream << "title=" << title << "\n";
					break;
				case 2: // Oddcast V3
					stream << artist << " - " << title << "\n";
					break;
			}
        		file.close();
    		}

		runCmd();
		emit( refreshed() );
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

void metaInfo::setMetaMode( const int metaMode )
{
	this->metaMode = metaMode;
}



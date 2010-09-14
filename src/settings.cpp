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
#include "settings.h"

QSettings Settings::settings("RadioMixer", "RadioMixer");

QVariant Settings::get(const QString &key, const QVariant &defaultValue)
{
    return Settings::settings.value( key, defaultValue);
}

void Settings::set( const QString &key, const QVariant &value )
{
    Settings::settings.setValue( key, value );
}

void Settings::remove( const QString &key )
{
    Settings::settings.remove( key );
    Settings::settings.beginGroup( key );
    foreach( QString del_key, Settings::settings.allKeys() )
        Settings::settings.remove( del_key );
    Settings::settings.endGroup();
}

QStringList Settings::getSubKeys(const QString &key)
{
    Settings::settings.beginGroup( key );
    QStringList ret = Settings::settings.childKeys();
    Settings::settings.endGroup();
    return ret;
}

QStringList Settings::getSubGroups( const QString& key )
{
    Settings::settings.beginGroup( key );
    QStringList ret = Settings::settings.childGroups();
    Settings::settings.endGroup();
    return ret;
}

void Settings::sync()
{
    Settings::settings.sync();
}

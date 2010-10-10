/* $Id$ */
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

#ifndef HTTPCOMM_H
#define HTTPCOMM_H

#include <QNetworkAccessManager>

class httpComm
{
public:
    // wrapper functions for QNetworkAccessManager class
    static QNetworkReply* get( const QNetworkRequest & request );
    static QNetworkReply* head( const QNetworkRequest & request );
    static QNetworkReply* post( const QNetworkRequest & request, QIODevice * data );
    static QNetworkReply* post( const QNetworkRequest & request, const QByteArray & data );

private:
    httpComm(){} // Do not allow to create objects from this class
    httpComm( httpComm& ){}  // disable copy contructor

    static QNetworkAccessManager* nam;
};

#endif // HTTPCOMM_H

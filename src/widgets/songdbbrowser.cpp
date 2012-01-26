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
#include "songdbbrowser.h"
#include "ui_songdbbrowser.h"
#include "httpcomm.h"
#include "settings.h"
#include "playlistitemsongdb.h"

#include <QXmlStreamReader>

songDbBrowser::songDbBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::songDbBrowser)
{
    ui->setupUi(this);
    reloadGenres();
}

songDbBrowser::~songDbBrowser()
{
    delete ui;
}

void songDbBrowser::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void songDbBrowser::reloadGenres()
{
    apiReply = httpComm::post(  QNetworkRequest(QUrl(Settings::get("songDBURL", "http://songdb.mm.loc/xmlctrl.pl").toString())), QString("getGenres=1").toAscii() );
    connect( apiReply, SIGNAL(finished()), this, SLOT(apiReplyReceived()));
}

void songDbBrowser::apiReplyReceived()
{
    QXmlStreamReader xml;
    xml.addData( apiReply->readAll() );
    if( xml.readNextStartElement() )
    {
	if(xml.name() == "genres")
	{
	    ui->genres->clear();
	    ui->genres->addItem(tr("-- ALL --"),"*");
	    while(xml.readNextStartElement())
	    {
		if( xml.name() == "genre")
		{
		    ui->genres->addItem(
			    xml.attributes().value("name").toString(),
			    xml.attributes().value("id").toString()
			    );
		}
		xml.skipCurrentElement();
	    }
	}
	if(xml.name() == "songs")
	{
	    ui->songList->clear();
	    ui->songList->setColumnHidden( 4, true );
	    while(xml.readNextStartElement())
	    {
		if( xml.name() == "song")
		{
		    QTreeWidgetItem* itm = new QTreeWidgetItem( ui->songList );
		    itm->setText( 0, xml.attributes().value("title").toString() );
		    itm->setText( 1, xml.attributes().value("interpret").toString() );
		    itm->setText( 2, xml.attributes().value("genre").toString() );
		    itm->setText( 3, xml.attributes().value("length").toString() );
		    itm->setData( 4, Qt::UserRole, xml.attributes().value("id").toString() );
		}
		xml.skipCurrentElement();
	    }
	    ui->songList->resizeColumnToContents(0);
	    ui->songList->resizeColumnToContents(1);
	}
	xml.skipCurrentElement();
    }
    apiReply->deleteLater();
}

void songDbBrowser::on_searchButton_clicked()
{
    apiReply = httpComm::post(  QNetworkRequest(QUrl(Settings::get("songDBURL", "http://songdb.mm.loc/xmlctrl.pl").toString())),
				QString( "query="+ui->searchBox->text()+"&genre="+ui->genres->itemData( ui->genres->currentIndex() ).toString() ).toAscii()
				);
    connect( apiReply, SIGNAL(finished()), this, SLOT(apiReplyReceived()));
}

void songDbBrowser::on_songList_doubleClicked(QModelIndex index)
{
    QString selectedId = ui->songList->currentItem()->data( 4, Qt::UserRole ).toString();
    emit( itemSelected( new playListItemSongDB( selectedId.toInt() )) );
}

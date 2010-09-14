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
#include "playlistdialog.h"
#include "ui_playlistdialog.h"

#include "playlistwidget.h"
#include "filebrowser.h"
#include "songdbbrowser.h"

playlistDialog::playlistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playlistDialog)
{
    ui->setupUi(this);

    // get playlist manager singleton...
    plm = playlistManager::getInstance();

    foreach( playList* playlist, plm->getAllPlaylists() )
        new playlistWidget( playlist, ui->playlistList );
    ui->playlistList->setCurrentRow(0);

    // append one initial file browser source tab...
    fileBrowser* filebrowser = new fileBrowser( ui->itemSourceTab );
    ui->itemSourceTab->insertTab( filebrowser, tr("Filebrowser") );
    connect( filebrowser, SIGNAL(itemSelected(playListItem*)), this, SLOT(addItemToCurrentPlaylist(playListItem*)));

    songDbBrowser* songdb = new songDbBrowser( ui->itemSourceTab );
    ui->itemSourceTab->insertTab( songdb, tr("SongDB browser") );
    connect( songdb, SIGNAL(itemSelected(playListItem*)), this, SLOT(addItemToCurrentPlaylist(playListItem*)));
}

playlistDialog::~playlistDialog()
{
    delete ui;
}

void playlistDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void playlistDialog::on_playlistList_itemChanged(QListWidgetItem* item)
{
    playlistWidget* pls = dynamic_cast<playlistWidget*>(item);
    if(pls)
        pls->playlist->rename( item->text() );
}

void playlistDialog::on_playlistList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    playlistWidget* prev = dynamic_cast<playlistWidget*>(previous);
    if(prev)
        disconnect( prev->playlist, SIGNAL(changed()), this, SLOT(reloadPlaylist()));
    playlistWidget* cur = dynamic_cast<playlistWidget*>(previous);
    if(cur)
        connect( cur->playlist, SIGNAL(changed()), this, SLOT(reloadPlaylist()));
    reloadPlaylist();
}

void playlistDialog::reloadPlaylist()
{
    ui->playListView->clear();
    playList* plst = dynamic_cast<playlistWidget*>(ui->playlistList->currentItem())->playlist;
    foreach(playListItem* item, plst->getItems())
    {
        QTreeWidgetItem* itm = new QTreeWidgetItem( ui->playListView );
        itm->setText( 0, item->getTitle() );
        itm->setText( 1, item->getArtist() );
    }
    ui->playListView->resizeColumnToContents(0);
    ui->playListView->resizeColumnToContents(1);
}

void playlistDialog::addItemToCurrentPlaylist( playListItem* item )
{
    playlistWidget* plst = dynamic_cast<playlistWidget*>(ui->playlistList->currentItem());
    if(plst)
        plst->playlist->addItem( item );
    else
        qWarning("FATAL: trying to add item to playlist but non selected...");
}
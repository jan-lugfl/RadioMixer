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

#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>

playlistDialog::playlistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playlistDialog)
{
    ui->setupUi(this);

    // get playlist manager singleton...
    plm = playlistManager::getInstance();

    // reload playlists here if the manager notifies us, that a list was added, removed, renamed or what ever...
    connect( plm, SIGNAL(changed()), this, SLOT(reloadPlaylists()));

    // load current playlists now...
    reloadPlaylists();

    ui->playListView->setAlternatingRowColors( true );

    // set standard Icons...
    ui->newPlaylist->setIcon( QApplication::style()->standardIcon( QStyle::SP_FileDialogNewFolder ));
    ui->closePlaylist->setIcon( QApplication::style()->standardIcon( QStyle::SP_DialogCloseButton ));
    ui->openPlaylist->setIcon( QApplication::style()->standardIcon( QStyle::SP_DialogOpenButton ));
    ui->savePlaylist->setIcon( QApplication::style()->standardIcon( QStyle::SP_DialogSaveButton ));
    ui->resetPlaylist->setIcon( QApplication::style()->standardIcon( QStyle::SP_DialogResetButton ));

    // append one initial file browser source tab...
    fileBrowser* filebrowser = new fileBrowser( ui->itemSourceTab );
    ui->itemSourceTab->insertTab( 0, filebrowser, tr("Filebrowser") );
    connect( filebrowser, SIGNAL(itemSelected(playListItem*)), this, SLOT(addItemToCurrentPlaylist(playListItem*)));

    songDbBrowser* songdb = new songDbBrowser( ui->itemSourceTab );
    ui->itemSourceTab->insertTab( 1, songdb, tr("SongDB browser") );
    connect( songdb, SIGNAL(itemSelected(playListItem*)), this, SLOT(addItemToCurrentPlaylist(playListItem*)));
}

playlistDialog::~playlistDialog()
{
    delete ui;
}

playList* playlistDialog:: getCurrentSelectedPlaylist()
{
    if(ui->playlistList->currentItem())
    {
        playlistWidget* plst = dynamic_cast<playlistWidget*>(ui->playlistList->currentItem());
        if(plst)
            return plst->playlist;
    }
    return NULL;
}

void playlistDialog::reloadPlaylists()
{
    qWarning("reloading playlists");
    ui->playlistList->clear();
    foreach( playList* playlist, plm->getAllPlaylists() )
    {
        playlistWidget* itm = new playlistWidget( playlist, ui->playlistList );
        itm->setIcon( QIcon(":/icons/playlist-icon") );
    }
    ui->playlistList->setCurrentRow(0);
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
    playList* pl = getCurrentSelectedPlaylist();
    if(pl)
        pl->rename( item->text() );
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
    playList* plst = getCurrentSelectedPlaylist();
    if(!plst)
        return;
    foreach(playListItem* item, plst->getItems())
    {
        QTreeWidgetItem* itm = new QTreeWidgetItem( ui->playListView );
        itm->setData( 0, Qt::UserRole, item->getUuid().toString() );
        itm->setText( 0, item->getTitle() );
        itm->setText( 1, item->getArtist() );
        itm->setText( 2, item->getGenre() );
        itm->setText( 3, item->getLength().toString() );
        switch( item->getState() )
        {
        case playListItem::Loading:
            itm->setBackgroundColor( 0, Qt::yellow);
            itm->setBackgroundColor( 1, Qt::yellow);
            itm->setBackgroundColor( 2, Qt::yellow);
            itm->setBackgroundColor( 3, Qt::yellow);
            break;
        case playListItem::Normal:
            break;
        case playListItem::Cued:
            itm->setBackgroundColor( 0, Qt::gray );
            itm->setBackgroundColor( 1, Qt::gray );
            itm->setBackgroundColor( 2, Qt::gray );
            itm->setBackgroundColor( 3, Qt::gray );
            ui->playListView->scrollToItem( itm, QAbstractItemView::PositionAtCenter );
            break;
        case playListItem::Playing:
            itm->setBackgroundColor( 0, Qt::green );
            itm->setBackgroundColor( 1, Qt::green );
            itm->setBackgroundColor( 2, Qt::green );
            itm->setBackgroundColor( 3, Qt::green );
            ui->playListView->scrollToItem( itm, QAbstractItemView::PositionAtCenter );
            break;
        case playListItem::Played:
            QBrush foreground = QBrush(Qt::gray);
            itm->setForeground( 0, foreground );
            itm->setForeground( 1, foreground );
            itm->setForeground( 2, foreground );
            itm->setForeground( 3, foreground );
            break;
        }
    }
    ui->playListView->resizeColumnToContents(0);
    ui->playListView->resizeColumnToContents(1);
}

void playlistDialog::addItemToCurrentPlaylist( playListItem* item )
{
    playList* pl = getCurrentSelectedPlaylist();
    if(!pl)
    {
        qWarning("FATAL: trying to add item to playlist but non selected...");
        return;
    }
    pl->addItem( item );
}

void playlistDialog::on_newPlaylist_clicked()
{
    bool ok;
    QString name = QInputDialog::getText( this, tr("Name of new playlist"), tr("Please enter the name for the new playlist"), QLineEdit::Normal, tr("New Playlist"), &ok);
    if(ok)
    {
        playList* newList = new playList();
        newList->rename( name );
    }
}

void playlistDialog::on_closePlaylist_clicked()
{
    playList* pl = getCurrentSelectedPlaylist();
    if(!pl)
        return;
    delete pl;
}

void playlistDialog::on_openPlaylist_clicked()
{
    QString fname = QFileDialog::getOpenFileName( this, tr("load playlist") );
    playList* newList = new playList();
    if(!newList->loadFromFile( fname ))
	delete newList; // remove list as it could not be loaded...
}

void playlistDialog::on_savePlaylist_clicked()
{
    playList* pl = getCurrentSelectedPlaylist();
    if(!pl)
	return;
    pl->saveToFile( QFileDialog::getSaveFileName( this, tr("Save playlist") ));
}

void playlistDialog::on_resetPlaylist_clicked()
{
    playList* pl = getCurrentSelectedPlaylist();
    if(!pl)
        return;
    pl->reset();
}

void playlistDialog::on_playListView_customContextMenuRequested(const QPoint &pos)
{
    QPoint displayPos = ui->playListView->viewport()->mapToGlobal(pos);
    QMenu contextMenu;
    QAction* act_delete = contextMenu.addAction(tr("Delete"));
    connect(act_delete, SIGNAL(triggered()), this, SLOT(deleteCurrentPlaylistItem()));
    QAction* act_reset = contextMenu.addAction(tr("Reset state"));
    connect(act_reset, SIGNAL(triggered()), this, SLOT(resetCurrentPlaylistItem()));

    contextMenu.exec(displayPos);
}

void playlistDialog::deleteCurrentPlaylistItem()
{
    playList* pl = getCurrentSelectedPlaylist();
    if(pl)
        pl->deleteItem(QUuid(ui->playListView->currentItem()->data(0, Qt::UserRole).toString()));
}

void playlistDialog::resetCurrentPlaylistItem()
{
    playList* pl = getCurrentSelectedPlaylist();
    if(pl)
    {
        playListItem* item = pl->getItem(QUuid(ui->playListView->currentItem()->data(0, Qt::UserRole).toString()));
        if(item)
            item->resetState();
    }
}

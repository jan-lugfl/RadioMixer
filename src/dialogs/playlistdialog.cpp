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

playlistDialog::playlistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playlistDialog)
{
    ui->setupUi(this);

    // get playlist manager singleton...
    plm = playlistManager::getInstance();

    foreach( playList* playlist, plm->getAllPlaylists() )
        new playlistWidget( playlist, ui->playlistList );
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

}

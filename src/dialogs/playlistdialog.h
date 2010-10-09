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
#ifndef PLAYLISTDIALOG_H
#define PLAYLISTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "playlistmanager.h"

namespace Ui {
    class playlistDialog;
}

class playlistDialog : public QDialog {
    Q_OBJECT
public:
    playlistDialog(QWidget *parent = 0);
    ~playlistDialog();

protected:
    void changeEvent(QEvent *e);
    playlistManager* plm;

private:
    Ui::playlistDialog *ui;

    playList* getCurrentSelectedPlaylist();

private slots:
    void on_resetPlaylist_clicked();
    void on_savePlaylist_clicked();
    void on_openPlaylist_clicked();
    void on_closePlaylist_clicked();
    void on_newPlaylist_clicked();
    void on_playlistList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_playlistList_itemChanged(QListWidgetItem* item);

    void addItemToCurrentPlaylist( playListItem* item );
    void reloadPlaylist();
    void reloadPlaylists();
};

#endif // PLAYLISTDIALOG_H

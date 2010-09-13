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
#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>
#include <QFileSystemModel>
#include "playlistitem.h"

namespace Ui {
    class fileBrowser;
}

class fileBrowser : public QWidget {
    Q_OBJECT
public:
    explicit fileBrowser(QWidget *parent = 0);
    virtual ~fileBrowser();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::fileBrowser *ui;
    QFileSystemModel* fs_dir;
    QFileSystemModel* fs_file;

signals:
    void itemSelected( playListItem* );

private slots:
    void on_current_path_returnPressed();
    void on_file_view_doubleClicked(QModelIndex index);
    void on_path_view_collapsed(QModelIndex index);
    void on_path_view_expanded(QModelIndex index);
    void on_path_view_activated(QModelIndex index);

    void onFilepathChanged( QString newPath );
};

#endif // FILEBROWSER_H

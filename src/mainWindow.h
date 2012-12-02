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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "soundplayer.h"
#include "mixerguialsamix.h"
#include "channelmixer.h"
#include "remotecontrol_midi.h"

#include "dialogs/playlistdialog.h"

#include <QtGui/QMainWindow>

namespace Ui {
    class RadioMixer;
}

class mainWindow : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY(mainWindow)
public:
    explicit mainWindow(QWidget *parent = 0);
    virtual ~mainWindow();

    QUuid addNewChannel( QString type, QUuid uuid = QUuid() );
    Ui::RadioMixer *rm_ui;

public slots:
    virtual void aboutQt();
    virtual void showSettings();

protected:
    virtual void changeEvent(QEvent *e);

private:
    soundPlayer* player;
    remoteControl* rc;

    // object for our channel mixer thread which the mixing engine for all channels...
    channelMixer* mixer;

    playlistDialog* playlistDlg;

signals:
    void showAboutQt();

private slots:
    void on_action_Playlist_Manager_triggered();
    void on_action_About_triggered();
};

#endif // MAINWINDOW_H

/* $Id:$ */
/***************************************************************************
 *   OpenRadio - RadioMixer                                                *
 *   Copyright (C) 2005-2009 by Jan Boysen                                *
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

#include "metainfo.h"
#include "soundplayer.h"
#include "mixerguialsamix.h"

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

public slots:
    virtual void aboutQt();
    virtual void addNewFilePlayer();
    virtual void addNewMixerChannel();
    virtual void addNewJackChannel( QString chName );

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::RadioMixer *rm_ui;
    soundPlayer* player;
    // used for testing only
    mixerGUI* dummy;
    mixerChannel* dummy1_;
    mixerGUI* dummy2;
    mixerChannel* dummy2_;
    mixerGUI* dummy3;
    mixerChannel* dummy3_;
    mixerGUI* dummy4;
    mixerChannel* dummy4_;
    mixerGUI* dummy5;
    mixerGUI* dummy6;
    mixerGUI* dummy7;
    mixerGUI* dummy8;

    // Meta Info for Live Streaming with Ices
    metaInfo meta;

signals:
    void showAboutQt();
};

#endif // MAINWINDOW_H

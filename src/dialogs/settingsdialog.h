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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "mixerchannel.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
    class settingsDialog;
}

class settingsDialog : public QDialog {
    Q_OBJECT
public:
    settingsDialog(QWidget *parent = 0);
    ~settingsDialog();

    QList<QUuid> channels;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::settingsDialog *ui;
    QMap<QUuid, mixerChannel::settingsType> settingsCache; // used to store the channel settings untill we apply the changes

protected slots:
    virtual void accept();

private slots:
    void on_channel_add_clicked();
    void on_colorChooser_clicked();
    void on_channelList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_channel_remove_clicked();
    void on_downButton_clicked();
    void on_upButton_clicked();
};

#endif // SETTINGSDIALOG_H

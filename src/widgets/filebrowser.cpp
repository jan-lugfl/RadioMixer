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
#include "filebrowser.h"
#include "ui_filebrowser.h"

fileBrowser::fileBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileBrowser)
{
    ui->setupUi(this);

    fs_dir = new QFileSystemModel( this );
    fs_dir->setRootPath(QDir::rootPath());
    fs_dir->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Drives );
    ui->path_view->setModel( fs_dir );
    ui->path_view->setColumnHidden(1, true);
    ui->path_view->setColumnHidden(2, true);
    ui->path_view->setColumnHidden(3, true);
    ui->path_view->setColumnHidden(4, true);
    ui->path_view->setRootIndex( fs_dir->index(QDir::rootPath()));

    fs_file = new QFileSystemModel( this );
    connect( fs_file, SIGNAL(rootPathChanged(QString)), this, SLOT(onFilepathChanged(QString)));
    fs_file->setRootPath(QDir::rootPath());
    fs_file->setFilter( QDir::Files );
    fs_file->setNameFilterDisables ( false ); // hide filtered items instead of disabling them
    QStringList supported_filetypes;
#ifdef HAVE_MAD
    supported_filetypes.append("*.mp3");
#endif
#ifdef HAVE_OGG
    supported_filetypes.append("*.ogg");
#endif
#ifdef HAVE_SNDFILE
    supported_filetypes << "*.wav" << "*.bwf" << "*.flac" << "*.fla";
#endif
    fs_file->setNameFilters( supported_filetypes );
    ui->file_view->setModel( fs_file );
    ui->file_view->setColumnHidden(3, true);
    ui->file_view->setRootIndex( fs_file->index(QDir::rootPath()));
}

fileBrowser::~fileBrowser()
{
    delete ui;
}

void fileBrowser::changeEvent(QEvent *e)
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

void fileBrowser::on_path_view_activated(QModelIndex index)
{
    ui->current_path->setText( fs_dir->filePath(index) );
    fs_file->setRootPath( fs_dir->filePath(index) );
    ui->file_view->setRootIndex( fs_file->index( fs_dir->filePath( index ) ));
    ui->file_view->resizeColumnToContents(0);
}

void fileBrowser::on_path_view_expanded(QModelIndex index)
{
    ui->path_view->resizeColumnToContents( 0 );
}

void fileBrowser::on_path_view_collapsed(QModelIndex index)
{
    ui->path_view->resizeColumnToContents( 0 );
}

void fileBrowser::onFilepathChanged( QString newPath )
{
    ui->file_view->resizeColumnToContents(0);
}

void fileBrowser::on_file_view_doubleClicked(QModelIndex index)
{
    emit( itemSelected( new playListItem(fs_file->filePath(index))) );
}

void fileBrowser::on_current_path_returnPressed()
{
    ui->path_view->setCurrentIndex( fs_dir->index( ui->current_path->text() ) );
    fs_file->setRootPath( ui->current_path->text() );
    ui->file_view->setRootIndex( fs_file->index( ui->current_path->text() ));
    ui->file_view->resizeColumnToContents(0);
}

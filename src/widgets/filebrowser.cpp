#include "filebrowser.h"
#include "ui_filebrowser.h"

fileBrowser::fileBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileBrowser)
{
    ui->setupUi(this);
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

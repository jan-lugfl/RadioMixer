#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>

namespace Ui {
    class fileBrowser;
}

class fileBrowser : public QWidget {
    Q_OBJECT
public:
    fileBrowser(QWidget *parent = 0);
    ~fileBrowser();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::fileBrowser *ui;
};

#endif // FILEBROWSER_H

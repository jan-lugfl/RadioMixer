/****************************************************************************
** Form interface generated from reading ui file 'timerSettings.ui'
**
** Created: Wed Sep 19 23:41:41 2007
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TIMERSETTINGS_H
#define TIMERSETTINGS_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QSpinBox;
class QRadioButton;
class QLineEdit;
class QPushButton;

class timerSettings : public QDialog
{
    Q_OBJECT

public:
    timerSettings( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~timerSettings();

    QFrame* frame3;
    QLabel* textLabel1;
    QSpinBox* hoursBox;
    QLabel* starttimelabel;
    QRadioButton* forward;
    QLineEdit* timerName;
    QRadioButton* reverse;
    QSpinBox* minutesBox;
    QSpinBox* secondsBox;
    QPushButton* okButton;

protected:
    QVBoxLayout* timerSettingsLayout;

protected slots:
    virtual void languageChange();

};

#endif // TIMERSETTINGS_H

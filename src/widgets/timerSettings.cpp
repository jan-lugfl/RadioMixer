/****************************************************************************
** Form implementation generated from reading ui file 'timerSettings.ui'
**
** Created: Wed Sep 19 23:41:42 2007
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "timerSettings.h"

#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a timerSettings as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
timerSettings::timerSettings( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "timerSettings" );
    timerSettingsLayout = new QVBoxLayout( this, 11, 6, "timerSettingsLayout"); 

    frame3 = new QFrame( this, "frame3" );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );

    textLabel1 = new QLabel( frame3, "textLabel1" );
    textLabel1->setGeometry( QRect( 20, 20, 73, 20 ) );

    hoursBox = new QSpinBox( frame3, "hoursBox" );
    hoursBox->setEnabled( FALSE );
    hoursBox->setGeometry( QRect( 60, 170, 60, 24 ) );

    starttimelabel = new QLabel( frame3, "starttimelabel" );
    starttimelabel->setEnabled( FALSE );
    starttimelabel->setGeometry( QRect( 50, 150, 170, 20 ) );

    forward = new QRadioButton( frame3, "forward" );
    forward->setGeometry( QRect( 20, 80, 350, 22 ) );
    forward->setChecked( TRUE );

    timerName = new QLineEdit( frame3, "timerName" );
    timerName->setGeometry( QRect( 20, 40, 350, 23 ) );

    reverse = new QRadioButton( frame3, "reverse" );
    reverse->setEnabled( TRUE );
    reverse->setGeometry( QRect( 20, 120, 350, 22 ) );

    minutesBox = new QSpinBox( frame3, "minutesBox" );
    minutesBox->setEnabled( FALSE );
    minutesBox->setGeometry( QRect( 140, 170, 50, 24 ) );
    minutesBox->setMaxValue( 59 );

    secondsBox = new QSpinBox( frame3, "secondsBox" );
    secondsBox->setEnabled( FALSE );
    secondsBox->setGeometry( QRect( 210, 170, 50, 24 ) );
    secondsBox->setMaxValue( 59 );
    timerSettingsLayout->addWidget( frame3 );

    okButton = new QPushButton( this, "okButton" );
    okButton->setDefault( TRUE );
    okButton->setFlat( FALSE );
    timerSettingsLayout->addWidget( okButton );
    languageChange();
    resize( QSize(411, 273).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( forward, SIGNAL( clicked() ), reverse, SLOT( toggle() ) );
    connect( reverse, SIGNAL( clicked() ), forward, SLOT( toggle() ) );
    connect( reverse, SIGNAL( toggled(bool) ), starttimelabel, SLOT( setEnabled(bool) ) );
    connect( reverse, SIGNAL( toggled(bool) ), hoursBox, SLOT( setEnabled(bool) ) );
    connect( reverse, SIGNAL( toggled(bool) ), minutesBox, SLOT( setEnabled(bool) ) );
    connect( reverse, SIGNAL( toggled(bool) ), secondsBox, SLOT( setEnabled(bool) ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
timerSettings::~timerSettings()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void timerSettings::languageChange()
{
    setCaption( tr( "timer settings" ) );
    textLabel1->setText( tr( "Name:" ) );
    starttimelabel->setText( tr( "start time:" ) );
    forward->setText( tr( "&count forward" ) );
    forward->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    reverse->setText( tr( "count bac&kward" ) );
    reverse->setAccel( QKeySequence( tr( "Alt+K" ) ) );
    okButton->setText( tr( "&Ok" ) );
    okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
}


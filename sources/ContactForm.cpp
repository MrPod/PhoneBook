#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QDate>
#include <QIcon>
#include <QRandomGenerator>
#include <QSizePolicy>
#include <QFrame>
#include <QSplitter>
#include <QPushButton>
#include <QFormLayout>
#include <QSlider>
#include <QWidget>
#include <QLayoutItem>
#include <QToolButton>
#include <QPushButton>
#include <QFont>
#include <QAction>
#include <QPalette>
#include <QDialog>
#include <QScrollArea>
#include <QStringList>

#include "ContactForm.h"
#include "RegEx.h"
#include "Exceptions.h"
#include "ChoosePic.h"


ContactForm::ContactForm( Contact& contact, QWidget* parent ) : QWidget( parent )
{
  this->contact = contact;
  this->setWindowIcon( QIcon( "images/contact.png" ) );
  this->setWindowTitle( QString( "%1 Contact" ).arg( this->contact.cnameGetter().isEmpty() ? "Add" : "Edit" ) );
  this->setFixedSize( 370, 400 );
  this->setWindowModality( Qt::WindowModal );

  this->image_name = this->contact.cimageGetter();
  if (!this->image_name.isEmpty())
    this->image = QPixmap( this->image_name );
  else
  {
    QRandomGenerator r = QRandomGenerator( QRandomGenerator::securelySeeded() );
    this->image_name = QString( "images/contact_icon_%1.png" ).arg( r.bounded( 10 ) );
    this->image = QPixmap( image_name );
    this->contact.imageSetter( image_name );
  }

  this->name_edit->setText( this->contact.cnameGetter() );
  this->surname_edit->setText( this->contact.surnameGetter() );
  this->middle_edit->setText( this->contact.cmiddleGetter() );
  this->address_edit->setText( this->contact.caddressGetter() );
  this->email_edit->setText( this->contact.cemailGetter() );

  if (this->contact.cbirthGetter().isValid())
    this->birth->setText( this->contact.csbirthGetter() );

  for (auto& i : this->contact.cphonesGetter())
  {
    QLineEdit* phone = new QLineEdit();
    this->phones->addWidget( phone );
    phone->setMaxLength( 24 );
    phone->setPlaceholderText( "+Xxx XXXxx XXXxx XX XX" );
    phone->setText( i );

    connect( phone, &QLineEdit::textChanged, this, [this, phone]() {addPhone( phone ); } );
    connect( phone, &QLineEdit::textChanged, this, [=]() {checkPhoneExp( this->phone, this->phones ); } );
  }

  QVBoxLayout* vbox = new QVBoxLayout( this );

  this->image = this->image.scaledToHeight( 100 );
  this->image = this->image.scaledToWidth( 100 );

  pic_label->setPixmap( this->image );

  QVBoxLayout* pics = new QVBoxLayout();
  QPushButton* edit_pic = new QPushButton( "Change", this );
  pics->addWidget( pic_label, 0, Qt::AlignHCenter | Qt::AlignTop );
  pics->addStretch( 1 );
  pics->addWidget( edit_pic, 0, Qt::AlignCenter );


  connect( edit_pic, &QPushButton::clicked, this, [=]() {choosePic( pic_label ); } );


  QHBoxLayout* hbox_upper = new QHBoxLayout();
  hbox_upper->addLayout( pics );
  hbox_upper->addSpacing( 11 );

  QVBoxLayout* form_box = new QVBoxLayout();
  form_box->addStretch( 1 );
  QFormLayout* form_first = new QFormLayout();

  form_first->setLabelAlignment( Qt::AlignLeft );
  form_first->setFormAlignment( Qt::AlignHCenter | Qt::AlignTop );
  form_first->setRowWrapPolicy( QFormLayout::DontWrapRows );


  QHBoxLayout* name_box = new QHBoxLayout();
  QPushButton* name_but = new QPushButton();
  name_edit->setPlaceholderText( "Nikolai" );
  name_but->setCheckable( true );
  name_but->setFixedSize( name_edit->height() - 7, name_edit->height() - 7 );
  name_box->addWidget( name_edit );
  name_box->addWidget( name_but );
  form_first->addRow( this->name, name_box );

  connect( name_edit, &QLineEdit::textChanged, this, [=]() {checkRegExp( this->name, name_edit, Name ); } );


  surname_edit->setPlaceholderText( "Rimsky-Korsakov" );
  middle_edit->setPlaceholderText( "Andreyevich" );

  form_first->addRow( surname_label, surname_edit );
  form_first->addRow( middle, middle_edit );

  hideButtonPressed( name_but, surname_label, surname_edit, middle, middle_edit );
  connect( name_but, &QPushButton::toggled, this, [=]() {hideButtonPressed( name_but, surname_label, surname_edit, middle, middle_edit ); } );
  connect( surname_edit, &QLineEdit::textChanged, this, [=]() {checkRegExp( surname_label, surname_edit, Name ); } );
  connect( middle_edit, &QLineEdit::textChanged, this, [=]() {checkRegExp( middle, middle_edit, Name ); } );

  QSlider* birth_date = new QSlider( Qt::Horizontal );
  birth_date->setMaximum( date->daysTo( QDate::currentDate() ) );
  birth_date->setMinimum( 0 );
  birth_date->setSliderPosition( birth_date->maximum() - this->contact.cbirthGetter().daysTo( QDate::currentDate() ) );

  QVBoxLayout* birth_box = new QVBoxLayout();
  birth_box->addWidget( birth, 0, Qt::AlignHCenter );
  birth_box->addWidget( birth_date );
  form_first->addRow( "Birth date", birth_box );

  connect( birth_date, &QSlider::valueChanged, this, [=]( int value ) { birth->setText( date->addDays( value ).toString( "dd.MM.yyyy" ) ); } );

  form_box->addLayout( form_first );
  form_box->addStretch( 1 );


  hbox_upper->addLayout( form_box );


  form_second->setLabelAlignment( Qt::AlignLeft );
  form_second->setFormAlignment( Qt::AlignLeft | Qt::AlignTop );
  form_second->setRowWrapPolicy( QFormLayout::DontWrapRows );

  email_edit->setPlaceholderText( "example@at.com" );
  form_second->addRow( email, email_edit );
  connect( email_edit, &QLineEdit::textChanged, this, [=]() {checkRegExp( email, email_edit, Email ); } );

  form_second->addRow( address, address_edit );
  address_edit->setPlaceholderText( "Saint Petersburg, Bolshoy pr., 14" );
  connect( address_edit, &QLineEdit::textChanged, this, [=]() {checkRegExp( address, address_edit, Address ); } );


  QLineEdit* phones_1 = new QLineEdit();
  phones_1->setMaxLength( 24 );
  phones_1->setPlaceholderText( "+Xxx XXXxx XXXxx XX XX" );
  phones->addWidget( phones_1 );

  QWidget* widget = new QWidget( this );
  QScrollArea* scroll = new QScrollArea();


  this->phones->setSizeConstraint( QLayout::SetMinAndMaxSize ); // it's needed (hard to express in words)
  widget->setLayout( phones );
  scroll->setWidget( widget );
  scroll->setWidgetResizable( true ); // to make the `QLineEdit` fill the whole width
  scroll->setMinimumSize( phones_1->width(), phones_1->height() * 6 ); // 5 visible on the area
  scroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  scroll->setFrameStyle( QFrame::NoFrame );


  form_second->addRow( phone, scroll );

  connect( phones_1, &QLineEdit::textChanged, this, [this, phones_1]() {addPhone( phones_1 ); } );
  connect( phones_1, &QLineEdit::textChanged, this, [=]() {checkPhoneExp( phone, phones ); } );

  QHBoxLayout* hbox_lower = new QHBoxLayout();
  hbox_lower->addLayout( form_second, 1 );

  vbox->addLayout( hbox_upper );
  vbox->addSpacing( 11 );
  vbox->addLayout( hbox_lower );
  vbox->addStretch( 1 );

  QHBoxLayout* buttons_layout = new QHBoxLayout();
  QPushButton *cancel_but = new QPushButton( "Cancel" );
  buttons_layout->addStretch( 1 );
  buttons_layout->addWidget( save_but );
  buttons_layout->addWidget( cancel_but );
  buttons_layout->addStretch( 1 );


  // to hide back when the window is closed through `cancel_but`
  connect( cancel_but, &QPushButton::clicked, this, [=]() { name_but->setChecked( false ); hideButtonPressed( name_but, surname_label, surname_edit, middle, middle_edit ); } );
  connect( cancel_but, &QPushButton::clicked, this, &ContactForm::cancel );

  if (name_edit->text().isEmpty() || qobject_cast<QLineEdit*>(phones->itemAt( 0 )->widget())->text().isEmpty() || QColor( "red" ) == phone->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->name->palette().color( QPalette::WindowText ))
    save_but->setDisabled( true );
  connect( save_but, &QPushButton::clicked, this, &ContactForm::save );

  connect( this, &ContactForm::check, this, &ContactForm::checkSignal );

  vbox->addLayout( buttons_layout );
  vbox->addStretch( 1 );

  this->setLayout( vbox );

  connect( this, &ContactForm::closing, this, [=]() { name_but->setChecked( false ); hideButtonPressed( name_but, surname_label, surname_edit, middle, middle_edit ); cancel(); } );
}


void ContactForm::addPhone( QLineEdit* line )
{
  QLineEdit* item = qobject_cast<QLineEdit*>(this->phones->itemAt( this->phones->count() - 1 )->widget());
  if (line->text().isEmpty())
    line->setParent( Q_NULLPTR );
  else if (!item->text().isEmpty())
  {
    QLineEdit* phone = new QLineEdit();
    this->phones->addWidget( phone );
    phone->setMaxLength( 24 );
    phone->setPlaceholderText( "+Xxx XXXxx XXXxx XX XX" );

    connect( phone, &QLineEdit::textChanged, this, [this, phone]() {addPhone( phone ); } );
    connect( phone, &QLineEdit::textChanged, this, [=]() {checkPhoneExp( this->phone, this->phones ); } );
  }
}


void ContactForm::hideButtonPressed( QPushButton* but, QLabel* label1, QLineEdit* line1, QLabel* label2, QLineEdit* line2 )
{
  if (but->isChecked())
  {
    but->setText( u8"▲" );
    label1->show();
    line1->show();
    label2->show();
    line2->show();
  }
  else
  {
    but->setText( u8"▼" );
    label1->hide();
    line1->hide();
    label2->hide();
    line2->hide();
  }
}



void ContactForm::checkRegExp( QLabel* label, QLineEdit* line, Mode mode )
{
  bool flag = false;
  QString s = line->text();
  switch (mode)
  {
    case Name:
      flag = nameExp( s );
      break;
    case Email:
      flag = emailExp( s );
      break;
    case Address:
      flag = addressExp( s );
      break;
  }
  label->setStyleSheet( QString( "QLabel { color : %1 }" ).arg( flag || line->text().isEmpty() ? "black" : "red" ) );

  emit check();
}


void ContactForm::checkPhoneExp( QLabel* label, QVBoxLayout* layout )
{
  bool flag = true;
  QString s;
  for (int i = 0; i < layout->count(); i++)
  {
    QLineEdit* item = qobject_cast<QLineEdit*>(layout->itemAt( i )->widget());
    s = item->text();
    if (!phoneExp( s ) && !s.isEmpty())
    {
      flag = false;
      break;
    }
  }
  label->setStyleSheet( QString( "QLabel { color : %1 }" ).arg( flag ? "black" : "red" ) );

  emit check();
}


void ContactForm::choosePic( QLabel* pic )
{
  QDialog* dialog = new QDialog( this );
  dialog->setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
  dialog->setAttribute( Qt::WA_TranslucentBackground );
  dialog->setModal( true );
  QGridLayout* grid = new QGridLayout( dialog );

  QVector<QVector<int>> pos( { {0, 3}, {1, 4}, {1, 2}, {2, 1}, {2, 3}, {2, 5}, {3, 0}, {3, 2}, {3, 4}, {3, 6} } );
  for (int i = 0; i < 10; i++)
  {
    QString name = QString( "images/contact_icon_%1.png" ).arg( i );
    QPixmap image( name ), img( image );
    image = image.scaledToHeight( 80 );
    image = image.scaledToWidth( 80 );
    img = img.scaledToHeight( 100 );
    img = img.scaledToWidth( 100 );
    ChoosePic* label = new ChoosePic( dialog );
    label->setPixmap( image );
    grid->addWidget( label, pos[i][0], pos[i][1], 1, 1 );
    label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    connect( label, &ChoosePic::clicked, this, [=]() { pic->setPixmap( img ); this->image_name = name; dialog->close(); } );
  }

  dialog->show();
}


void ContactForm::save( void )
{
  this->contact.nameSetter( name_edit->text() );
  this->contact.surnameSetter( surname_edit->text() );
  this->contact.middleSetter( middle_edit->text() );
  this->contact.emailSetter( email_edit->text() );
  this->contact.addressSetter( address_edit->text() );
  this->contact.birthSetter( birth->text() );

  this->contact.phonesSetter( QStringList() );
  for (int i = 0; i < phones->count(); i++)
    this->contact.phonesSetter( qobject_cast<QLineEdit*>(phones->itemAt( i )->widget())->text() );

  this->contact.imageSetter( this->image_name );

  this->close();
  emit saved( &this->contact );
}


void ContactForm::cancel( void )
{
  this->name_edit->setText( this->contact.cnameGetter() );
  this->surname_edit->setText( this->contact.csurnameGetter() );
  this->middle_edit->setText( this->contact.cmiddleGetter() );
  this->birth->setText( this->contact.csbirthGetter() );
  this->email_edit->setText( this->contact.cemailGetter() );
  this->address_edit->setText( this->contact.caddressGetter() );

  int c{};
  for (auto& i : this->contact.cphonesGetter())
  {
    if (this->phones->count() > c)
      qobject_cast<QLineEdit*>(this->phones->itemAt( c )->widget())->setText( i );
    else
    {
      QLineEdit* phone = new QLineEdit();
      this->phones->addWidget( phone );
      phone->setMaxLength( 24 );
      phone->setText( i );

      connect( phone, &QLineEdit::textChanged, this, [this, phone]() {addPhone( phone ); } );
      connect( phone, &QLineEdit::textChanged, this, [=]() {checkPhoneExp( this->phone, this->phones ); } );
    }
    c++;
  }

  this->image_name = this->contact.cimageGetter();
  this->image = QPixmap(this->image_name);
  this->image = this->image.scaledToHeight( 100 );
  this->image = this->image.scaledToWidth( 100 );
  this->pic_label->setPixmap(this->image);

  this->close();
}


void ContactForm::checkSignal()
{
  if (this->name_edit->text().isEmpty() || qobject_cast<QLineEdit*>(this->phones->itemAt( 0 )->widget())->text().isEmpty() || QColor( "red" ) == phone->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->name->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->surname_label->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->middle->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->address->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->email->palette().color( QPalette::WindowText ) || QColor( "red" ) == this->birth->palette().color( QPalette::WindowText ))
    this->save_but->setDisabled( true );
  else
    this->save_but->setDisabled( false );
}


void ContactForm::closeEvent( QCloseEvent *event )
{
  event->accept();
  emit closing();
}

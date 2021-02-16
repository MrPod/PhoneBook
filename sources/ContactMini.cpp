#include <QHBoxLayout>
#include <QPixmap>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QFrame>
#include <QFont>

#include "ContactMini.h"
#include "Exceptions.h"
#include "Comp.h"


ContactMini::ContactMini( Contact& contact, QWidget* parent ) : QWidget( parent )
{
  this->contact = contact;

  ContactForm* form = new ContactForm( contact );

  QHBoxLayout* hbox = new QHBoxLayout( this );
  widget_frame->setFrameStyle( QFrame::Box );
  widget_frame->setLayout( hbox );
  QVBoxLayout* image_box = new QVBoxLayout();
  QVBoxLayout* lines = new QVBoxLayout();
  QHBoxLayout* lb = new QHBoxLayout();
  QVBoxLayout* buttons = new QVBoxLayout();
  QHBoxLayout* buttons_line = new QHBoxLayout();

  name->setText( this->contact.cnameGetter() );
  phone->setText( this->contact.cphonesGetter()[0] );
  QPixmap image( this->contact.imageGetter() );
  image = image.scaledToHeight( 50 );
  image = image.scaledToWidth( 50 );
  image_label->setPixmap( image );

  image_box->addSpacing( -5 );
  image_box->addWidget( image_label );

  this->setMinimumWidth( 250 );
  this->setMinimumSize( this->minimumWidth(), 74 );
  widget_frame->setFixedSize( this->minimumSize() );

  QFont font( name->font() );
  font.setPointSize( 10 );
  name->setFont( font );
  font.setPointSize( 12 );
  phone->setFont( font );
  name->setText( this->contact.cnameGetter() );
  phone->setText( this->contact.cphonesGetter()[0] );


  QPushButton* edit = new QPushButton( );
  edit->setIcon(QIcon( "images/pencil.png"));
  edit->setFixedSize( name->height() / 1.5, name->height() / 1.5 );
  edit->setIconSize( QSize(edit->height() - 10, edit->width() - 10 ));
  del->setFixedSize( name->height() / 1.5, name->height() / 1.5 );
  buttons_line->addWidget( edit, 0, Qt::AlignTop );
  buttons_line->addWidget( del, 0, Qt::AlignTop );

  lb->addWidget( name, 1, Qt::AlignLeft );
  lb->addStretch( 1 );
  lb->addLayout( buttons_line, 1 );

  lines->addLayout( lb );
  lines->addWidget( phone, 1, Qt::AlignLeft );
  lines->addStretch( 1 );

  hbox->addSpacing( -5 );
  hbox->addLayout( image_box );
  hbox->addLayout( lines );

  connect( edit, &QPushButton::clicked, this, [=]() { form->show(); } );
  connect( form, &ContactForm::saved, this, &ContactMini::Alter );
  connect( del, &QPushButton::clicked, this, [=]() { emit delet(); } );
}


void Sort( QList<ContactMini*>& list, Contact::Field f )
{
  std::sort( list.begin(), list.end(), Comp_M(f) );
}


//bool ContactMini::operator==( const ContactMini& c )
//{
//  return this->contact == c.contact;
//}
//
//
//void ContactMini::operator= ( const ContactMini& c )
//{
//  this->contact = c.contact;
//}
//
//
//ContactMini::ContactMini( const ContactMini&& c )
//{
//  *this = c;
//}
//
//
//ContactMini::ContactMini( const ContactMini& c )
//{
//  *this = c;
//}


void ContactMini::Alter( Contact* c )
{
  emit saved(false);

  this->contact = *c;
  this->name->setText( this->contact.cnameGetter() );
  this->phone->setText( this->contact.cphonesGetter()[0] );
  QPixmap im( this->contact.cimageGetter() );
  im = im.scaledToHeight( 50 );
  im = im.scaledToWidth( 50 );
  this->image_label->setPixmap( im );

  emit saved(true);
}

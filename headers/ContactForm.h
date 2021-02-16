#pragma once

#include <QWidget>
#include <QLabel>
#include <QDate>
#include <QBoxLayout>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QFormLayout>
#include <QPushButton>
#include <QPixmap>
#include <QCloseEvent>

#include "Contact.h"


class ContactForm : public QWidget, protected Contact
{
  Q_OBJECT


public:
  ContactForm( Contact& contact, QWidget* parent = Q_NULLPTR );


private:
  Contact contact;
  QString image_name;
  QPixmap image;
  QLineEdit* name_edit = new QLineEdit( this );
  QLineEdit* surname_edit = new QLineEdit( this );
  QLineEdit* middle_edit = new QLineEdit( this );
  QLineEdit* email_edit = new QLineEdit( this );
  QLineEdit* address_edit = new QLineEdit( this );

  QDate* date = new QDate( 1903, 12, 14 );

  QLabel* pic_label = new QLabel( this );
  QLabel* name = new QLabel( "Name *" );
  QLabel* surname_label = new QLabel( "Surname" );
  QLabel* middle = new QLabel( "Middle name" );
  QLabel* phone = new QLabel( "Phones *", this );
  QLabel* birth = new QLabel( date->toString( "dd.MM.yyyy" ) );
  QLabel* email = new QLabel( "Email", this );
  QLabel* address = new QLabel( "Address", this );
  QVBoxLayout* phones = new QVBoxLayout();
  QFormLayout* form_second = new QFormLayout();

  QPushButton* save_but = new QPushButton( "Save" );
  enum Mode { Name, Email, Address, Phone, Date };

signals:
  void check();
  void saved(Contact* contact);
  void closing();

public slots:
  void closeEvent( QCloseEvent *event );
  void addPhone( QLineEdit* line );
  void hideButtonPressed( QPushButton* but, QLabel* label1, QLineEdit* line1, QLabel* label2, QLineEdit* line2 );
  void checkRegExp( QLabel* label, QLineEdit* line, Mode mode );
  void checkPhoneExp( QLabel* label, QVBoxLayout* layout );
  void choosePic( QLabel* pic );
  void save( void );
  void cancel( void );
  void checkSignal();
};
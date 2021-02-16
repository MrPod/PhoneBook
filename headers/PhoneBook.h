#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSet>
#include <QList>
#include <QStringList>
#include <QComboBox>
#include <QString>

#include "ContactMini.h"
#include "Contact.h"


class PhoneBook : public QMainWindow
{
  Q_OBJECT

public:
  PhoneBook( QWidget *parent = Q_NULLPTR );

private:
  QSet<Contact> set;
  QList<ContactMini*> list;
  QLineEdit* search = new QLineEdit();
  QVBoxLayout* vbox = new QVBoxLayout();
  QStringList internals = { "Name", "Surname", "Birth date", "Email", "Phone", "Middle name", "Address" };
  QComboBox* mode = new QComboBox();

  QPushButton* find = new QPushButton( u8"Find ▼" );
  QLabel* city = new QLabel( "Region" );
  QLabel* age = new QLabel( "Age" );
  QLabel* domain = new QLabel( "Email" );
  QLineEdit* city_edit = new QLineEdit();
  QLineEdit* age_edit_low = new QLineEdit();
  QLabel* separator = new QLabel( "--" );
  QLineEdit* age_edit_high = new QLineEdit();
  QLineEdit* domain_edit = new QLineEdit();

  void fix( void );
  void Regroup( void );
  void Sample( void );

public slots:
  void deleteContact( QVBoxLayout* contacts, ContactMini* contact );
  void save( void );
  void load( void );
  void Handler( void );
  void clear( void );
  void Add( Contact* c );
  void Altered( bool state, ContactMini* mini );
  void hideFind( void );
  void Validation( QLineEdit* line, QIntValidator* valid, QString& str );
  void Check( void );
  void clearFilters( void );
};

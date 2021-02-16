#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFrame>
#include <QList>

#include "Contact.h"
#include "ContactForm.h"

class ContactMini : public QWidget, protected Contact
{
  Q_OBJECT

private:
  QFrame* widget_frame = new QFrame( this );
  QLabel* name = new QLabel(this);
  QLabel* phone = new QLabel(this);
  QLabel* image_label = new QLabel( this );
  QPushButton* del = new QPushButton( u8"✖" );

signals:
  void delet( void );
  void saved( bool );

public:
  Contact contact;

  ContactMini( Contact& contact, QWidget* parent = Q_NULLPTR );
  friend void Sort( QList<ContactMini*>& list, Contact::Field f );

private slots:
  void Alter( Contact* c );
};
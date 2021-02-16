#pragma once
#include <QString>

#include "Contact.h"

class Contacts : public Contact
{
private:
  QVector<Contact> contacts;
public:

  void Add(const Contact& contact );
  virtual void Out();
  void Remove( const int number ) noexcept(false);
  void Write( QString filename ) noexcept(false);
  void Read( QString filename ) noexcept(false);
};
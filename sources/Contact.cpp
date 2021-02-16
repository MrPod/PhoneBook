#include <algorithm>
#include <QtAlgorithms>
#include <set>

#include "Contact.h"
#include "Exceptions.h"
#include "RegEx.h"
#include "Comp.h"
#include "AdditionalFuncs.h"


Contact::Contact()
{
  this->image = "";
  this->name = "";
  this->surname = "";
  this->middle_name = "";
  this->address = "";
  this->email = "";
  this->birth = QDate( 1903, 12, 14 );
  this->phones = QStringList();
}


void Sort( QList<Contact>& folks, Contact::Field f )
{
  std::sort( folks.begin(), folks.end(), Comp(f) );
}


bool Contact::operator==( const Contact& c ) const
{
  return (this->cnameGetter() == c.cnameGetter() &&
          this->csurnameGetter() == c.csurnameGetter() &&
          this->caddressGetter() == c.caddressGetter() &&
          this->cmiddleGetter() == c.cmiddleGetter() &&
          this->cbirthGetter() == c.cbirthGetter() &&
          this->cemailGetter() == c.cemailGetter() &&
          this->cphonesGetter() == c.cphonesGetter());
}


uint qHash( const Contact &c, uint seed )
{
  QStringList list = { c.cnameGetter(), c.csurnameGetter(), c.caddressGetter(), c.csbirthGetter(), c.cemailGetter(), c.cmiddleGetter() };
  list += c.cphonesGetter();
  uint num{};

  for (auto& i : list)
    num ^= qHash(i, seed);

  return num;
}


void Contact::Out( void )
{
  qDebug() << "\nImage loc: " << this->image << "\nName: " << this->name << "\nSurname: " << this->surname << "\nMiddle name: " << this->middle_name << "\nAddress: " << this->address << "\nEmail: " << this->email << "\nBirth: " << this->birth.toString( "dd.MM.yyyy" ) << "\nPhones: ";

  for (auto& i : this->phones)
    qDebug() << "\t" << i;

  qDebug() << "\n";
}


void Contact::clear( void )
{
  this->image = "";
  this->name = "";
  this->surname = "";
  this->middle_name = "";
  this->address = "";
  this->email = "";
  this->birth.setDate( 0, 0, 0 );
  this->phones.clear();
}


void Contact::imageSetter( const QString& string )
{
  this->image = string;
}


void Contact::nameSetter( QString& string )
{
  if (!nameExp( string ))
    throw RegExpNotMatch();
  else
    this->name = string;
}


void Contact::surnameSetter( QString& string )
{
  if (!nameExp( string ) && !string.isEmpty())
    throw RegExpNotMatch();
  else
    this->surname = string;
}


void Contact::middleSetter( QString& string )
{
  if (!nameExp( string ) && !string.isEmpty())
    throw RegExpNotMatch();
  else
    this->middle_name = string;
}


void Contact::addressSetter( QString& string )
{
  if (!addressExp( string ) && !string.isEmpty())
    throw RegExpNotMatch();
  else
    this->address = string;
}


void Contact::emailSetter( QString& string )
{
  if (!emailExp( string ) && !string.isEmpty())
    throw RegExpNotMatch();
  else
    this->email = string;
}


void Contact::birthSetter( QDate& date )
{
  if (date.isValid())
    this->birth = date;
  else
  {
    //TODO: do something
    date = date;
  }
}


void Contact::birthSetter( QString& string )
{
  try
  {
    this->birth = dateExp( string );
  }
  catch (RegExpNotMatch)
  {
    //TODO: do something
    string = string;
  }
  catch (InvalidDate)
  {
    //TODO: do something
    string = string;
  }
}



void Contact::phonesSetter( QString& string )
{
  if (!phoneExp( string ) && !string.isEmpty())
    throw RegExpNotMatch();
  else if (!string.isEmpty())
    this->phones.append( string );
}


void Contact::phonesSetter( QStringList& list )
{
  this->phones.clear();
  for (auto& i : list)
    if (!phoneExp( i ) && !i.isEmpty())
      throw RegExpNotMatch();
    else
      this->phones.append( i );
}


QString& Contact::imageGetter()
{
  return this->image;
}


QString& Contact::nameGetter()
{
  return this->name;
}


QString& Contact::surnameGetter()
{
  return this->surname;
}


QString& Contact::middleGetter()
{
  return this->middle_name;
}


QString& Contact::addressGetter()
{
  return this->address;
}


QString& Contact::emailGetter()
{
  return this->email;
}


QDate& Contact::birthGetter()
{
  return this->birth;
}


QString Contact::sbirthGetter()
{
  return this->birth.toString("dd.MM.yyyy");
}

QStringList& Contact::phonesGetter()
{
  return this->phones;
}


const QString& Contact::cimageGetter() const
{
  return this->image;
}


const QString& Contact::cnameGetter() const
{
  return this->name;
}


const QString& Contact::csurnameGetter() const
{
  return this->surname;
}


const QString& Contact::cmiddleGetter() const
{
  return this->middle_name;
}


const QString& Contact::caddressGetter() const
{
  return this->address;
}


const QString& Contact::cemailGetter() const
{
  return this->email;
}


const QDate& Contact::cbirthGetter() const
{
  return this->birth;
}


const QString Contact::csbirthGetter() const
{
  return this->birth.toString("dd.MM.yyyy");
}


const QStringList& Contact::cphonesGetter() const
{
  return this->phones;
}

#include <QFile>

#include "Contacts.h"
#include "Exceptions.h"


void Contacts::Write( QString filename )
{
  QFile file( filename );

  if (!file.open( QIODevice::WriteOnly | QIODevice::Text ))
    throw FileNotOpened( file.errorString() );

  QTextStream in( &file );
  for (auto ptr : contacts)
  {
    in << ptr.nameGetter() << "\n" << ptr.surnameGetter() << "\n" << ptr.middleGetter() << "\n" << ptr.addressGetter() << "\n" << ptr.emailGetter() << "\n" << ptr.birthGetter().toString( "dd.MM.yyyy" ) << "\n";

    for (auto& i : ptr.phonesGetter())
      in << i << "\n";
    in << "\n";
  }
  file.close();
}


void Contacts::Read( QString filename )
{
  QFile file( filename );

  if (!file.open( QIODevice::ReadOnly | QIODevice::Text ))
    throw FileNotOpened( file.errorString() );

  QTextStream out( &file );
  QString string;
  Contact contact;

  while (!out.atEnd())
  {
    contact.nameSetter( out.readLine() );
    contact.surnameSetter( out.readLine() );
    contact.middleSetter( out.readLine() );
    contact.addressSetter( out.readLine() );
    contact.emailSetter( out.readLine() );
    contact.birthSetter( out.readLine() );
    while ((string = out.readLine()).size())
      contact.phonesSetter( string );

    contacts.append( contact );
    contact.clear();
  }
  file.close();
}


void Contacts::Add( const Contact& contact )
{
  contacts.append( contact );
}


void Contacts::Remove( const int number )
{
  if (number >= contacts.size())
    throw OutOfBoundaries();
  contacts.remove(number);
}


void Contacts::Out()
{
  for (auto i : contacts)
    i.Out();
}
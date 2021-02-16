#include <QRandomGenerator>

#include "AdditionalFuncs.h"

QRandomGenerator r = QRandomGenerator( QRandomGenerator::securelySeeded() );


int IntGen( int u, int d )
{
  return r.bounded( d, u );
}


QString StringGen( int length )
{
  QString string;
  for (int i = 0; i < length; i++)
    string += static_cast<char>(r.bounded( 26 )) + 65 + 32 * static_cast<bool>(r.bounded( 2 ));
  return string;
}


QString AddressGen()
{
  QString string;
  return StringGen( 10 ) + ", " + StringGen( 10 ) + " " + StringGen( 5 ) + "., " + string.setNum( r.bounded( 99 ) + 1 );
}


QString EmailGen()
{
  return StringGen( 10 ) + "@" + StringGen( 6 ) + "." + StringGen( 3 );
}


QDate DateGen()
{
  int year = r.bounded( 1905, QDate::currentDate().year() ) + 1, month = r.bounded( 12 ) + 1;
  QVector<int> validMonth( { 31, 28 + QDate::isLeapYear( year ), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } );
  int day = r.bounded( validMonth[month - 1] ) + 1;
  return QDate( year, month, day );
}


QStringList PhonesGen( int amount )
{
  QString string, helper;
  QStringList list;
  for (int i = 0; i < amount; i++)
  {
    string += '+';
    string += helper.setNum(r.bounded( 99 ) + 1);
    string += " (";
    string += helper.setNum( r.bounded( 999 ) + 1 ).rightJustified( 3, '0' );
    string += ") ";
    string += helper.setNum( r.bounded( 999 ) + 1 ).rightJustified( 3, '0' );
    string += ' ';
    string += helper.setNum( r.bounded( 99 ) + 1 ).rightJustified( 2, '0' );
    string += ' ';
    string += helper.setNum( r.bounded( 99 ) + 1 ).rightJustified( 2, '0' );

    list.append( string );
    string.clear();
  }
  return list;
}

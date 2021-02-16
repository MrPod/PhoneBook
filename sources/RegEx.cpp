#include <QtCore>

#include "RegEx.h"
#include "Exceptions.h"


bool nameExp( QString& string )
{
  QRegExp r( R"((?:[\w\d]+[- ]?[\w\d]+|[\w\d]+)+)" );

  if (!r.exactMatch( string ))
    return false;;
  return true;
}


bool emailExp( QString& string )
{
  QRegExp r( R"((\S+|".*")@(?:\w+\.+\w+)+)" );

  return r.exactMatch( string );
}


bool phoneExp( QString& string )
{
  QRegExp r( R"((?:\(\+\d{1,3}\)|\+\d{1,3})[ -]?(?:\(?\d{3,5}\)?|\d{3,5})[ -]?\d{3,5}[ -]?(?:\d{4}|\d{2}[- ]?\d{2}))" );
  return r.exactMatch( string );
}


bool addressExp( QString& string )
{
  QRegExp r( R"((?:\w+[- ]?\w+|\w+)+, [A-Za-z]+ [A-Za-z]+\., \d+)" ); // city, street str., #
  return r.exactMatch( string);
}


QDate dateExp( QString& string )
{
  QRegExp r( R"((?:(\d{2})[ -\.](\d{2})[ -\.](\d{4})|(\d{4})[ -\.](\d{2})[ -\.](\d{2})))" );
  if (!r.exactMatch( string ))
    throw RegExpNotMatch();

  int first, second, year;

  first = (r.cap( 1 ) + r.cap( 6 )).toInt();
  second = (r.cap( 2 ) + r.cap( 5 )).toInt();
  year = (r.cap( 3 ) + r.cap( 4 )).toInt();

  QVector<int> validMonth( { 31, 28 + QDate::isLeapYear( year ), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } );

  if (year <= QDate::currentDate().year())
  {
    // dd mm yyyy && yyyy mm dd are more prioritized then mm dd yyyy && yyyy dd mm
    if (second < 13 && validMonth[second - 1] >= first)
      return QDate( year, second, first );
    else if (first < 13 && validMonth[first - 1] >= second)
      return QDate( year, first, second );
  }
  throw InvalidDate();
}


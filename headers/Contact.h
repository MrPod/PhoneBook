#pragma once

#include <QString>
#include <QDate>
#include <QStringList>
#include <QList>


class Contact
{
private:
  QString image;
  QString name;
  QString surname;
  QString middle_name;
  QString address;
  QString email;
  QDate birth;
  QStringList phones;

public:
  enum Field { Image, Name, Surname, Birth, Email, Phones, Middle, Address };
  Contact();

  void Out( void );
  void clear( void );

  friend void Sort( QList<Contact>& folks, Field field );

  bool operator==( const Contact& c ) const;
  friend uint qHash( const Contact &c, uint seed );

  void imageSetter( const QString& string );
  void nameSetter( QString& string );
  void surnameSetter( QString& string );
  void middleSetter( QString& string );
  void addressSetter( QString& string );
  void emailSetter( QString& string );
  void birthSetter( QDate& date );
  void birthSetter( QString& string );
  void phonesSetter( QString& string );
  void phonesSetter( QStringList& list );


  QString& imageGetter();
  QString& nameGetter();
  QString& surnameGetter();
  QString& middleGetter();
  QString& addressGetter();
  QString& emailGetter();
  QDate& birthGetter();
  QString sbirthGetter();
  QStringList& phonesGetter();


  const QString& cimageGetter() const;
  const QString& cnameGetter() const;
  const QString& csurnameGetter() const;
  const QString& cmiddleGetter() const;
  const QString& caddressGetter() const;
  const QString& cemailGetter() const;
  const QDate& cbirthGetter() const;
  const QString csbirthGetter() const;
  const QStringList& cphonesGetter() const;
};

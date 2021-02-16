#pragma once
#include <QDebug>

class KeyExc
{
public:
  KeyExc() { qDebug() << "\nWrong Key to the Sort func was passed\n"; }
};


class RegExpNotMatch
{
public:
  RegExpNotMatch() { qDebug() << "\nRegular Expression didn't match\n"; }
};


class InvalidDate
{
public:
  InvalidDate() { qDebug() << "\nThe input Date was invalid\n"; }
};


class FileNotOpened
{
public:
  FileNotOpened(QString& error) { qDebug() << "\nFile wasn't opened because" << error; }
};


class OutOfBoundaries
{
public:
  OutOfBoundaries() { qDebug() << "\nYou've stepped out of boundaries\n"; }
};
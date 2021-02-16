#pragma once

#include <QString>
#include <QDate>
#include <QStringList>

int IntGen( int u, int d = 0 );
QString StringGen( int length );
QString AddressGen();
QString EmailGen();
QDate DateGen();
QStringList PhonesGen( int amount );

#include <QtCore> 
#include <QApplication>
#include <QWidget>
#include <QIcon>
#include <QDebug>
#include <QSizePolicy>

#include "PhoneBook.h"
#include "ContactForm.h"
#include "Contacts.h"
#include "AdditionalFuncs.h"
#include "ContactMini.h"


int main( int argc, char *argv[] )
{
  //TODO: shortcuts
  QApplication a( argc, argv );
  PhoneBook w;
  w.show();

  return a.exec();
}

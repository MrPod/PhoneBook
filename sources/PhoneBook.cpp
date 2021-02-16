#include <QVBoxLayout>
#include <QPixMap>
#include <QScrollArea>
#include <QWidget>
#include <QToolBar>
#include <QComboBox>
#include <QFrame>
#include <QFile>
#include <QSet>
#include <QMap>
#include <QValidator>
#include <QRegExp>
#include <QFileDialog>

#include "PhoneBook.h"
#include "Contact.h"
#include "Contacts.h"
#include "ContactMini.h"
#include "AdditionalFuncs.h"
#include "ContactForm.h"
#include "Exceptions.h"


PhoneBook::PhoneBook( QWidget *parent )
  : QMainWindow( parent )
{
  this->setWindowTitle( "Phone book" );
  this->setWindowIcon( QIcon( "images/phonebook_icon.png" ) );
  this->setFixedSize( 461, 408 );

  QPixmap savepix( "images/save_icon.png" );
  QPixmap loadpix( "images/load_icon.png" );
  QPixmap addpix( "images/add_icon.png" );
  QPixmap clearpix( "images/clear_icon.png" );

  QToolBar* toolbar = new QToolBar();
  this->addToolBar( toolbar );
  toolbar->setMovable( false );
  QAction* save_but = toolbar->addAction( QIcon( savepix ), "Save to File" );
  QAction* load_but = toolbar->addAction( QIcon( loadpix ), "Load from File" );
  toolbar->addSeparator();
  QAction* add = toolbar->addAction( QIcon( addpix ), "Add new Contact" );
  QAction* clear = toolbar->addAction( QIcon( clearpix ), "Clear all Records" );

  connect( add, &QAction::triggered, this, &PhoneBook::Handler );
  connect( save_but, &QAction::triggered, this, &PhoneBook::save );
  connect( load_but, &QAction::triggered, this, &PhoneBook::load );
  connect( clear, &QAction::triggered, this, &PhoneBook::clear );

  QWidget* mainwidget = new QWidget( this );
  this->setCentralWidget( mainwidget );
  QVBoxLayout* vlayout = new QVBoxLayout( mainwidget );

  search->setParent( mainwidget );
  search->setMaximumHeight( 20 );
  search->move( 5, 0 );
  search->setPlaceholderText( "Name or Phone" );

  QRegExpValidator* search_valid = new QRegExpValidator( QRegExp( R"((?:(?:\w+[ -]?\w+|\w+)+|(?:\(\+\d{1,3}\)|\+\d{1,3})[ -]?(?:\(?\d{3,5}\)?|\d{3,5})[ -]?\d{3,5}[ -]?(?:\d{4}|\d{2}[- ]?\d{2})))" ) );
  search->setValidator( search_valid );

  connect( search, &QLineEdit::textChanged, this, [=]() { Sample(); } );
  vlayout->addWidget( search );

  QHBoxLayout* hbox = new QHBoxLayout();

  QWidget* widget = new QWidget( mainwidget );
  QScrollArea* scroll = new QScrollArea();
  this->vbox->setSizeConstraint( QLayout::SetMinAndMaxSize );
  widget->setLayout( vbox );
  scroll->setWidget( widget );
  scroll->setWidgetResizable( true );
  scroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  scroll->setMinimumWidth( 287 );
  scroll->setMaximumWidth( 287 );

  hbox->addWidget( scroll );

  QVBoxLayout* settings = new QVBoxLayout();
  QLabel* sort = new QLabel( "Sort By" );
  this->mode->addItems( internals );

  this->find->setCheckable( true );
  QPushButton* clear_filters = new QPushButton( u8"✖" );
  clear_filters->setMaximumWidth( 25 );

  connect( this->find, &QPushButton::toggled, this, &PhoneBook::hideFind );
  connect( clear_filters, &QPushButton::clicked, this, &PhoneBook::clearFilters );

  connect( this->mode, &QComboBox::currentTextChanged, this, [=]() { fix(); } );

  this->city_edit->setPlaceholderText( "Saint Petersburg" );
  QIntValidator* valid = new QIntValidator( 0, 114 );
  this->age_edit_low->setPlaceholderText( "0" );
  this->age_edit_low->setValidator( valid );
  this->age_edit_high->setPlaceholderText( "114" );
  this->age_edit_high->setValidator( valid );
  this->domain_edit->setPlaceholderText( "example@at.com" );

  QRegExpValidator* regex_valid = new QRegExpValidator( QRegExp( R"((?:\w+[ -]?\w+|\w+)+)" ) );
  QRegExpValidator* regex_domain_valid = new QRegExpValidator( QRegExp( R"((\S+|".*")@(?:\w+\.+\w+)+)" ) );
  this->city_edit->setValidator( regex_valid );
  this->domain_edit->setValidator( regex_domain_valid );

  connect( this->city_edit, &QLineEdit::textChanged, this, [=]() { Sample(); } );
  connect( this->domain_edit, &QLineEdit::textChanged, this, [=]() { Sample(); } );
  connect( this->age_edit_low, &QLineEdit::textChanged, this, [=]() { Validation( this->age_edit_low, valid, this->age_edit_low->text() ); } );
  connect( this->age_edit_high, &QLineEdit::textChanged, this, [=]() { Validation( this->age_edit_high, valid, this->age_edit_high->text() ); } );
  connect( this->age_edit_low, &QLineEdit::editingFinished, this, &PhoneBook::Check );
  connect( this->age_edit_high, &QLineEdit::editingFinished, this, &PhoneBook::Check );

  QVBoxLayout* find_box = new QVBoxLayout();
  QHBoxLayout* find_stretchers = new QHBoxLayout();
  find_stretchers->addStretch( 1 );
  find_stretchers->addWidget( this->find );
  find_stretchers->addWidget( clear_filters );
  find_stretchers->addStretch( 1 );
  find_box->addLayout( find_stretchers );
  find_box->addWidget( this->city, 0, Qt::AlignLeft );
  find_box->addWidget( this->city_edit );
  find_box->addWidget( this->age, 0, Qt::AlignLeft );

  QHBoxLayout* age_edits = new QHBoxLayout();
  age_edits->addStretch( 1 );
  age_edits->addWidget( this->age_edit_low, 0, Qt::AlignLeft );
  age_edits->setSpacing( 2 );
  age_edits->addWidget( this->separator, 0, Qt::AlignCenter );
  age_edits->setSpacing( 2 );
  age_edits->addWidget( this->age_edit_high, 0, Qt::AlignRight );
  age_edits->addStretch( 1 );

  find_box->addLayout( age_edits );
  find_box->addWidget( this->domain, 0, Qt::AlignLeft );
  find_box->addWidget( this->domain_edit );

  settings->addWidget( sort, 0, Qt::AlignCenter );
  settings->addWidget( mode, 0, Qt::AlignCenter );
  settings->addLayout( find_box );
  settings->addStretch( 1 );

  hbox->addSpacing( 5 );
  hbox->addLayout( settings );
  vlayout->addLayout( hbox );

  hideFind();

  Contact contact;
  ContactMini* w;
  for (int i = 0; i < 10; i++)
  {
    contact.nameSetter( StringGen( 12 ) );
    contact.surnameSetter( StringGen( 11 ) );
    contact.middleSetter( StringGen( 10 ) );
    contact.addressSetter( AddressGen() );
    contact.emailSetter( EmailGen() );
    contact.birthSetter( DateGen() );
    contact.phonesSetter( PhonesGen( 4 ) );
    contact.imageSetter( QString( "images/contact_icon_%1.png" ).arg( IntGen( 10 ) ) );

    if (!this->set.contains( contact ))
    {
      w = new ContactMini( contact );
      this->vbox->addWidget( w );
      connect( w, &ContactMini::delet, this, [=]() { deleteContact( this->vbox, w ); } );
      connect( w, &ContactMini::saved, this, [=]( bool state ) { PhoneBook::Altered( state, w ); } );
      this->set += contact;
      this->list.append( w );
    }
    contact.clear();
  }
  fix();
}


void PhoneBook::deleteContact( QVBoxLayout* contacts, ContactMini* contact )
{
  contact->setParent( Q_NULLPTR );
  this->set -= contact->contact;
  this->list.removeOne( contact );
}


void PhoneBook::save( void )
{
  QString filename = QFileDialog::getSaveFileName( this, "Save to", "", "*.txt" );
  QFile file( filename );

  if (!filename.isEmpty())
  {
    if (!file.open( QIODevice::WriteOnly | QIODevice::Text ))
      throw FileNotOpened( file.errorString() );

    QTextStream in( &file );

    for (auto& contact : this->set)
    {
      in << contact.cimageGetter() << "\n" << contact.cnameGetter() << "\n" << contact.csurnameGetter() << "\n" << contact.cmiddleGetter() << "\n" << contact.caddressGetter() << "\n" << contact.cemailGetter() << "\n" << contact.csbirthGetter() << "\n";

      for (auto& i : contact.cphonesGetter())
        in << i << "\n";
      in << "\n";
    }
    file.close();
  }
}


void PhoneBook::load( void )
{
  QString filename = QFileDialog::getOpenFileName( this, "Load from", "", "*.txt" );
  QFile file( filename );

  if (!filename.isEmpty())
  {
    if (!file.open( QIODevice::ReadOnly | QIODevice::Text ))
      throw FileNotOpened( file.errorString() );

    QTextStream out( &file );
    QString string;
    Contact contact;
    ContactMini* mini;

    while (!out.atEnd())
    {
      try
      {
        contact.imageSetter( out.readLine() );
        contact.nameSetter( out.readLine() );
        contact.surnameSetter( out.readLine() );
        contact.middleSetter( out.readLine() );
        contact.addressSetter( out.readLine() );
        contact.emailSetter( out.readLine() );
        contact.birthSetter( out.readLine() );
        while ((string = out.readLine()).size())
          contact.phonesSetter( string );
      }
      catch (RegExpNotMatch)
      {
        QDialog* dialog = new QDialog( this );
        dialog->setModal( true );
        dialog->setWindowTitle( "Invalid file internals" );
        QVBoxLayout* main = new QVBoxLayout( dialog );
        QLabel* text = new QLabel("Seems, the file you've chosen contained some invalid\nyet maybe important data, well, it's fixed now :D" );
        QPushButton* ok = new QPushButton( "Ok" );
        main->addWidget( text );
        main->addWidget( ok, 0, Qt::AlignRight );
        connect( ok, &QPushButton::clicked, dialog, &QDialog::close );
        dialog->show();

        file.remove();

        break;
      }

      if (!this->set.contains( contact ))
      {
        mini = new ContactMini( contact );
        this->vbox->addWidget( mini );
        connect( mini, &ContactMini::delet, this, [=]() { deleteContact( this->vbox, mini ); } );
        connect( mini, &ContactMini::saved, this, [=]( bool state ) { PhoneBook::Altered( state, mini ); } );
        this->set += contact;
        this->list.append( mini );
      }
      contact.clear(); // it is needed to clear the `phones`
    }
    file.close();
    fix();
  }
}


void PhoneBook::Regroup( void )
{
  ContactMini* ptr;
  while (this->vbox->count())
  {
    ptr = qobject_cast<ContactMini*>(vbox->itemAt( 0 )->widget());
    if (ptr == Q_NULLPTR)
      this->vbox->takeAt( 0 );
    else
      ptr->setParent( Q_NULLPTR );
  }

  for (auto i : this->list)
    this->vbox->addWidget( i );
  this->vbox->addStretch( 1 );
}


void PhoneBook::Handler( void )
{
  Contact c;
  ContactForm* a = new ContactForm( c );
  a->show();
  connect( a, &ContactForm::saved, this, &PhoneBook::Add );
}


void PhoneBook::Add( Contact* c )
{
  if (!this->set.contains( *c ))
  {
    ContactMini* ptr = new ContactMini( *c );
    this->vbox->addWidget( ptr );
    this->set += *c;
    this->list.append( ptr );
    connect( ptr, &ContactMini::delet, this, [=]() { deleteContact( this->vbox, ptr ); } );
    connect( ptr, &ContactMini::saved, this, [=]( bool state ) { PhoneBook::Altered( state, ptr ); } );
    fix();
  }
}


void PhoneBook::clear( void )
{
  ContactMini* ptr;
  while (this->vbox->count())
  {
    ptr = qobject_cast<ContactMini*>(vbox->itemAt( 0 )->widget());
    if (ptr == Q_NULLPTR)
      this->vbox->takeAt( 0 );
    else
      ptr->setParent( Q_NULLPTR );
  }
  this->set.clear();
  this->list.clear();
}


void PhoneBook::Altered( bool state, ContactMini* mini )
{
  if (state)
  {
    this->set += mini->contact;
    this->list.append( mini );
    fix();
  }
  else
  {
    this->set -= mini->contact;
    this->list.removeOne( mini );
  }
}


void PhoneBook::hideFind( void )
{
  if (find->isChecked())
  {
    find->setText( u8"Find ▲" );
    city->show();
    age->show();
    domain->show();
    city_edit->show();
    age_edit_low->show();
    separator->show();
    age_edit_high->show();
    domain_edit->show();
  }
  else
  {
    find->setText( u8"Find ▼" );
    city->hide();
    age->hide();
    domain->hide();
    city_edit->hide();
    age_edit_low->hide();
    separator->hide();
    age_edit_high->hide();
    domain_edit->hide();
  }
}


void PhoneBook::Validation( QLineEdit* line, QIntValidator* valid, QString& str )
{
  int pos = 0;

  if (valid->validate( str, pos ) < 2 || QRegExp( "^0{2,}" ).exactMatch( str ))
    line->setText( str.left( str.size() - 1 ) );

  Sample(); // needed as the signal for this slot `QLineEdit::changed`
}


void PhoneBook::Check( void )
{
  if (this->age_edit_low->text().toInt() > this->age_edit_high->text().toInt() && !this->age_edit_high->text().isEmpty() && !this->age_edit_low->text().isEmpty())
  {
    QString str = this->age_edit_low->text();
    this->age_edit_low->setText( this->age_edit_high->text() );
    this->age_edit_high->setText( str );
  }
  // `Sample()` is not neede here because the texts of `QLineEdit`s are changed here, hence the signal is sent
}


void PhoneBook::Sample( void )
{
  for (auto i : this->list)
  {
    i->show();

    if (!this->search->text().isEmpty())
    {
      QStringList list = this->search->text().split( ' ', QString::SkipEmptyParts );
      bool name = true, phone = true;
      for (auto& str : list)
      {
        if (i->contact.cnameGetter().contains( str, Qt::CaseInsensitive ) ||
             i->contact.csurnameGetter().contains( str, Qt::CaseInsensitive ) ||
             i->contact.cmiddleGetter().contains( str, Qt::CaseInsensitive ))
          name = false;

        for (auto& j : i->contact.phonesGetter())
          if (j.contains( str, Qt::CaseInsensitive ))
          {
            phone = false;
            break;
          }
      }

      if (this->search->text().contains( QRegExp( R"([\d])" ) ) && phone)
        i->hide();
      else if (this->search->text().contains( QRegExp( R"([A-Za-z-])" ) ) && name)
        i->hide();
      else if (this->search->text().contains( QRegExp( R"((?:[\d].*?[A-Za-z-]|[A-Za-z-].*?[\d]))" ) ) && (name || phone))
        i->hide();

      name = true;
      phone = true;
    }

    if (!this->domain_edit->text().isEmpty() && !i->contact.cemailGetter().contains( this->domain_edit->text(), Qt::CaseInsensitive ))
      i->hide();

    if (!this->city_edit->text().isEmpty() && !i->contact.caddressGetter().contains( this->city_edit->text(), Qt::CaseInsensitive ))
      i->hide();

    int years = QDate::currentDate().year() - i->contact.cbirthGetter().year();
    if (!this->age_edit_low->text().isEmpty() && this->age_edit_low->text().toInt() > years)
      i->hide();

    if (!this->age_edit_high->text().isEmpty() && this->age_edit_high->text().toInt() < years)
      i->hide();
  }
}


void PhoneBook::clearFilters( void )
{
  this->search->setText( "" );
  this->age_edit_high->setText( "" );
  this->age_edit_low->setText( "" );
  this->city_edit->setText( "" );
  this->domain_edit->setText( "" );
}


void PhoneBook::fix( void )
{
  Sort( this->list, Contact::Field( this->internals.indexOf( this->mode->currentText() ) + 1 ) );
  Regroup();
  Sample();
}
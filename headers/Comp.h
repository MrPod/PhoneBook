#include "Contact.h"
#include "ContactMini.h"
#include "Exceptions.h"


class Comp
{
public:
  Contact::Field field;
  Comp( Contact::Field new_field ) : field( new_field ) { };
  bool operator() ( const Contact& p1, const Contact& p2 )
  {
    switch (field)
    {
      case Contact::Name:
        return p1.cnameGetter().toLower() < p2.cnameGetter().toLower();
        break;
      case Contact::Surname:
        return p1.csurnameGetter().toLower() < p2.csurnameGetter().toLower();
        break;
      case Contact::Middle:
        return p1.cmiddleGetter().toLower() < p2.cmiddleGetter().toLower();
        break;
      case Contact::Address:
        return p1.caddressGetter().toLower() < p2.caddressGetter().toLower();
        break;
      case Contact::Email:
        return p1.cemailGetter().toLower() < p2.cemailGetter().toLower();
        break;
      case Contact::Birth:
        return p1.cbirthGetter() < p2.cbirthGetter();
        break;
      case Contact::Phones:
        return p1.cphonesGetter() < p2.cphonesGetter();
        break;
      default:
        throw KeyExc();
    }
  };
};


class Comp_M
{
public:
  Contact::Field field;
  Comp_M( Contact::Field new_field ) : field( new_field ) { };
  bool operator() ( const ContactMini* c1, const ContactMini* c2 )
  {
    switch (field)
    {
      case Contact::Name:
        return c1->contact.cnameGetter().toLower() < c2->contact.cnameGetter().toLower();
        break;
      case Contact::Surname:
        return c1->contact.csurnameGetter().toLower() < c2->contact.csurnameGetter().toLower();
        break;
      case Contact::Middle:
        return c1->contact.cmiddleGetter().toLower() < c2->contact.cmiddleGetter().toLower();
        break;
      case Contact::Address:
        return c1->contact.caddressGetter().toLower() < c2->contact.caddressGetter().toLower();
        break;
      case Contact::Email:
        return c1->contact.cemailGetter().toLower() < c2->contact.cemailGetter().toLower();
        break;
      case Contact::Birth:
        return c1->contact.cbirthGetter() < c2->contact.cbirthGetter();
        break;
      case Contact::Phones:
        return c1->contact.cphonesGetter() < c2->contact.cphonesGetter();
        break;
      default:
        throw KeyExc();
    }
  };
};
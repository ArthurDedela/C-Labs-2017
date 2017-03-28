#include "Polynom.h"
#include <new>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/***CONSTRUCTORS***/
Polynom::Polynom(string str)
{
  string mon;
  int coef;
  unsigned power;
  int monom_end;
  

  while (str.length())
  {
    monom_end = str.find_first_of("+-", 1);

    if (monom_end == -1) {
      mon.assign(str);
      str.clear();
    }
    else {
      mon.assign(str, 0, monom_end);
      if (str[monom_end] == '+') monom_end++;
      str = str.substr(monom_end);
    }

    if (mon[0] == '-') coef = -1;
    else coef = 1;
    power = 0;
    
    if (isdigit(mon[0]) || mon[0] == '-') {
      stringstream sStream(mon);
      sStream >> coef;

      mon.erase(0, mon.find('x'));
    }

    if (isalpha(mon[0])) {
      int pow_start = mon.find("^");

      if (pow_start == -1) {
        power = 1;
      }
      else {
        mon.erase(0, pow_start + 1);
        stringstream sStream(mon);
        sStream >> power;
      }
    }

    Monom *current = new Monom(coef, power);
    add(current);

    delete current;
  }

  optimize();
}

Polynom::Polynom(const Polynom &p)
{
  if (p.list) {
    try {
      list = new Node(new Monom(p.list->get_monom()->get_coef(), p.list->get_monom()->get_pow()));
    }
    catch (const std::bad_alloc&) {
      alloc_error();
    }
  }
  Node *pTemp = p.list->get_next(), *thisTemp = list;

  while (pTemp) {
    try {
      thisTemp->set_next(new Node(new Monom(pTemp->get_monom()->get_coef(), pTemp->get_monom()->get_pow())));
    }
    catch (const std::bad_alloc&) {
      alloc_error();
    }
    pTemp = pTemp->get_next();
    thisTemp = thisTemp->get_next();
  }
}
/*****************/


/***PRIVATE METHODS***/
void Polynom::alloc_error()
{
  std::cout << "Memory allocation error!";
  exit(-1);
}

void Polynom::optimize()
{
  for (Node* temp_1 = list; temp_1 != nullptr; temp_1 = temp_1->get_next())
    for (Node* temp_2 = list; temp_2 != nullptr; temp_2 = temp_2->get_next())
    {
      if (temp_1 == temp_2) continue;

      if (temp_1->get_monom()->get_pow() == temp_2->get_monom()->get_pow()) {
        temp_1->get_monom()->set_coef(temp_1->get_monom()->get_coef() + temp_2->get_monom()->get_coef());

        temp_2 = delNode(temp_2);
      }
    }

  checkZeros();
}

void Polynom::checkZeros()
{
  for (Node* temp = list; temp != nullptr; temp = temp->get_next())
  {
    if (!temp->get_monom()->get_coef()) temp = delNode(temp);
  }
}


void Polynom::destroy()
{
  Node* temp = list;
  Monom* mon;
  while (temp) {
    list = list->get_next();
    mon = temp->get_monom();
    delete mon;
    delete temp;
    temp = list;
  }
  list = nullptr;
}

Node* Polynom::delNode(Node *ptr)
{
  Node* temp;
  Node* toDelete;
  if (ptr == list) {
    toDelete = list;
    list = list->get_next();
    temp = list;
  }
  else {
    for (temp = list; temp->get_next() != ptr; temp = temp->get_next());

    toDelete = temp->get_next();
    temp->set_next(temp->get_next()->get_next());
  }

  delete toDelete->get_monom();
  delete toDelete;

  return temp;
}

void Polynom::divide(const Polynom& divider, Polynom& result, Polynom& remainder)
{
  remainder = *this;
  unsigned pow_1 = remainder.list->get_monom()->get_pow(), pow_2 = divider.list->get_monom()->get_pow();
  int coef_1 = remainder.list->get_monom()->get_coef(), coef_2 = divider.list->get_monom()->get_coef();
  
  while((pow_1 >= pow_2) && (abs(coef_1) >= abs(coef_2)))
  {
    Monom *tempRes = new Monom(coef_1 / coef_2, pow_1 - pow_2);
    result.add(tempRes);
    remainder -= const_cast<Polynom&>(divider) * *tempRes;

    pow_1 = remainder.list->get_monom()->get_pow();
    coef_1 = remainder.list->get_monom()->get_coef();  
  }
}
/*********************/


void Polynom::add(Monom *pMonom)
{
  unsigned pos = seekPos(pMonom->get_pow());

  try {
    if (!pos) {
      list = new Node(pMonom, list);
      unsigned kek = seekPos(pMonom->get_pow() - 1);
    }
      
    else {
      Node *temp = list;
      while (--pos && temp->get_next()) 
        temp = temp->get_next();

      if (temp->get_next()) 
        temp->set_next(new Node(pMonom, temp->get_next()));
      else 
        temp->set_next(new Node(pMonom));
    }
  }
  catch (const std::bad_alloc&) {
    alloc_error();
  }
}

unsigned Polynom::seekPos(unsigned power)
{
  unsigned i = 0;
  if (list) {
    for (Node* temp = list; temp && (power < temp->get_monom()->get_pow()); temp = temp->get_next()) 
      i++;
  }

  return i;
}


/***OPERATORS***/
Polynom & Polynom::operator=(const Polynom &pol)
{
  destroy();
  if (pol.list) {
    try {
      list = new Node(new Monom(pol.list->get_monom()->get_coef(), pol.list->get_monom()->get_pow()));
    }
    catch (const std::bad_alloc&) {
      alloc_error();
    }
  }
  Node *polTemp = pol.list->get_next(), *thisTemp = list;

  while (polTemp) {
    try {
      thisTemp->set_next(new Node(new Monom(polTemp->get_monom()->get_coef(), polTemp->get_monom()->get_pow())));
    }
    catch (const std::bad_alloc&) {
      alloc_error();
    }
    polTemp = polTemp->get_next();
    thisTemp = thisTemp->get_next();
  }

  return *this;
}


Polynom & Polynom::operator*=(const Polynom &pol)
{
  Node *thisIterator, *polIterator;
  Polynom tempPolynom;
  
  for (thisIterator = list; thisIterator != nullptr; thisIterator = thisIterator->get_next())
    for (polIterator = pol.list; polIterator != nullptr; polIterator = polIterator->get_next())
    {
      tempPolynom.add(&(*thisIterator->get_monom() * *polIterator->get_monom()));
    }

  tempPolynom.optimize();
  *this = tempPolynom;

  return *this;
}

Polynom Polynom::operator*(const Polynom &pol)
{
  Polynom temp = *this;
  temp *= pol;
  return temp;
}


Polynom & Polynom::operator*=(Monom mon)
{
  Polynom temp(&mon);
  
  *this *= temp;
  
  return *this;
}

Polynom Polynom::operator*(Monom mon)
{
  Polynom temp = *this;
  temp *= mon;
  return temp;
}


Polynom & Polynom::operator*=(int n)
{
  Monom mon(n, 0);

  *this *= mon;

  return *this;
}

Polynom Polynom::operator*(int n)
{
  Polynom temp = *this;
  temp *= n;
  return temp;
}

Polynom Polynom::operator-()
{
  Polynom temp = *this * -1;
  return temp;
}


Polynom & Polynom::operator+=(const Polynom &pol)
{
  Node *polIterator;

  for (polIterator = pol.list; polIterator != nullptr; polIterator = polIterator->get_next())
  {
    add(polIterator->get_monom());
  }
  optimize();

  return *this;
}

Polynom Polynom::operator+(const Polynom &pol)
{
  Polynom temp = *this;

  temp += pol;

  return temp;
}


Polynom & Polynom::operator-=(const Polynom &pol)
{
  *this += const_cast<Polynom&>(pol) * -1;
  return *this;
}

Polynom Polynom::operator-(const Polynom &pol)
{
  Polynom temp = *this;
  temp -= pol;
  return temp;
}


Polynom & Polynom::operator/=(const Polynom &pol)
{
  Polynom result, remainder;
  divide(pol, result, remainder);
  
  *this = result;
  return *this;
}

Polynom Polynom::operator/(const Polynom &pol)
{
  Polynom temp = *this;
  temp /= pol;
  return temp;
}


Polynom & Polynom::operator%=(const Polynom &pol)
{
  Polynom result, remainder;
  divide(pol, result, remainder);

  *this = remainder;
  return *this;
}

Polynom Polynom::operator%(const Polynom & pol)
{
  Polynom temp = *this;
  temp %= pol;
  return temp;
}

/***************/


void Polynom::print()
{
  if (list) {
    for (Node* temp = list; temp != nullptr; temp = temp->get_next())
    {
      if (temp != list) cout.setf(ios::showpos);
      int coef = temp->get_monom()->get_coef();
      unsigned pow = temp->get_monom()->get_pow();

      if ((coef == 1) && pow) cout << "+";
      else if ((coef == -1) && pow) cout << "-";
      else cout << coef;

      if (!pow) continue;
      else if (pow) cout << "x";

      if (pow > 1) cout << "^" << pow;
    }
    cout << endl;
  }
  cout.unsetf(ios::showpos);
}

ostream & operator<<(ostream & os, Polynom & p)
{
  p.print();
  return os;
}


/***DESTRUCTOR***/
Polynom::~Polynom()
{
  destroy();
}

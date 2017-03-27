#pragma once
#include "Node.h"
#include <string>

class Polynom
{
  Node *list;

  void alloc_error();
  void optimize();
  void checkZeros();
  void destroy();

  /*Returns pointer, previous to deleted noed, if head removed returns new head*/
  Node* delNode(Node*);
  void divide(const Polynom& Divider, Polynom& Quotient, Polynom& Remainder);
public:
  Polynom() { list = nullptr; };
  Polynom(std::string sPolynom);
  Polynom(Monom *pMonom) { list = new Node(pMonom); };
  Polynom(const Polynom&);
  
  void add(Monom * pMonom);
  unsigned seekPos(unsigned power);

  Polynom& operator=(const Polynom&);

  Polynom& operator*=(const Polynom&);
  Polynom operator*(const Polynom&);

  Polynom& operator*=(Monom);
  Polynom operator*(Monom);

  Polynom& operator+=(const Polynom&);
  Polynom operator+(const Polynom&);

  Polynom& operator-=(const Polynom&);
  Polynom operator-(const Polynom&);

  Polynom& operator/=(const Polynom & pol);
  Polynom operator/(const Polynom &pol);

  Polynom& operator%=(const Polynom &pol);
  Polynom operator%(const Polynom &pol);

  Polynom& operator*=(int);
  Polynom operator*(int n);

  Polynom operator-();

  void print();

  
  ~Polynom();
};

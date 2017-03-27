#include "Monom.h"

Monom::Monom(int k, unsigned power)
{
  coef = k;
  pow = power;
}

Monom Monom::operator*(const Monom &mon)
{
  Monom temp;
  temp.set_coef(coef * mon.coef);
  temp.set_pow(pow + mon.pow);
  return temp;
}


Monom::~Monom()
{
}

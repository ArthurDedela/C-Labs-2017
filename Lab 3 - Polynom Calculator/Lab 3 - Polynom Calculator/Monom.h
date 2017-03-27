#pragma once
class Monom
{
  int coef;
  unsigned pow;

public:
  Monom(int coef = 0, unsigned power = 0);

  int get_coef() { return coef; };
  unsigned get_pow() { return pow; };
  void set_coef(int k) { coef = k; };
  void set_pow(unsigned power) { pow = power; };

  Monom operator*(const Monom&);

  ~Monom();
};


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Polynom.h"

using namespace std;


int main()
{
  Polynom pol("2x+2x+2x+2"), pol2 = pol;
  Polynom pol3("8x-1"), test1("8x^3+4x^2-3x+5"), test2("1x^1-2x^0");
  pol2.print();
  pol3.print();

  Monom mon(-1, 0);

  test1.print();
  test2.print();

  (test1 %= test2).print();
  test1.print();

  system("pause");
  return 0;
}
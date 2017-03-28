#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Polynom.h"

using namespace std;


int main()
{
  Polynom pol("2x+2x+2x+2"), pol2 = pol;
  Polynom pol3("8x-1"), test1("8x^3+4x^2-3x+5");
  Polynom test2("x^2-2x+1");
  cout << pol2 << pol3;

  Monom mon(-1, 0);

  cout << test1 << test2;

  test1 /= test2;
  cout << test1;

  system("pause");
  return 0;
}
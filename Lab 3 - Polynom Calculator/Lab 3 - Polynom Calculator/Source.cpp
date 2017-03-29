#include <iostream>
#include <fstream>
#include <string>
#include "Polynom.h"

using namespace std;


Polynom polStack[5];
char operStack[5];
string operators = "+-*/%";

void interpret(const string& fileName);
int priority(char oper);
void calculate(int& polCnt, char oper);
void fatalError(string message);
void saveResult(string iName);


int main()
{
  //Polynom kldsf("0");
  //cout << kldsf;

  //Polynom pol("2x+2x+2x+2"), pol2 = pol;
  //Polynom pol3("8x-1");
  Polynom test1("8x^3+4x^2-3x+5"), test2("2x^2-2x+1");
  //cout << pol2 << pol3;


  string fileName("input.txt");

  //Monom mon(-1, 0);

  //cout << test1 << test2;

  test1 %= test2;
  cout << test1 << endl;

  interpret(fileName);

  system("pause");
  return 0;
}

void interpret(const string& fileName)
{
  ifstream fin(fileName);
  if (!fin.is_open()) {
    cout << "Error opening file " << fileName;
    return;
  }

#define is_operator(c) ((operators.find_first_of(c) != -1) ? (true) : (false))
  char c;
  string s;
  int polCnt = -1;
  char *stC = operStack;
  *stC = '(';

  while (!fin.eof())
  {
    fin >> c;

    if (c == '(') *++stC = c;

    else if (c == '<') 
    {
      s.clear();

      while ((c = fin.get()) != '>')
      {
        if (!isspace(c)) s += c;
      }

      Polynom newPolynom(s);
      polStack[++polCnt] = newPolynom;
    }

    else if (is_operator(c)) 
    {
      while (is_operator(*stC) && (priority(*stC) >= priority(c)))
      {
        calculate(polCnt, *stC--);
      }
      *++stC = c;
    }

    else if (c == ';')
    {
      while (*stC != '(') calculate(polCnt, *stC--);
      saveResult(fileName);
      polCnt = -1;
    }

    else if (c == '#') getline(fin, s);

  }
}

int priority(char oper)
{
  if (operators.find_first_of(oper) > 1) return 2;
  else return 1;
}

void calculate(int& polCnt, char oper)
{
  Polynom arg2 = polStack[polCnt--];
  Polynom arg1 = polStack[polCnt];
  switch (oper)
  {
  case '+':
    polStack[polCnt] = arg1 + arg2;
    break;

  case '-':
    polStack[polCnt] = arg1 - arg2;
    break;

  case '*':
    polStack[polCnt] = arg1 * arg2;
    break;

  case '/':
    if (arg2.is_zero()) fatalError("Division by zero!");
    polStack[polCnt] = arg1 / arg2;
    break;

  case '%':
    if (arg2.is_zero()) fatalError("Division by zero!");
    polStack[polCnt] = arg1 % arg2;
    break;
  }
}

void saveResult(string iName)
{
  static ofstream fout(iName + "_answers.txt");
  
  fout << polStack[0] << ";" << endl;
}

void fatalError(string message)
{
  cout << "Fatal error!" << endl << message << endl;
  system("pause");
  exit(-1);
}
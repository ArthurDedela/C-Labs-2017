#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <clocale>
#include <conio.h>
#include <fstream>
#include <math.h>
#include <time.h>

using namespace std;

#define arrSize(x) sizeof(x) / sizeof(x[0])
#define A coefs.a
#define B coefs.b
#define C coefs.c


struct Files {
  string input = "input.txt";
  string output = "output.txt";
  string log = "log.txt";
} files;

string helpFiles[] = {
  "Help/navigation.txt",
  "Help/about.txt",
  "Help/inputSyntax.txt",
  "Help/taskSyntax.txt",
  "Help/inputExamples.txt"
};

string menuItems[] = {
  "Информация о студенте",
  "Помощь",
  "Запуск программы",
  "Настройки файлов",
  "Выход (ESC)"
};

string fileMenu[] = {
  "Изменить файл ввода",
  "Изменить файл вывода",
  "Назад (ESC)"
};

typedef struct Coefficients {
  double a, b, c;
} lineCoefs;

typedef struct LineInfo {
  int lineType;
  string equation;
  lineCoefs coefs;
} lineinfo;

typedef struct Point {
  double x, y;
} point;

void drawTable(int itemsCnt, int selectedItem, int menuWidth, string items[] = menuItems);
void handleArrow(int arrow, int &selectedItem, int itemsCnt);
void showStudent();
void showHelp();
void showCurFiles();
string readFile(ifstream &f);
void changeFile(int selectedItem);
void eraseSpaces(string &str);
void log(string, int closeLog = 0);

/*Returns parsed vector dimension, or -1 if vector not found in str.*/
int getCoordinates(string str, int &lastPos, int a[3]);
void writeOut(string message, string res = "null", int endFlag = 0);

void interpret();
//vectors
void scalarProd(string&);
void vectorProd(string&);
void mixedProd(string&);
void vectorArithmetics(string&);
void vectorModule(string&);

//lines task 2.1
void makeAllLines(lineCoefs);
string makeCommon(lineCoefs);
string makeCanonical(lineCoefs);
string makeSlope(lineCoefs);
string makeSegment(lineCoefs);
string makeTwoPoints(lineCoefs);
string makeParametric(lineCoefs);
void createLines(string&);
void fromCommon(string&, lineCoefs&);
void fromSlope(string&, lineCoefs&);
void fromSegments(string&, lineCoefs&);
void fromCanonical(string&, lineCoefs&);
void fromTwoPoints(string&, lineCoefs&);
void fromParametric(string&, lineCoefs&);
void(*lines[7])(string&, lineCoefs&) = { fromCommon, fromCanonical, fromSlope, fromSegments, fromTwoPoints, fromParametric };
/*******/

void findCrosspoint(string&);
void cramerMethod(lineCoefs coefs, lineCoefs coefs_2, point &crosspoint);
void getLineInfo(string &str, lineinfo &line);

void distanceToPoint(string&);

void(*algebra[2][5])(string&) = { 
  { scalarProd, vectorProd, mixedProd, vectorArithmetics, vectorModule }, 
  { createLines, findCrosspoint, distanceToPoint } 
};


void (*menuFunctions[])() = { showStudent, showHelp, interpret };

int main() {
  log("Стартуем!");
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); 

  char c, cf;
  int selectedItem = 0, selectedFile = 0;
  const int menuSize = arrSize(menuItems), fileMenuSize = arrSize(fileMenu);

  /***MAIN MENU***/
  log("Пользователь вошел в меню.");
  do {
    system("cls");
    drawTable(menuSize, selectedItem, 40);
    c = _getch();
    if (c == -32) {
      handleArrow(_getch(), selectedItem, menuSize);
    }
    else if (c == 13) {
      if (selectedItem == 3) {
        log("Пользователь вошел в меню изменения файлов.");
        /***FILE MENU***/
        do {
          system("cls");
          drawTable(fileMenuSize, selectedFile, 40, fileMenu);
          showCurFiles();
          cf = _getch();
          if (cf == -32) {
            handleArrow(_getch(), selectedFile, fileMenuSize);
          }
          else if (cf == 13) {
            if (selectedFile == fileMenuSize - 1) cf = 27;
            else changeFile(selectedFile);
          }
        } while (cf != 27);
        log("Пользователь покинул меню изменения файлов.");
      }/***END FILE MENU***/
      else if (selectedItem == menuSize - 1) c = 27;
      else menuFunctions[selectedItem]();
    }
  } while (c != 27);
  log("Пользователь вышел из меню.");

  log("Программа завершилась.", 1);
  return 0;
}

/***SHELL FUNCTIONS***/
void drawTable(int itemsCnt, int selectedItem, int menuWidth, string items[]) {
  string borderTop(menuWidth, 205), curItem;
  string borderBottom = borderTop;

  borderTop.insert(borderTop.begin(), 201);
  borderTop.push_back(187);

  borderBottom.insert(borderBottom.begin(), 200);
  borderBottom.push_back(188);

  setlocale(LC_ALL, "C");
  cout << borderTop << '\n';
  for (int i = 0; i < itemsCnt; i++)
  {
    cout << char(186);
    setlocale(LC_ALL, "russian");
    curItem.assign(items[i]);
    curItem.resize(menuWidth);
    if (i == selectedItem) {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 0));
      cout << curItem;
      SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
    }
    else cout << curItem;
    setlocale(LC_ALL, "C");
    cout << char(186) << '\n';
  }
  cout << borderBottom << '\n';
  setlocale(LC_ALL, "russian");
}

void handleArrow(int arrow, int &selectedItem, int itemsCnt) {
  switch (arrow)
  {
  case 72: //↑
    if (selectedItem == 0) selectedItem = itemsCnt - 1;
    else selectedItem--;
    break;
  case 80: //↓
    if (selectedItem == (itemsCnt - 1)) selectedItem = 0;
    else selectedItem++;
    break;
  }
}

void showCurFiles()
{
  cout << "Текущие файлы:\n" << "Ввод - " << files.input << '\n';
  cout << "Вывод - " << files.output << '\n';
  cout << "Лог - " << files.log << '\n';
}

void showStudent() {
  cout << "Студент: Дедела Артур" << '\n' << "Группа: Т30-102Б-16" << '\n';
  system("pause");
}

void showHelp()
{
  log("Пользователь зашел в помощь.");
  string helpPages[arrSize(helpFiles)];
  char c;
  ifstream fIn;
  for (int i = 0; i < arrSize(helpFiles); i++) 
  {
    fIn.open(helpFiles[i]);
    if (fIn.is_open()) helpPages[i] = readFile(fIn);
    else cout << "Error downloading '" << helpFiles[i] << "' help page";
  }
  int page = 0;
  do {
    system("cls");
    cout << helpPages[page];
    if ((c = _getch()) == '+') {
      page++;
      if (page >= arrSize(helpPages)) page = 0;
    }
  } while (c != 27);
  log("Пользователь вышел из помощи.");
}

string readFile(ifstream &f)
{
  string buf, text;
  if (f.is_open()) {
    while (!f.eof()) {
      getline(f, buf);
      buf += '\n';
      text += buf;
    }
    f.close();
    return text;
  }
}

void changeFile(int file)
{
  string fileName;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  cout << "Введите новое имя файла: ";
  cin >> fileName;
  SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
  switch (file)
  {
  case 0:
    log("Пользователь изменил входной файл: " + files.input + " на файл: " + fileName);
    files.input = fileName;
    break;
  case 1:
    log("Пользователь изменил выходной файл: " + files.output + " на файл: " + fileName);
    files.output = fileName;
    break;
  }
}
/*********************/

/***MAIN INTERPRETING FUNCTION***/
void interpret() {
  log("Запускается функция интерпретации.");
  ofstream fout(files.output);
  fout.close();
  ifstream fIn(files.input);
  string buf;
  int topic, task;
  if (fIn.is_open()) {
    log("Начата интерпретация входного файла " + files.input);
    while (!fIn.eof()) {
      getline(fIn, buf);
      if ((buf.length() < 5) || (buf.front() != '№')) continue;
      topic = buf[1] - '0' - 1;
      task = buf[3] - '0' - 1;
      eraseSpaces(buf.erase(0, 4));
      algebra[topic][task](buf);
    }
    fIn.close();
    writeOut(" ", " ", 1);
    cout << "Все задания выполнены." << endl;
    log("Файл ввода был успешно прочитан и выполнен.");
  }
  else {
    cout << "Файл с заданиями не найден!";
    log("Не найден входной файл " + files.input);
  }

  system("pause");
}


/***VECTOR FUNCTIONS***/
void scalarProd(string &str)
{
  int vectors[2][3], lastPos = 0, dimension, res;
  for (int i = 0; i < 2; i++) {
    dimension = getCoordinates(str, lastPos, vectors[i]);
  }
  if (dimension == 2) {
    res = vectors[0][0] * vectors[1][0] + vectors[0][1] * vectors[1][1];
  }
  else if (dimension == 3) {
    res = vectors[0][0] * vectors[1][0] + vectors[0][1] * vectors[1][1] + vectors[0][2] * vectors[1][2];
  }
  else {
    cout << "№1.1 Координаты векторов заданы неверно." << endl;
    return;
  }

  str.insert(str.find("}") + 1, " ", 1);
  string message("№1.1 Скалярное произведение векторов. " + str);

  writeOut(message, to_string(res));
}

void vectorProd(string &str)
{
  int vectors[2][3], res[3], lastPos = 0;
  for (int i = 0; i < 2; i++) {
    if (getCoordinates(str, lastPos, vectors[i]) != 3) {
      cout << "№1.2 Размерности векторов не совпадают, либо меньше 3." << endl;
      return;
    }
  }
  res[0] = vectors[0][1] * vectors[1][2] - vectors[0][2] * vectors[1][1];
  res[1] = vectors[0][2] * vectors[1][0] - vectors[0][0] * vectors[1][2];
  res[2] = vectors[0][0] * vectors[1][1] - vectors[0][1] * vectors[1][0];
  str.insert(str.find("}") + 1, " ", 1);
  string message("№1.2 Векторное произведение векторов. " + str), answer;
  answer = "{" + to_string(res[0]) + ", " + to_string(res[1]) + ", " + to_string(res[2]) + "}";
  writeOut(message, answer);
}

void mixedProd(string &str)
{
  int vectors[3][3], res, lastPos = 0;
  for (int i = 0; i < 3; i++) {
    if (getCoordinates(str, lastPos, vectors[i]) != 3) cout << "№1.3 Размерности векторов не совпадают, либо меньше 3." << endl;
  }
  res = vectors[0][0] * vectors[1][1] * vectors[2][2] + vectors[0][1] * vectors[1][2] * vectors[2][0] + vectors[0][2] * vectors[1][0] * vectors[2][1] - vectors[0][2] * vectors[1][1] * vectors[2][0] - vectors[0][0] * vectors[1][2] * vectors[2][1] - vectors[0][1] * vectors[1][0] * vectors[2][2];

  str.insert(str.find("}") + 1, " ", 1);
  str.insert(str.find("}", str.find("}") + 1) + 1, " ", 1);
  string message("№1.3 Смешанное произведение векторов. " + str);
  writeOut(message, to_string(res));
}

void vectorArithmetics(string &str) 
{
  int signPos = str.find_first_of("+-*");
  int vectors[2][3], lastPos = 0, dimension, prev = 0, res[3], i;
  for (i = 0; i < 2; i++) {
    dimension = getCoordinates(str, lastPos, vectors[i]);
    if ((i > 0) && (dimension != -1) && (prev != dimension)) {
      cout << "№1.4 Ошибка! Разные размерности векторов.";
      return;
    }
    if (dimension == -1) dimension = prev;
    prev = dimension;
  }
  string action;
  switch (str[signPos])
  {
  case '+':
    for (i = 0; i < dimension; i++) res[i] = vectors[0][i] + vectors[1][i];
    break;
  case '-':
    for (i = 0; i < dimension; i++) res[i] = vectors[0][i] - vectors[1][i];
    break;
  case '*':
    string koef(str, signPos + 1);
    int k = atoi(koef.c_str());
    for (i = 0; i < dimension; i++) {
      res[i] = vectors[0][i] * k;
    }
    break;
  }
  string answer("{");
  for (i = 0; i < dimension; i++) {
    answer += to_string(res[i]);
    if (dimension - i > 1) answer += ", ";
  }
  answer += "}";
  str.insert(signPos + 1, " ", 1);
  str.insert(str.find("}") + 1, " ", 1);
  string message("№1.4 Cтандартные арифметические операции над векторами. " + str);
  writeOut(message, answer);
}

void vectorModule(string &str)
{
  int temp = 0, dimension, vector[3], lastPos = 0;
  dimension = getCoordinates(str, lastPos, vector);
  for (int i = 0; i < dimension; i++) {
    temp += vector[i] * vector[i];
  }
  double res = sqrt(temp);
  string message("№1.5 Нахождение модуля вектора вектора. " + str);
  writeOut(message, to_string(res));
}

/*****************/


/***TASK 2.1***/
void createLines(string &str)
{
  int lineType = str[1] - '0' - 1;
  lineCoefs coefs;
  str.erase(0, 3);
  writeOut("№2.1 Привести уравнение прямой к другим видам. Дано: " + str);
  lines[lineType](str, coefs);
  makeAllLines(coefs);
}

void fromCommon(string &str, lineCoefs &coefs) {
  stringstream common(str);
  common >> A;
  common.ignore(1); 
  common >> B;
  common.ignore(1); 
  common >> C;
}

void fromCanonical(string &str, lineCoefs &coefs) {
  stringstream line(str);
  double x1, y1;
  line.ignore(3);
  line >> x1;
  line.ignore(2);
  line >> B;
  line.ignore(4);
  line >> y1;
  line.ignore(2);
  line >> A;
  C = -(A*x1) + B*y1;
  B *= -1;
}

void fromSlope(string &str, lineCoefs &coefs) {
  stringstream slope(str);
  B = -1;
  slope.ignore(2);
  slope >> A;
  slope.ignore(2); 
  slope >> C;
}

void fromSegments(string &str, lineCoefs &coefs) {
  stringstream line(str);
  C = 1;
  line.ignore(3);
  line >> A;
  line.ignore(5);
  line >> B;
  A = 1 / A;
  B = 1 / B;
}

void fromTwoPoints(string &str, lineCoefs &coefs) {
  double x1, x2, y1, y2;
  stringstream canonical;
  int from = str.find('(', 3), to = str.find(')', from);
  string buf = str.substr(from + 1, to - from - 1);
  stringstream line(buf);
  line >> x2;
  line.ignore(1);
  line >> x1;
  canonical << str.substr(0, str.find("(", 3)) << x2 - x1;
  str.erase(0, str.find("="));
  canonical << str.substr(0, str.rfind("("));
  str.erase(0, str.rfind("("));
  
  from = str.rfind('('), to = str.rfind(')');
  buf = str.substr(from + 1, to - from - 1);
  stringstream line1(buf);
  line1 >> y2;
  line1.ignore(1);
  line1 >> y1;
  canonical << y2 - y1;
  fromCanonical(canonical.str(), coefs);
}

void fromParametric(string &str, lineCoefs &coefs){
  double x1, y1;
  stringstream canonical, line(str);
  line.ignore(2);
  line >> x1;
  line.ignore(1);
  line >> A;
  line.ignore(3);
  line >> y1;
  line.ignore(1);
  line >> B;
  canonical.setf(ios::showpos);
  canonical << "(x" << x1 << ")/" << A << "=(y" << y1 << ")/" << B;
  fromCanonical(canonical.str(), coefs);
}

void makeAllLines(lineCoefs coefs)
{
  writeOut("1)Общий вид.", makeCommon(coefs));
  writeOut("2)Канонический вид.", makeCanonical(coefs));
  writeOut("3)Уравнение прямой с угловым коэффициентом.", makeSlope(coefs));
  writeOut("4)Уравнение прямой в отрезках.", makeSegment(coefs));
  writeOut("5)Уравнение прямой через 2 точки.", makeTwoPoints(coefs));
  writeOut("6)Параметрическое уравнение прямой.", makeParametric(coefs));
}

string makeCommon(lineCoefs coefs)
{
  stringstream common;
  common.setf(ios::showpos);
  common << A << "x" << B << "y" << C << "=0";
  return common.str();
}

string makeCanonical(lineCoefs coefs)
{
  stringstream equation;
  equation.setf(ios::showpos);
  equation << "x/" << -B << "=(y" << -(-C / B) << ")/" << A;
  return equation.str();
}

string makeSlope(lineCoefs coefs)
{
  stringstream equation;
  equation.setf(ios::showpos);
  equation << "y=" << A / (-B) << "x" << C / (-B);
  return equation.str();
}

string makeSegment(lineCoefs coefs)
{
  stringstream equation;
  equation.setf(ios::showpos);
  equation << "x/" << -(C / A) << " + y/" << -(C / B) << " = 1";
  return equation.str();
}

string makeTwoPoints(lineCoefs coefs)
{
  double x1 = 2, x2 = 5, y1, y2;
  y1 = (-(A*x1) - C) / B;
  y2 = (-(A*x2) - C) / B;
  stringstream equation;
  equation.setf(ios::showpos);
  equation << "(x" << x2 << ")/(" << x2 << x1 << ") = (y" << y2 << ")/(" << y2 << y1 << ")"; 
  return equation.str();
}

string makeParametric(lineCoefs coefs)
{
  double x1 = 3, y1;
  y1 = (-(A*x1) - C) / B;
  stringstream equation;
  equation.setf(ios::showpos);
  equation << "x = " << x1 << -B << "t" << endl;
  equation << "y = " << y1 << A << "t";
  return equation.str();
}
/*************/


/***CROSSPOINT***/
void findCrosspoint(string &str)
{
  lineinfo line_1, line_2;
  point crosspoint;
  getLineInfo(str, line_1);
  getLineInfo(str, line_2);
  cramerMethod(line_1.coefs, line_2.coefs, crosspoint);

  stringstream answer;
  if (isnan(crosspoint.x) || isnan(crosspoint.y)) answer << "Прямые сливаются.";
  else if (isinf(crosspoint.x) || isinf(crosspoint.y)) answer << "Прямые параллельны. Общих точек нет.";
  else answer << "(" << crosspoint.x << ", " << crosspoint.y << ")";
  string message("№2.2 Найти точку пересечения прямых на плоскости. ");
  message += line_1.equation + " | " + line_2.equation;
  writeOut(message, answer.str());
}

void cramerMethod(lineCoefs coefs, lineCoefs coefs_2, point &crosspoint)
{
#define A2 coefs_2.a
#define B2 coefs_2.b
#define C2 coefs_2.c
  C *= -1;
  C2 *= -1;
  double det = A * B2 - B * A2;
  double det_1 = C * B2 - B * C2;
  double det_2 = A * C2 - C * A2;
  crosspoint.x = det_1 / det;
  crosspoint.y = det_2 / det;
}
/****************/


void distanceToPoint(string &str)
{
  lineinfo line;
  int point[2], lastPos = 0;
  getLineInfo(str, line);
  getCoordinates(str, lastPos, point);
  double distance = abs(line.A * point[0] + line.B * point[1] + line.C) / sqrt(line.A*line.A + line.B*line.B);
  string message = "№2.3 Найти расстояние от точки до прямой. Прямая: " + line.equation + " Точка: " + str;
  writeOut(message, to_string(distance));
}


/***SERVICE FUNCTIONS***/
void writeOut(string message, string res, int endFlag) 
{
  ofstream fOut(files.output, ios::app);
  if (endFlag) fOut.close();
  else if (fOut.is_open())
  {
    fOut << message << endl;
    if (res != "null") fOut << "Ответ: " << res << '\n' << endl;
  }
}

void eraseSpaces(string &str)
{
  int i = 0;
  while(i < str.length())
  {
    if (isspace(str[i])) str.erase(i, 1);
    else i++;
  }
}

int getCoordinates(string str, int &lastPos, int a[3])
{
  char c = ' ';
  int dimension, from = str.find("{", lastPos), to;
  if (from == string::npos) return -1;
  to = str.find("}", lastPos);
  string coordinates;
  coordinates.assign(str, from + 1, to - from);
  lastPos = to + 1;
  stringstream coordStream(coordinates);
  for (dimension = 0; c != '}'; dimension++)
  {
    coordStream >> a[dimension] >> c;
  }
  return dimension;
}

void getLineInfo(string &str, lineinfo &line)
{
  int del;
  line.lineType = str[1] - '0' - 1;
  str.erase(0, 3);
  line.equation = str.substr(0, del = str.find(";"));
  str.erase(0, del + 1);
  lines[line.lineType](line.equation, line.coefs);
}

void log(string message, int closeLog)
{
  static ofstream fOut(files.log);
  time_t t = time(0);
  char sTime[30];
  strftime(sTime, sizeof(sTime), "%H:%M:%S", localtime(&t));
  fOut << sTime << " — " << message << endl;
  if (closeLog) fOut.close();
}
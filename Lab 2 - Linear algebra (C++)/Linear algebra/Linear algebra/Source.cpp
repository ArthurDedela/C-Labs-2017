#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <clocale>
#include <conio.h>
#include <fstream>

using namespace std;

#define aSize(x) sizeof(x) / sizeof(x[0])

struct Files {
  char input[30] = "input.txt";
  char output[30] = "output.txt";
  char errors[30] = "errors.txt";
  char log[30] = "log.txt";
} files;

char *helpFiles[] = {
  "help/navigation.txt",
  "help/about.txt",
  "help/inputSyntax.txt",
};

string menuItems[] = {
  "Информация о студенте",
  "Помощь",
  "Настройки файлов",
  "Запуск программы",
  "Выход (ESC)"
};

string fileMenu[] = {
  "Изменить файл ввода",
  "Изменить файл вывода",
  "Изменить файл ошибок",
  "Изменить лог файл",
  "Назад (ESC)"
};

void drawTable(int itemsCnt, int selectedItem, int menuWidth, string items[] = menuItems);
void handleArrow(int arrow, int &selectedItem, int itemsCnt);
void showStudent();
void showHelp();
void showCurFiles();
FILE* openFile(char *fileName, char *mode);
void readFile(FILE *f, char *buf);
void changeFile(int selectedItem);

void (*menuFunctions[])() = { showStudent, showHelp };

int main() {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); 

  char c, cf;
  int selectedItem = 0, selectedFile = 0;
  const int menuSize = aSize(menuItems), fileMenuSize = aSize(fileMenu);

  /***MAIN MENU***/
  do {
    system("cls");
    drawTable(menuSize, selectedItem, 40);
    c = _getch();
    if (c == -32) {
      handleArrow(_getch(), selectedItem, menuSize);
    }
    else if (c == 13) {
      if (selectedItem == 2) {
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
      }/***END FILE MENU***/
      else if (selectedItem == menuSize - 1) c = 27;
      else menuFunctions[selectedItem]();
    }
  } while (c != 27);

  return 0;
}

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
  cout << "Ошибки - " << files.errors << '\n';
  cout << "Лог - " << files.log << '\n';
}

void showStudent() {
  cout << "Студент: Дедела Артур" << '\n' << "Группа: Т30-102Б-16" << '\n';
  system("pause");
}

void showHelp()
{
  char helpPages[aSize(helpFiles)][1000], c;
  for (int i = 0; i < aSize(helpFiles); i++) 
  {
    readFile(openFile(helpFiles[i], "r"), helpPages[i]);
  }
  int page = 0;
  do {
    system("cls");
    cout << helpPages[page];
    if ((c = _getch()) == '+') {
      page++;
      if (page >= aSize(helpPages)) page = 0;
    }
  } while (c != 27);
}

FILE* openFile(char *fileName, char *mode) {
  FILE *fTemp;
  if (!(fTemp = fopen(fileName, mode))) {
    printf("Error #404!\nFile '%s' not found!\n", fileName);
    return NULL;
  }
  return fTemp;
}

void readFile(FILE *f, char *buf)
{
  wchar_t c;
  char *ptr = buf;
  while (c = fgetc(f), !feof(f)) {
    *ptr++ = c;
  }
  *ptr = 0;
  fclose(f);
}

void changeFile(int file)
{
  char fileName[30];
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  cout << "Введите новое имя файла: ";
  cin >> fileName;
  SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
  switch (file)
  {
  case 0: 
    strcpy(files.input, fileName);
    break;
  case 1:
    strcpy(files.output, fileName);
    break;
  case 2:
    strcpy(files.errors, fileName);
    break;
  case 3:
    strcpy(files.log, fileName);
    break;
  }
}
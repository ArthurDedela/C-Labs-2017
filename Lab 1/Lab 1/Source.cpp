#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#define _STUDENT_INFO_FLAG 0x1
#define _EMAIL_INFO_FLAG 0x2
#define _HELP_FLAG 0x4
#define _MENU_FLAG 0x8
#define _INI_FILE_FLAG 0x10
#define _USE_RC4_FLAG 0x20

#define _ENABLE(x) controlStruct.flags |= x;
#define _DISABLE(x) controlStruct.flags &= ~x;

#define swap(a,b) a^=b; b^=a; a^=b;

char *tableItems[]{
	"Show Help (F1)",
	"Change input file name",
	"Show task info",
	"Change output file name",
	"Show student info",
	"Change key file name",
	"Show student e-mail",
	"Exit from menu (esc)",
	"Encode/Decode",
	"Exit"
};

char *settings[]{
  "SHOW_STUDENT_INFO",
  "SHOW_EMAIL",
  "SHOW_HELP",
  "SHOW_MENU",
  "RUN_ENCRYPTING",
  "INPUT_FILENAME",
  "OUTPUT_FILENAME",
  "KEY_FILENAME"
};

struct ControlStruct {
	unsigned flags;
	char fIniName[255] = "options.ini";
	char fInName[255] = "input.txt";
	char fOutName[255] = "encoded.txt";
	char fLogName[255] = "log.txt";
  char fKeyName[255] = "key.txt";
} controlStruct;

void drawLine();
void drawTable(int item);
void handleArrow(int arrow, int*);
void showHelp();
void showTask();
void showStudent();
void showEmail();
void changeFile(char*, int);
FILE* openFile(char *fileName, char *mode);
void enableFlag(char);
void disableFlag(char);
void executeFlags();
int getFlagValue(int);
void readFile(FILE*, char*);
void initializeFromFile(char*);
void log(char *fileName, int printTime);
void encode(FILE*, FILE*, char*);

char navPage[1000], RC4Page[1000], cmdArgsPage[1000], syntaxPage[1000]; //help pages

int main(int argc, char *argv[]) {
  log("Program starts...", 1);
  
  log("Caching help pages.", 1);
  readFile(openFile("help/navigation.txt", "r"), navPage);
  readFile(openFile("help/about_RC4.txt", "r"), RC4Page);
  readFile(openFile("help/Command_Arguments.txt", "r"), cmdArgsPage);
  readFile(openFile("help/options_syntax.txt", "r"), syntaxPage);
  log("Help pages has cached", 1);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));

  log("Received flags:", 1);
  for (int i = 1; i < argc; i++)
  {
    log(argv[i], 0);
    if (argv[i][0] != '/') {
      printf("Uknown flag '%s'", argv[i]);
      system("pause");
      continue;
    }
    else if (strlen(argv[i]) == 2) {
      enableFlag(argv[i][1]);
    }
    else if ((strlen(argv[i]) == 3) && (argv[i][1] == '!')) {
      disableFlag(argv[i][2]);
    }
    else if (!strcmp(argv[i], "/fin")) {
      changeFile(argv[i + 1], 1);
    }
    else if (!strcmp(argv[i], "/fout")) {
      changeFile(argv[i + 1], 3);
    }
    else if (!strcmp(argv[i], "/fini")) {
      changeFile(argv[i + 1], 2);
    }
    else {
      printf("Uknown flag '%s'", argv[i]);
    }
  }
  log("All flags have handled.", 1);

  system("cls");

  if (getFlagValue(4)) {
    log("Starting initialization from file...", 1);
    initializeFromFile(controlStruct.fIniName);
  }

  log("Executing flags", 1);
  executeFlags();
  log("Flags had executed", 1);

	int selectedItem = 0;
	char c, fileName[30], key[256];
  FILE *fIn, *fOut, *fKey;

  system("cls");
  //Menu
  if (getFlagValue(3)) {
    log("User entered the menu.", 1);
    do
    {
      drawTable(selectedItem);
      c = _getch();
      if (c == -32) {
        handleArrow(_getch(), &selectedItem);
      }
      else {
        if (c == 13) {
          system("cls");
          if (selectedItem == 7 || selectedItem == 9) break;
          switch (selectedItem)
          {
          case 0:
            log("User entered the help.", 1);
            showHelp();
            system("cls");
            continue;
          case 2:
            log("User entered the task info.", 1);
            showTask();
            break;
          case 1:
          case 3:
          case 5:
            printf("Enter file name: ");
            scanf("%s", fileName);
            changeFile(fileName, selectedItem);
            break;
          case 4:
            log("User entered student info.", 1);
            showStudent();
            break;
          case 6:
            log("User entered the email info.", 1);
            showEmail();
            break;
          case 8:
            log("User launches encoding.", 1);
            fIn = openFile(controlStruct.fInName, "rb");
            fOut = openFile(controlStruct.fOutName, "wb");
            fKey = openFile(controlStruct.fKeyName, "r");
            if (!fIn || !fOut || !fKey) break;
            readFile(fKey, key);
            encode(fIn, fOut, key);
          }
          system("pause");
        }
        else if (c == 0 && (c = _getch()) == 59) {
          log("User entered the help.", 1);
          system("cls");
          showHelp();
          system("cls");
        }
      }
      system("cls");
    } while (c != 27);
    log("User exited from menu.", 1);
  }
  
  log("Program successfully finished.", 1);
  log("logEnd", 0);
	return 0;
}

void drawLine() {
	for (int j = 0; j < 39; j++) putchar(205);
}

void drawTable(int selectedItem) {
	putchar(201);
	drawLine();
	putchar(203);
	drawLine();
	putchar(187);
	putchar('\n');
	int j = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int ii = 0; ii < 2; ii++, j++) {
			putchar(186);
			if (selectedItem == j) {
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 0));
				printf(">%-38s", tableItems[j]);
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
				continue;
			}
			printf("%-39s", tableItems[j]);
		}
		putchar(186);
		putchar('\n');
	}
	putchar(200);
	drawLine();
	putchar(202);
	drawLine();
	putchar(188);
	putchar('\n');
}

void handleArrow(int arrow, int *selectedItem) {
	switch (arrow)
	{
	case 72: //↑
		if (*selectedItem == 0) *selectedItem = 8;
		else if (*selectedItem == 1) *selectedItem = 9;
		else *selectedItem -= 2;
		break;
	case 80: //↓
		if (*selectedItem == 8) *selectedItem = 0;
		else if (*selectedItem == 9) *selectedItem = 1;
		else *selectedItem += 2;
		break;
	case 77: //→
		*selectedItem += 1;
		if (*selectedItem > 9) *selectedItem = 0;
		break;
	case 75: //←
		*selectedItem -= 1;
		if (*selectedItem < 0) *selectedItem = 9;
		break;
	}
}

void enableFlag(char flag)
{
  switch (flag)
  {
  case 's': 
  case 0:
    _ENABLE(_STUDENT_INFO_FLAG);
    break;
  case 'e': 
  case 1:
    _ENABLE(_EMAIL_INFO_FLAG);
    break;
  case 'h': 
  case 2:
    _ENABLE(_HELP_FLAG);
    break;
  case 'm':
  case 3:
    _ENABLE(_MENU_FLAG);
    break;
  case 'i':
    _ENABLE(_INI_FILE_FLAG);
    break;
  case 'c':
  case 4:
    _ENABLE(_USE_RC4_FLAG);
    break;
  default:
    printf("No such flag '%c'", flag);
  }
}

void disableFlag(char flag)
{
  switch (flag)
  {
  case 's':
  case 0:
    _DISABLE(_STUDENT_INFO_FLAG);
    break;
  case 'e':
  case 1:
    _DISABLE(_EMAIL_INFO_FLAG);
    break;
  case 'h':
  case 2:
    _DISABLE(_HELP_FLAG);
    break;
  case 'm':
  case 3:
    _DISABLE(_MENU_FLAG);
    break;
  case 'i':
    _DISABLE(_INI_FILE_FLAG);
    break;
  case 'c':
  case 4:
    _DISABLE(_USE_RC4_FLAG);
    break;
  default:
    printf("No such flag '%c'", flag);
  }
}

void executeFlags() {
  for (int i = 0; i < 3; i++)
  {
    system("cls");
    if (getFlagValue(i)) {
      switch (i) {
      case 0:
        showStudent();
        break;
      case 1:
        showEmail();
        break;
      case 2:
        showHelp();
        break;
      }
      if (i != 2) system("pause");
    }
  }
}

int getFlagValue(int flag) {
  return (controlStruct.flags >> flag) & 1;
}

void showHelp() {
  int page = 0;
  char c;
  do
  {
    system("cls");
    switch (page)
    {
    case 0:
      printf("%s", navPage);
      break;
    case 1:
      printf("\t\t%s\n\nThis program encrypts the input file(%s) using the RC4 cipher.\nAll encrypted information will be written to this file '%s'.\nThe encryption key is located in the file '%s'\n", "About", controlStruct.fInName, controlStruct.fOutName, controlStruct.fKeyName);
      break;
    case 2:
      printf("%s", RC4Page);
      break;
    case 3:
      printf("%s", cmdArgsPage);
      break;
    case 4:
      printf("%s", syntaxPage);
    }
    c = _getch();
    if (c == -32) {
      if ((c = _getch()) == 73) page--;
      else if (c == 81) page++;
      if (page > 4) page = 0;
      else if (page < 0) page = 4;
    }
  } while (c != 27);
}

void showTask() {
	printf("Encrypting with RC4.\n");
}

void showStudent() {
	printf("Dedela Arthur\nT30-102B-16\n");
}

void showEmail() {
	printf("Email: example@example.com\n");
}

void changeFile(char *fileName, int fileType) {
  switch (fileType)
  {
  case 1:
    log("User changed input file name.", 1);
    strcpy(controlStruct.fInName, fileName);
    break;
  case 2:
    strcpy(controlStruct.fIniName, fileName);
    break;
  case 3:
    log("User changed output file name.", 1);
    strcpy(controlStruct.fOutName, fileName);
    break;
  case 5:
    log("User changed key file name.", 1);
    strcpy(controlStruct.fKeyName, fileName);
  }
  printf("File successfully changed!\n");
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
  char c, *ptr = buf;
  while ((c = fgetc(f)) != EOF) {
    *ptr++ = c;
  }
  *ptr = 0;
}

void initializeFromFile(char *fileName)
{
  printf("Initializing file '%s'", fileName);
  FILE *f;
  char c, c_ = ' ', buf[30], *ptr = buf;
  int i;
  if (!(f = openFile(fileName, "r"))) {
    printf("Initializing from file '%s' failed!\nUsing default settings.\n");
    return;
  }
  while ((c = fgetc(f)) != EOF)
  {
    if (isgraph(c) && (c != '=')) *ptr++ = c;
    else if (c == '=') {
      *ptr = 0;
      for (i = 0; i < 8; i++)
      {
        if (!strcmp(buf, settings[i])) {
          if (i < 5) {
            if ((c = fgetc(f)) == 'Y') enableFlag(i);
            else if (c == 'N') disableFlag(i);
            else {
              printf("Syntax error! Uknown flag value!");
              log("Initialization failed. Syntax error! Uknown flag value!", 1);
              system("pause");
              return;
            }
            if (fgetc(f) != ';') {
              printf("Syntax error! Missing semicolon!");
              log("Initialization failed. Syntax error! Missing semicolon!", 1);
              system("pause");
              return;
            }
          }
          else {
            ptr = buf;
            while ((c = fgetc(f)) != ';') {
              *ptr++ = c;
            }
            *ptr = 0;
            switch (i)
            {
            case 5: 
              changeFile(buf, 1);
              break;
            case 6:
              changeFile(buf, 3);
              break;
            case 7:
              changeFile(buf, 5);
              break;
            }
          }
          break;
        }
      }//for end
      if (i == 8) {
        printf("Syntax error! Uknown property '%s'", buf);
        log("Initialization failed! Syntax error, uknown property!", 1);
        system("pause");
        return;
      }
      ptr = buf;
    }
  }
  log("Initialization success.", 1);
}

void log(char *logMessage, int printTime)
{
  static FILE *fLog = openFile(controlStruct.fLogName, "w");

  if (!(strcmp(logMessage, "logEnd"))) {
    fclose(fLog);
    return;
  }

  if (printTime) {
    time_t rawtime;
    struct tm *curTime;
    time(&rawtime);
    curTime = localtime(&rawtime);
    fprintf(fLog, "%02d:%02d:%02d -  ", curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
  }
  fprintf(fLog, "%s\n", logMessage);
}

void encode(FILE *fIn, FILE *fOut, char *key)
{
  //S-block initialization
  int keyLen = strlen(key);
  int i;
  byte S[256];
  for (i = 0; i < 256; i++) {
    S[i] = i;
  }
  int j = 0;
  for (i = 0; i < 256; i++) {
    j = (j + S[i] + key[i % keyLen]) % 256; // n = 8 ; 28 = 256
    swap(S[i], S[j]);
  }
  //End s-block

  byte bytesBuf[4096], K;

  i = 0;
  j = 0;
  int t;
  int bytesCnt;

  while ((bytesCnt = fread(bytesBuf, sizeof(byte), 4096, fIn)) > 0)
  {
    for (int b = 0; b < bytesCnt; b++) {
      i = (i + 1) % 256;
      j = (j + S[i]) % 256;
      swap(S[i], S[j]);
      t = (S[i] + S[j]) % 256;
      K = S[t];
      bytesBuf[b] ^= K;
    }
    fwrite(bytesBuf, sizeof(byte), bytesCnt, fOut);
  }

  fclose(fIn);
  fclose(fOut);
  printf("Encoding finished!\n");
  log("Encoding end.", 1);
}
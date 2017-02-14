#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define _STUDENT_INFO_FLAG 0x1
#define _EMAIL_INFO_FLAG 0x2
#define _MENU_FLAG 0x4
#define _HELP_FLAG 0x8
#define _INI_FILE_FLAG 0x10
#define _USE_RC4_FLAG 0x20

char *tableItems[]{
	"Show Help (F1)",
	"Change input file name",
	"Show task info",
	"Change output file name",
	"Show student info",
	"Change key file name",
	"Show student e-mail",
	"Exit from menu (esc)",
	"Start task",
	"Exit"
};

struct ControlStruct {
	unsigned flags;
	char fIniName[255] = "options.ini";
	char fInName[255] = "input.txt";
	char fOutName[255] = "output.txt";
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
void changeFile(int);
int isExistingFile(char*);

FILE *fIn, *fOut, *fKey;

int main(int argc, char *argv[]) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '/') {
      printf("Uknown flag '%s'", argv[i]);
      system("pause");
      continue;
    }
  }

	int selectedItem = 0;
	char c;

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
					showHelp();
          system("cls");
          continue;
				case 2:
					showTask();
          break;
				case 1:
				case 3:
				case 5:
					changeFile(selectedItem);
					break;
				case 4:
					showStudent();
					break;
				case 6:
					showEmail();
					break;
        /*case 8:
          encrypt();*/
				}
        system("pause");
			}
			else if (c == 0 && (c = _getch()) == 59) {
        system("cls");
				showHelp();
			}
		}
    system("cls");
	} while (c != 27);
  

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

void showHelp() {
  int page = 0;
  char c;
  do
  {
    system("cls");
    switch (page)
    {
    case 0:
      printf("%60s\n\nTo navigate in help use <PgUp> and <PgDn> or <Esc> to exit.\nTo navigate in menu use arrows, to select item press <Enter>.\nTo exit from menu press <Esc>\n", "Navigation");
      break;
    case 1:
      printf("%60s\n\nThis program encrypts the input file(%s) using the RC4 cipher.\nAll encrypted information will be written to this file '%s'.\nThe encryption key is located in the file '%s'\n", "About", controlStruct.fInName, controlStruct.fOutName, controlStruct.fKeyName);
      break;
    case 2:
      printf("%60s\n\nRC4 generates a pseudorandom stream of bits (a keystream). As with any stream cipher, these can be used for encryption by combining it with the plaintext using bit-wise exclusive-or; decryption is performed the same way (since exclusive-or with given data is an involution). (This is similar to the Vernam cipher except that generated pseudorandom bits, rather than a prepared stream, are used.) To generate the keystream, the cipher makes use of a secret internal state which consists of two parts:\n1) A permutation of all 256 possible bytes.\n2) Two 8 - bit index - pointers.\nThe permutation is initialized with a variable length key, typically between 40 and 2048 bits, using the key - scheduling algorithm(KSA).Once this has been completed, the stream of bits is generated using the pseudo - random generation algorithm(PRGA).\n",  "About RC4");
      break;
    case 3:
      printf("%60s\n\n/s - shows student information.\n/e - shows email.\n/h - shows help\n/i - uses the initialization file\n/c - uses RC4\n/fini fileName - reassign initializing file\n/fin fileName - reassign input file\n/fout fileName - reassign ouput file\n", "Command arguments");
      break;
    case 4:
      printf("%60s\n\nAt first comes property name, then, after equal sign, property value.\nRules always must end with semicolon.\n\nExample:\nSHOW_STUDENT_INFO = N;\nSHOW_EMAIL = N;\nSHOW_MENU = N;\nSHOW_HELP = Y;\nRUN_ENCRYPTING = Y;\nINPUT_FILENAME = input.txt;\nOUTPUT_FILENAME = output.txt;\nKEY_FILENAME = key.txt;", "Initialization file syntax");
    }
    c = _getch();
    if (c == -32) {
      if ((c = _getch()) == 73) page--;
      else if (c == 81) page++;
      if (page > 4) page = 0;
      else if (page < 0) page = 2;
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

void changeFile(int fileType) {
	char fileName[30];
	printf("Enter new file name: ");
	scanf("%s", fileName);
  if (isExistingFile(fileName)) {
    switch (fileType)
    {
    case 1:
      strcpy(controlStruct.fInName, fileName);
      break;
    case 3:
      strcpy(controlStruct.fOutName, fileName);
      break;
    case 5:
      strcpy(controlStruct.fKeyName, fileName);
    }
    printf("File successfully changed!\n");
  }
  
}

int isExistingFile(char *fileName) {
	FILE *fTemp;
	if (!(fTemp = fopen(fileName, "r"))) {
		printf("Error #404!\nFile not found!\n");
		return 0;
	}
  fclose(fTemp);
  return 1;
}
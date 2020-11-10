// Programmed by Makhovikov Andrey
// Compiled using Borland C++ v3.1
//
#include <fstream.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>

#define YES 1
#define NO  0

// Name "TESTER"
char warrior_man[0x3A] = {
			      0x22,0x05,
0x74,0x8E,0x03,0xCC,0xB7,0x41,0xB6,0x54,
0x20,0x40,0x81,0x02,0x05,0x0A,0x14,0x29,

0x06,0xE1,0x1B,0xC5,0x67,0x16,0x88,0x10,
0x21,0x40,0x81,0x02,0x81,0x02,0x14,0x29,

0x44,0xA0,0x48,0x91,0x22,0x44,0x88,0x10,
0x20,0x40,0x81,0x02,0x45,0x51,0x1F,0x28,

0x52,0x84,0x49,0x95,0x22,0xE5,0xC0,0x10
			};

char warrior_woman[0x3A] = {
			      0xB3,0x13,
0x34,0xC9,0x03,0xC0,0xAC,0x30,0xD1,0x7D,
0x31,0x62,0xC4,0x89,0x13,0x27,0x4E,0x9D,

0x6F,0x33,0xBF,0x8D,0xF6,0x34,0xCC,0x98,
0x30,0x62,0xC4,0x89,0x93,0x2A,0x4E,0x9D,

0x2D,0x72,0xEC,0xD9,0xB3,0x66,0xCC,0x98,
0x31,0x62,0xC4,0x89,0x53,0x7C,0x45,0x9C,

0xBB,0x7D,0xED,0xDD,0xB3,0xC7,0x84,0x98
			};

char mage_man[0x3A] = {
			      0xE6,0x7B,
0x34,0xAA,0x03,0xC0,0x29,0x49,0xE3,0xC1,
0x67,0xCF,0x9E,0x3D,0x7B,0xF7,0xEF,0xDF,

0xEA,0x39,0xAA,0xA7,0xA3,0x9E,0x99,0x33,
0x66,0xCF,0x9E,0x3D,0xFB,0xFA,0xEF,0xDF,

0xA8,0x78,0xF9,0xF3,0xE6,0xCC,0x99,0x33,
0x67,0xCF,0x9E,0x3D,0x3B,0xAC,0xE4,0xDE,

0xFE,0x73,0xF8,0xF7,0xE6,0x6D,0xD1,0x33
			};

char mage_woman[0x3A] = {
			      0xBB,0x0B,
0xE0,0x93,0x92,0x75,0x6A,0xC7,0x97,0x2E,
0xB0,0x61,0xC2,0x85,0x0B,0x17,0x2E,0x5D,

0xEF,0x32,0xBD,0x89,0xFE,0x24,0xEC,0xD8,
0xA0,0x61,0xC2,0x85,0x13,0xC6,0x49,0x5D,

0x13,0xA1,0x7E,0xDD,0x3E,0xD2,0xBC,0xD8,
0x40,0xA0,0xAB,0x85,0x13,0xC6,0x49,0x5D,

0x7B,0x71,0xEF,0xD9,0xBB,0x76,0xEC,0xD8
			};


void main(int argc, char **argv)

{
// ========================
// === Define variables ===
// ========================

   ifstream input_file;
   ofstream output_file;

   char *character;
   char *person_info;
   struct find_t ffblk;
   int done, file_is_found = NO;

   cout << "\n";
   cout << "\t\tÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n";
   cout << "\t\t³              A2LOCAL v.1.1             ³\n";
   cout << "\t\tÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "\t\t³      Written by Andrey Makhovikov      ³\n";
   cout << "\t\t³  E-mail: makhovikov_andrey@mtu-net.ru  ³\n";
   cout << "\t\tÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "\t\t³  You can now use *.a2c from server in  ³\n";
   cout << "\t\t³         a local game (Allods II)       ³\n";
   cout << "\t\t³    FREEWARE - USE ON YOUR OWN RISK!    ³\n";
   cout << "\t\tÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n";

   if (argc > 2)
   {
	if (!stricmp(argv[2],"-w")) person_info = warrior_man;
   else if (!stricmp(argv[2],"-ww")) person_info = warrior_woman;
   else if (!stricmp(argv[2],"-m")) person_info = mage_man;
   else if (!stricmp(argv[2],"-mm")) person_info = mage_woman;
   else
      {
	cout << "Unknown parameter!\n";
	cout << "You must enter correct character type:\n";
	cout << "-w  - Warrior, man\n";
	cout << "-ww - Warrior, woman\n";
	cout << "-m  - Mage, man\n";
	cout << "-mm - Mage, woman\n\n";
	cout << "Sample: a2local 2395361456.a2c -w\n";
	exit(EXIT_FAILURE);
      }
   }
   else
      {
	cout << "You must enter character filename and its type:\n";
	cout << "-w  - Warrior, man\n";
	cout << "-ww - Warrior, woman\n";
	cout << "-m  - Mage, man\n";
	cout << "-mm - Mage, woman\n\n";
	cout << "Sample: a2local 2395361456.a2c -w\n";
	exit(EXIT_FAILURE);
      }

   cout << "File\t\tSize\tDate\t\tResult\n";
   cout << "----\t\t----\t----\t\t------\n";

   done = _dos_findfirst("*.a2c", _A_NORMAL, &ffblk);

   for (; !done; done = _dos_findnext(&ffblk))
   {
   if (!stricmp(ffblk.name, "11111111.a2c")) continue;
   cout << ffblk.name << "\t";

/// Read file
   input_file. open(ffblk.name, ios::binary | ios::in | ios::nocreate);
   if (input_file. fail())
	  {
		cout << "Character file not available.\n";
		input_file. close();
		continue;
	  }

    cout << ffblk.size << "\t" << ((ffblk.wr_date << 11) >> 11) << "." << ((ffblk.wr_date << 7) >> 12) << "." << (1980 + (ffblk.wr_date >> 9));

    if (!stricmp(ffblk.name, argv[1]))
     {
    character = new char [(int) ffblk.size];

    input_file. read(character, (int) ffblk.size);
    if (input_file. fail())
	   {
		cout << "Character file can not be read.\n";
		input_file. close();
		continue;
	   }

/// Making output file
    output_file. open("11111111.a2c", ios::binary | ios::out);
    if (output_file. fail())
	  {
		cout << "Can not back up character.\n\n";
		continue;
	  }

    memcpy(character + 14, person_info, 0x3A);

    output_file. write(character, (int) ffblk.size);
    if (output_file. fail())
	   {
		cout << "Character file can not be written.\n";
		output_file. close();
		continue;
	   }

    output_file. close();
    cout << "\tDone!";
    delete character;
    file_is_found = YES;
     }
    input_file. close();
    cout << "\n";
   }

/// Clean up
   if (!file_is_found) cout << "\nRequired file not found!\nRemember not to enter long names in the DOS prompt.\n";
   exit(EXIT_SUCCESS);
}
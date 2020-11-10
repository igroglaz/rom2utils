// Programmed by Makhovikov Andrey
// Compiled using Borland C++ v3.1
//
#include <fstream.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>

void main(void)

{

// ========================
// === Define variables ===
// ========================

   ifstream input_file;
   char *character;
   struct find_t ffblk;
   int done;

   cout << "\n";
   cout << "\t\tÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n";
   cout << "\t\t³             CHR_INFO v.1.0             ³\n";
   cout << "\t\tÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "\t\t³      Written by Andrey Makhovikov      ³\n";
   cout << "\t\t³  E-mail: makhovikov_andrey@mtu-net.ru  ³\n";
   cout << "\t\t³       for Allods: Sealed Mystery       ³\n";
   cout << "\t\t³    FREEWARE - USE ON YOUR OWN RISK!    ³\n";
   cout << "\t\tÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n";

   done = _dos_findfirst("game????.chr", _A_NORMAL, &ffblk);
   cout << "File\t\t Size\tCharacter\tType\t\tOrigin\n";
   cout << "----\t\t ----\t---------\t----\t\t------\n";

   for (; !done; done = _dos_findnext(&ffblk))
   {
   cout << ffblk.name << "\t";
   input_file. open(ffblk.name, ios::binary | ios::in | ios::nocreate);
   if (input_file. fail())
	  {
		cout << " not available\n";
		input_file. close();
		continue;
	  }

    character = new char [(int) ffblk.size];

    input_file. read(character, (int) ffblk.size);
    if (input_file. fail())
	   {
		cout << " cannot be read\n";
		input_file. close();
		delete character;
		continue;
	   }
    input_file. close();
    cout << " " << ffblk.size << "\t";

    if (memcmp(character,"AlCh",4))
	   {
		cout << "damaged or not a character file\n";
		input_file. close();
		delete character;
		continue;
	   }
    cout << (character + 0x0C);
    if (strlen(character + 0x0C) < 16) cout << "\t";
    if (strlen(character + 0x0C) < 8) cout << "\t";

    switch (character[0x50])
       {
	 case 0:    cout << "Warrior\t\t";
		    break;
	 default:   cout << "Mage\t\t";
       }

    switch (character[0x6C])
       {
	 case 0x1C: cout << "IDOWEB\n";
		    break;
	 case 0x64: cout << "OGO\n";
		    break;
	 case 0:    cout << "Local\n";
		    break;
	 default:   cout << "Unknown\n";
       }
    delete character;
   }

   exit(EXIT_SUCCESS);
}
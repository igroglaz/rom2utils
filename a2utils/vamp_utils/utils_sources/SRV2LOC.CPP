// Programmed by Makhovikov Andrey
// Compiled using Borland C++ v3.1
//
#include <fstream.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>

const int POSITION = 0x6C;      // Position in a file
const char DATA_BEFORE1 = 0x1C; // Data before in this byte
const char DATA_BEFORE2 = 0x64; // Data before in this byte

void main(void)
{
// ========================
// === Define variables ===
// ========================

   ifstream input_file;
   ofstream output_file;
   char *character;
   struct find_t ffblk;
   int done;

   cout << "\n";
   cout << "\t\tÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n";
   cout << "\t\t³              SRV2LOC v.1.1             ³\n";
   cout << "\t\tÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "\t\t³      Written by Andrey Makhovikov      ³\n";
   cout << "\t\t³  E-mail: makhovikov_andrey@mtu-net.ru  ³\n";
   cout << "\t\t³       for Allods: Sealed Mystery       ³\n";
   cout << "\t\t³    FREEWARE - USE ON YOUR OWN RISK!    ³\n";
   cout << "\t\tÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n";

   cout << "File\t      Size\tResult\n";
   cout << "----\t      ----\t------\n";
   done = _dos_findfirst("game*.chr", _A_NORMAL, &ffblk);

   for (; !done; done = _dos_findnext(&ffblk))
   {
   cout << ffblk.name << " ";

// Read file
   input_file. open(ffblk.name, ios::binary | ios::in | ios::nocreate);
   if (input_file. fail())
	  {
		cout << "Character file not available.\n";
		input_file. close();
		continue;
	  }

   cout << " " << ffblk.size << "\t";
   character = new char [(int) ffblk.size];

   input_file. read(character, (int) ffblk.size);
   if (input_file. fail())
	   {
		cout << "Character file can not be read.\n";
		input_file. close();
		continue;
	   }
   input_file. close();

// Some checks
   if (memcmp(character, "AlCh", 4))
       {
	 cout << "Unknown file format. Conversion impossible.\n";
	 continue;
       }

   if (character[POSITION] == 0)
       {
	 cout << "Local character. No conversion needed.\n";
	 continue;
       }

// Making back-up file
   memcpy(ffblk.name + 9, "bak", 3);
   output_file. open(ffblk.name, ios::binary | ios::out);
   if (output_file. fail())
	  {
		cout << "Can not back up character.\n\n";
		continue;
	  }

   output_file. write(character, (int) ffblk.size);
   if (input_file. fail())
	   {
		cout << "Character file can not be written.\n";
		output_file. close();
		continue;
	   }

   output_file. close();

// Making changes
   if (character[POSITION] == DATA_BEFORE1 || character[POSITION] == DATA_BEFORE2)
	{
	    cout << "SUCCESSFULLY CONVERTED!\n";
	    character[POSITION] = 0;
	}
   else
	{
	    cout << "Unknown server type. Conversion may not work.\n";
	    character[POSITION] = 0;
	}

// Writing new file
   memcpy(ffblk.name + 9, "chr", 3);

   output_file. open(ffblk.name, ios::binary | ios::out);
   if (output_file. fail())
	  {
		cout << "Can not update character.\n\n";
		continue;
	  }

   output_file. write(character, (int) ffblk.size);
   if (input_file. fail())
	   {
		cout << "Character file can not be written.\n";
		output_file. close();
		continue;
	   }

   output_file. close();
   delete character;
  }

// Clean up
   delete character;
   exit(EXIT_SUCCESS);
}
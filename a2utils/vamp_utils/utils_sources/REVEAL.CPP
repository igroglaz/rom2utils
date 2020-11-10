// Programmed by Makhovikov Andrey
// Compiled using Borland C++ v3.1
//
#include <stdlib.h>
#include <fstream.h>
#include <process.h>
#define YES 1
#define NO  0

int WRITE = YES;

void main(int argc, char** argv)

{
// Info
   cout << "\n\n";
   cout << "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n";
   cout << "³         Map Revealer Utility v.1.0         ³\n";
   cout << "³          Written by A. Makhovikov          ³\n";
   cout << "³    E-mail: makhovikov_andrey@mtu-net.ru    ³\n";
   cout << "ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "³         For Allods/RoM  Map Editor         ³\n";
   cout << "³              v1.04 or earlier              ³\n";
   cout << "³                                            ³\n";
   cout << "³      FREEWARE - USE ON YOUR OWN RISK!      ³\n";
   cout << "ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "³       Usage:   REVEAL <your_map.alm>       ³\n";
   cout << "ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "³  Apply to maps that cause editor crashes.  ³\n";
   cout << "³  This sometimes  happens with client maps  ³\n";
   cout << "³  and after converting from *.ALL to *.ALM  ³\n";
   cout << "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n";

// Check command line parameters
   if (argc < 2)
     {
	 cout << "A map file name must be entered\n";
	 exit (EXIT_FAILURE);
     }

   fstream input_file;
   unsigned char header[5];

// Open source amd destination files
   input_file. open(argv[1], ios::binary | ios::in | ios::out | ios::nocreate);
   if (input_file. fail())
     {
	 cout << "No map file found. Terminating...\n";
	 exit (EXIT_FAILURE);
     }


// ==============================
// === Check map header - M7R ===
// ==============================

   input_file. read(header, 5);
   if (input_file. fail() || input_file. eof() || header[0] != 'M' || header[1] != '7' || header[2] != 'R' || header[3] != 0 || header[4] != 0x14)
     {
	 cout << "File is not a map. Terminating...\n";
	 input_file. close();
	 exit (EXIT_FAILURE);
     }


// We disable defence here that makes the editor crash
   input_file. seekp(0x50);
   input_file. write("\0\0\0\0\0\0\0\0", 8);


   input_file. close();
   cout << "Map patched!\n";
   exit (EXIT_SUCCESS);
}
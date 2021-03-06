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
   cout << "旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커\n";
   cout << "�         Map Revealer Utility v.1.0         �\n";
   cout << "�          Written by A. Makhovikov          �\n";
   cout << "�    E-mail: makhovikov_andrey@mtu-net.ru    �\n";
   cout << "쳐컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑\n";
   cout << "�         For Allods/RoM  Map Editor         �\n";
   cout << "�              v1.04 or earlier              �\n";
   cout << "�                                            �\n";
   cout << "�      FREEWARE - USE ON YOUR OWN RISK!      �\n";
   cout << "쳐컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑\n";
   cout << "�       Usage:   REVEAL <your_map.alm>       �\n";
   cout << "쳐컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑\n";
   cout << "�  Apply to maps that cause editor crashes.  �\n";
   cout << "�  This sometimes  happens with client maps  �\n";
   cout << "�  and after converting from *.ALL to *.ALM  �\n";
   cout << "읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸\n\n";

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
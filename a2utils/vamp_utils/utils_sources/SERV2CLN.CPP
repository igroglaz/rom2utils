// Programmed by Makhovikov Andrey
// Compiled using Borland C++ v3.1
//
#include <fstream.h>
#include <stdlib.h>
#include <string.h>


void main(int argc, char **argv)

{
// ============
// === Info ===
// ============

   cout << "\n\n";
   cout << "旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커\n";
   cout << "�  Server-to-Client Map Converter v.1.0  �\n";
   cout << "쳐컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑\n";
   cout << "�      Written by Andrey Makhovikov      �\n";
   cout << "�  E-mail: makhovikov_andrey@mtu-net.ru  �\n";
   cout << "�                                        �\n";
   cout << "�       for Allods: Sealed Mystery       �\n";
   cout << "�   and Rage of Mages multiplayer game   �\n";
   cout << "�                                        �\n";
   cout << "�    FREEWARE - USE ON YOUR OWN RISK!    �\n";
   cout << "쳐컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴캑\n";
   cout << "� Usage:  SERV2CLN server_map client_map �\n";
   cout << "읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸\n";

   if (argc < 3)
      {
	cout << "\nYou must enter source and destination filenames\n";
	exit(EXIT_FAILURE);
      }

// Source and destination must be different
   if (!strcmpi(argv[1], argv[2]))
     {
	 cout << "\nFilenames must be different\n";
	 exit(EXIT_FAILURE);
     }

// ========================
// === Define variables ===
// ========================

   ifstream input_file;
   ofstream output_file;
   unsigned char c, header[9], etalon[4];
   long i, LIMIT = 0;
   char alm_header[9] = {'M', '7', 'R', 0, 0x14, 0, 0, 0, 0x48};
   int status = 0;

   input_file. open(argv[1], ios::binary | ios::in | ios::nocreate);
   if (input_file. fail())
     {
	 cout << "\nNo source file found. Terminating...\n";
	 exit(EXIT_FAILURE);
     }

// ==============================
// === Check map header - M7R ===
// ==============================

   input_file. read(header, 9);
   if (input_file. fail() || input_file. eof() || memcmp(header, alm_header, 9))
     {
	 cout << "\nSource file is not a map. Terminating...\n";
	 input_file. close();
	 exit(EXIT_FAILURE);
     }

// ======================
// === Check map size ===
// ======================

   for (i = 9; ; i++)
     {
	 input_file. read(&c, 1);
	 if (input_file. eof()) break;
	 if (input_file. fail())
	   {
		cout << "\nSource file can not be read. Terminating...\n";
		input_file. close();
		exit(EXIT_FAILURE);
	   }

	 if (i == 0x0C && c != 0x0A) // Client Map Id
	   {
		cout << "\nSource file is not a server map. Terminating...\n";
		input_file. close();
		exit(EXIT_FAILURE);
	   }

	 if (i == 0x23)
	 {
	 input_file. read(etalon, 4);
	 i += 4;
	 cout << "\nConverting in progress.";
	 if (input_file. fail() || input_file. eof())
	     {
		cout << "\nSource file can not be read. Terminating...\n";
		input_file. close();
		exit(EXIT_FAILURE);
	     }
	  }
	  if ((i / 20000 * 20000) == i) cout << "."; // Progress indicator

// =======================
// === Detect group Id ===
// =======================

	 switch (status)
	   {
		case 0:	if (c == 5) status++; else status = 0;
			break;
		case 1:	if (c == 0) status++; else status = 0;
			break;
		case 2:	if (c == 0) status++; else status = 0;
			break;
		case 3:	if (c == 0) status++; else status = 0;
			break;
		case 4:	if (c == etalon[0]) status++; else status = 0;
			break;
		case 5:	if (c == etalon[1]) status++; else status = 0;
			break;
		case 6:	if (c == etalon[2]) status++; else status = 0;
			break;
		case 7:	if (c == etalon[3]) status++; else status = 0;
			break;
		case 8: LIMIT = i - 20;
			status = 0;
	    }
     }

   input_file. close();
// Check if Group 5 is really found
   if (LIMIT == 0)
     {
	 cout << "\nError found in server map. Terminating...\n";
	 exit(EXIT_FAILURE);
     }

// =============================
// === Make destination file ===
// =============================

   input_file. open(argv[1], ios::binary | ios::in | ios::nocreate);
   if (input_file. fail())
     {
	 cout << "\nNo source file found. Terminating...\n";
	 exit(EXIT_FAILURE);
     }

   output_file. open(argv[2], ios::binary | ios::out | ios::noreplace);
   if (output_file. fail())
     {
	 cout << "\nDestination file already exists. Terminating...\n";
	 input_file. close();
	 exit(EXIT_FAILURE);
     }

   for(i = 0; i < LIMIT; i++)
   {
   input_file. read(&c, 1);
   if ((i / 20000 * 20000) == i) cout << "."; // Progress indicator
   if (input_file. eof()) break;
   if (input_file. fail())
     {
	 cout << "\nSource file can not be read. Terminating...\n";
	 input_file. close();
	 output_file. close();
	 exit(EXIT_FAILURE);
     }

   if (i == 0x0C) c = 4; // Client Map Id
   if (i == 0x98) c = 1; // Number of players (1 - for client map)

   output_file. write(&c, 1);
   if (output_file. fail())
     {
	  cout << "\nDestination file can not be written. Terminating...\n";
	  input_file. close();
	  output_file. close();
	  exit(EXIT_FAILURE);
     }
   }

// ============
// === Exit ===
// ============

   cout << " Done!\n";
   input_file. close();
   output_file. close();
   exit(EXIT_SUCCESS);
}
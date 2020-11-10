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
   cout << "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n";
   cout << "³            MapLevel  v.1.1             ³\n";
   cout << "³            (Light Edition)             ³\n";
   cout << "³ Changes level of monsters in range 1-4 ³\n";
   cout << "³  Does not affect human units and NPCs  ³\n";
   cout << "ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "³      Written by Andrey Makhovikov      ³\n";
   cout << "³  E-mail: makhovikov_andrey@mtu-net.ru  ³\n";
   cout << "³                                        ³\n";
   cout << "³   for Allods, Allods 2, RoM and RoM2   ³\n";
   cout << "³    FREEWARE - USE ON YOUR OWN RISK!    ³\n";
   cout << "ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´\n";
   cout << "³   Usage:  MAPLEVEL map new_map <+/->   ³\n";
   cout << "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n";

   if (argc < 4)
      {
	cout << "\nYou must enter source, destination and mode\n";
	cout << "Sample: MAPLEVEL shallow.alm shallow1.alm +\n";
	exit(EXIT_FAILURE);
      }

   if (argv[3][0] != '+' && argv[3][0] != '-' )
      {
	cout << "\nIncorrect mode. Use \"+\" or \"-\" symbols only.\n";
	cout << "Sample: MAPLEVEL cross.alm cross1.alm +\n";
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
   fstream output_file;
   unsigned char c, cc, ccc, header[9], etalon[4], group = 6;
   long i;
   char alm_header[9] = {'M', '7', 'R', 0, 0x14, 0, 0, 0, 0x48};
   int status = 0, status1 = 0;
   int VERSION = 1; // Version of Allods/RoM

// ============================
// === Open source 1st time ===
// ============================

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

   for (i = 9; i < 0x24; i++)
     {
	 input_file. read(&c, 1);
	 if (input_file. fail() || input_file. eof())
	   {
		cout << "\nSource file can not be read. Terminating...\n";
		input_file. close();
		exit(EXIT_FAILURE);
	   }


	 if (i == 0x0C)
		if (c == 0x0A) VERSION = 1; // Server Map Id
		else if (c == 0x0D) VERSION = 2;
		else
		   {
			cout << "\nMap format is not supported. Terminating...\n";
			input_file. close();
			exit(EXIT_FAILURE);
		   }
     }

   input_file. read(etalon, 4);
   cout << "\nConverting in progress... ";
   if (input_file. fail() || input_file. eof())
    {
	cout << "\nSource file can not be read. Terminating...\n";
	input_file. close();
	exit(EXIT_FAILURE);
    }
   input_file. seekg(0); // Return to begining of the file

// =============================
// === Open destination file ===
// =============================

   output_file. open(argv[2], ios::binary | ios::out | ios::noreplace);
   if (output_file. fail())
     {
	 cout << "\nDestination file already exists. Terminating...\n";
	 input_file. close();
	 exit(EXIT_FAILURE);
     }


/////////////////////////////////////////////////////////////////////
//////////////////////////// Allods 2 ///////////////////////////////
/////////////////////////////////////////////////////////////////////

 if (VERSION == 2)

  {
// =======================================
// === Main cycle for searching groups ===
// =======================================

   for (i = 0; ; i++)
     {
	 input_file. read(&c, 1);
	 if (input_file. eof()) break;
	 if (input_file. fail())
	   {
		cout << "\nSource file can not be read. Terminating...\n";
		input_file. close();
		output_file. close();
		exit(EXIT_FAILURE);
	   }

// =======================
// === Detect group Id ===
// =======================

	 switch (status)
	   {
		case 0:	if (c == 7) status++; else status = 0;
			break;
		case 1:	if (c == 0) status++; else status = 0;
			break;
		case 2:	if (c == 0) status++; else status = 0;
			break;
		case 3:	if (c == 0) status++; else status = 0;
			break;
		case 4:	if (c == 0x14) status++; else status = 0;
			break;
		case 5:	if (c == 0) status++; else status = 0;
			break;
		case 6:	if (c == 0) status++; else status = 0;
			break;
		case 7:	if (c == 0) status++; else status = 0;
			break;
		case 8:	if (c == group) status++;
			break;
		case 9:	if (c == 0) status++; else status = 0;
			break;
		case 10: if (c == 0) status++; else status = 0;
			break;
		case 11: if (c == 0) status++; else status = 0;
			break;
		case 12: if (c == etalon[0]) status++; else status = 0;
			break;
		case 13: if (c == etalon[1]) status++; else status = 0;
			break;
		case 14: if (c == etalon[2]) status++; else status = 0;
			break;
		case 15: if (c == etalon[3]) status++; else status = 0;
			break;
		case 16: group++;
			status = 0;
	    }

	 if (group == 7) // Units location table + 1
	 {
// ===========================
// === Check unit sequence ===
// ===========================

	 switch (status1)
	   {
		case 0:	 if (c == 0x80) status1++; else status1 = 0;
			 break;
		case 4:  if (c == 0x80) status1++; else status1 = 0;
			 break;
		case 8:  status1++; // Change picture of the monster
			 ccc = c;   // Monster picture
			 break;
		case 10: status1++; // Change level of the monster
			 if (c >= 1 && c <= 4)
			  {
// =============================
// === Levels 1-4 down or up ===
// =============================
		cc = c;
		if (argv[3][0] == '-' && (cc - 1) >= 1 && (cc - 1) <= 4)
			{
				c--;
				ccc--;
			}
		if (argv[3][0] == '+' && (cc + 1) >= 1 && (cc + 1) <= 4)
			{
				c++;
				ccc++;
			}
		cout << (int) c << " ";
			  }
			 break;
		case 20: status1++; // Change position of the monster in editor list
		if (argv[3][0] == '-' && (cc - 1) >= 1 && (cc - 1) <= 4) c--;
		if (argv[3][0] == '+' && (cc + 1) >= 1 && (cc + 1) <= 4) c++;
			break;
		case 32: status1++;
			 if (c != 0x0A) c = 0x0A;
			 break;
		case 42: if (c == 0x80) status1++;
			 else
			 {
			  cout << "+";
			  status1 = 0; break;
			 }
			 break;
		case 47: status1 = 0;
			 break;
		default: status1++;
	   }
	 }

// =================================
// === Write to destination file ===
// =================================

   output_file. write(&c, 1);
   if (output_file. fail())
	{
	  cout << "\nDestination file can not be written. Terminating...\n";
	  input_file. close();
	  output_file. close();
	  exit(EXIT_FAILURE);
	}
     }
 }


/////////////////////////////////////////////////////////////////////
//////////////////////////// Allods 1 ///////////////////////////////
/////////////////////////////////////////////////////////////////////

  if (VERSION == 1)
  {

// =======================================
// === Main cycle for searching groups ===
// =======================================

   for (i = 0; ; i++)
     {
	 input_file. read(&c, 1);
	 if (input_file. eof()) break;
	 if (input_file. fail())
	   {
		cout << "\nSource file can not be read. Terminating...\n";
		input_file. close();
		output_file. close();
		exit(EXIT_FAILURE);
	   }

// =======================
// === Detect group Id ===
// =======================

	 switch (status)
	   {
		case 0:	if (c == group) status++; else status = 0;
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
		case 8: group++;
			status = 0;
	    }


	 if (group == 7) // Units location table + 1
	 {
// ===========================
// === Check unit sequence ===
// ===========================

	 switch (status1)
	   {
		case 0:	 if (c == 0x80) status1++; else status1 = 0;
			 break;
		case 4:  if (c == 0x80) status1++; else status1 = 0;
			 break;
		case 10: status1++;
			 if (c >= 1 && c <= 4)
			  {
// =============================
// === All levels down or up ===
// =============================

		if (argv[3][0] == '-' && (c - 1) >= 1 && (c - 1) <= 4) c--;
		if (argv[3][0] == '+' && (c + 1) >= 1 && (c + 1) <= 4) c++;
		cout << (int) c << " ";
			  }
			 break;
		case 69: status1 = 0;
			 break;
		default: status1++;
	   }
	 }

// =================================
// === Write to destination file ===
// =================================

   output_file. write(&c, 1);
   if (output_file. fail())
	{
	  cout << "\nDestination file can not be written. Terminating...\n";
	  input_file. close();
	  output_file. close();
	  exit(EXIT_FAILURE);
	}
     }
  }

// ============
// === Exit ===
// ============

   input_file. close();
   output_file. close();

   if (group > 6) cout << "Done!\n";
   else {
	  cout << "Can not convert - no changes made!\n";
	}
   exit(EXIT_SUCCESS);
}

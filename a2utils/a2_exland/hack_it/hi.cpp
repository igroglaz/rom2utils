/* ******************************************************************** **
** @@ A Quick Hack The Target File Using Crack Script
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 17 Jun 2000
** @  Update :
** @  Dscr   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include <windows.h>

#include <stdio.h>
#include <string.h>

/* ******************************************************************** **
** @@                   internal #defines
** ******************************************************************** */

typedef __int64   QWORD;               // For future :-)

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Abort()
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 08 Oct 1999
** @  Update :
** @  Dscr   :
** @  Proto  :
** @  Input  :
** @  Output :
** @  Notes  :
** ******************************************************************** */

void Abort()
{
   void* pMsgBuf;

   FormatMessage
   (
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM,               // source and processing options
      NULL,                                     // pointer to  message source
      GetLastError(),                           // requested message identifier
      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // language identifier for requested message
      (LPTSTR)&pMsgBuf,                         // pointer to message buffer
      0,                                        // maximum size of message buffer
      NULL                                      // address of array of message inserts
   );

   perror((const char *)pMsgBuf);

   LocalFree(pMsgBuf);

   exit(-1);
}

/* ******************************************************************** **
** @@ IsHex()
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 17 Jun 2000
** @  Update :
** @  Dscr   :
** @  Proto  :
** @  Input  :
** @  Output :
** @  Notes  :
** ******************************************************************** */

inline bool IsHex
(
   char  Chr
)
{
   return (((Chr >= '0') && (Chr <= '9') ||
            (Chr >= 'A') && (Chr <= 'F'))  ?  true  :  false);
}
            
/* ******************************************************************** **
** @@ GetParams()
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 17 Jun 2000
** @  Update :
** @  Dscr   :
** @  Proto  :
** @  Input  :
** @  Output :
** @  Notes  :
** ******************************************************************** */

void GetParams
(
   const char * const   pszBuf,
   DWORD&               dwOffset,
   DWORD&               dwSrc,
   DWORD&               dwDst
)
{
   char* pStart   = (char *)pszBuf;
   char* pCurrent = (char *)pszBuf;

   short    iNum = 0;

   while (*pCurrent)
   {
      while (*pCurrent && (IsHex(toupper(*pCurrent))))
         ++pCurrent;

      *pCurrent = 0;  // ASCIIZ

      switch (iNum)
      {
         case 0:
            sscanf(pStart,"%X",&dwOffset);
            break;
         case 1:
            sscanf(pStart,"%X",&dwSrc);
            break;
         case 2:
            sscanf(pStart,"%X",&dwDst);
            break;
         default :
            return;
      }

      ++iNum;
      ++pCurrent;

      while (*pCurrent && !(IsHex(*pCurrent)))
         ++pCurrent;

      pStart = pCurrent;
   }
}

/* ******************************************************************** **
** @@ Error()
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 17 Jun 2000
** @  Update :
** @  Dscr   :
** @  Proto  :
** @  Input  :
** @  Output :
** @  Notes  :
** ******************************************************************** */

void Error
(
   short    iLine,
   DWORD    dwOffset,
   BYTE     byDst,
   BYTE     byReally
)
{
   printf("\n\nError: Target Not Match Script. Aborted\n");
   printf("Line: %05d  Offset: %08X  Script: %02X  Target: %02X\n\n",iLine,dwOffset,byDst,byReally);

   exit(-1);
}

/* ******************************************************************** **
** @@ Error()
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 17 Jun 2000
** @  Update :
** @  Dscr   :
** @  Proto  :
** @  Input  :
** @  Output :
** @  Notes  :
** ******************************************************************** */

void Error
(
   short    iLine,
   DWORD    dwOffset
)
{
   printf("\n\nError: Offset Out of File. Aborted\n");
   printf("Line: %05d  Offset: %08X\n\n",iLine,dwOffset);

   exit(-1);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt : Aleph
** @  Author : Aleph
** @  Modify : 17 Jun 2000
** @  Update :
** @  Dscr   :
** @  Proto  :
** @  Input  :
** @  Output :
** @  Notes  :
** ******************************************************************** */

int main
(
   int      argc,
   char**   argv
)
{
   const char pszCopyright[] = "-*-   Hack It !"
                               "  *   Copyright (C) Aleph 2000   -*-";
   const char pszDescript [] = "A Quick Hack The Target File Using Crack Script";
   const char pszE_Mail   [] = "complains_n_suggestions direct to aleph@beer.com";

   if ((argc != 3) && (argc != 4)) {
      printf("\n\n%s\n",pszCopyright);
      printf("\n%s\n",  pszDescript);
      printf("\nUsage:  hi.com [-r] ScriptFile TargetFile\n");
      printf("\n%s\n\n",pszE_Mail);
      return (0);
   }

   bool  bReverse = false;

   if ((argc == 4) && (!strcmp(strupr(argv[1]),"-R")))
      bReverse = true;

   // Open Files

   FILE* hIn  = fopen(argv[argc - 2],"rt");

   if (!hIn)
   {
      perror("Script File Open Error");
      return (-1);
   }

   // Open Target File

   HANDLE Target_handle = CreateFile
   (
      argv[argc - 1],               // pointer to name of the file
      GENERIC_READ | GENERIC_WRITE, // access (read-write) mode
      0,                            // share mode
      NULL,                         // pointer to security attributes
      OPEN_EXISTING,                // how to create
      FILE_ATTRIBUTE_NORMAL,        // file attributes
      NULL                          // handle to file with attributes to copy
   );

   if (Target_handle == INVALID_HANDLE_VALUE)
      Abort();

   // Get File Size

   // Really FileSizeHi always ignored !

   DWORD dwFileSizeHi = 0;
   DWORD dwFileSizeLo = GetFileSize(Target_handle,&dwFileSizeHi);

   // QWORD qwFileSize = (((QWORD)dwFileSizeHi) << 32) + dwFileSizeLo;

   // Map to Memory

   HANDLE Target_map_handle = CreateFileMapping
   (
      Target_handle,    // handle to file to map
      NULL,             // optional security attributes
      PAGE_READWRITE,   // protection for mapping object
      dwFileSizeHi,     // high-order 32 bits of object size
      dwFileSizeLo,     // low-order 32 bits of object size
      NULL              // name of file-mapping object
   );

   if (!Target_map_handle)
      Abort();

   LPVOID pTarget_View = MapViewOfFile
   (
      Target_map_handle,   // file-mapping object to map into address space
      FILE_MAP_WRITE,      // access mode
      0,                   // high-order 32 bits of file offset
      0,                   // low-order 32 bits of file offset
      0                    // number of bytes to map
   );

   // Do Actual Work

   BYTE* pTarget = (BYTE *)pTarget_View;

   short    iLine = 0;

   while (!feof(hIn))
   {
      char     pszBuf[MAX_PATH + 1];

      DWORD    dwOffset = 0;
      DWORD    dwSrc    = 0;
      DWORD    dwDst    = 0;

      fgets(pszBuf,MAX_PATH,hIn);

      if (feof(hIn))
         break;

      ++iLine;

      if (*pszBuf != '0')       // No Script Line
         continue;

      GetParams(pszBuf,dwOffset,dwSrc,dwDst);

      if (dwOffset > dwFileSizeLo)
         Error(iLine,dwOffset);

      if (bReverse)
      {
         if (*(pTarget + dwOffset) == (BYTE)dwDst)
            *(pTarget + dwOffset) = (BYTE)dwSrc;
         else
            Error(iLine,dwOffset,(BYTE)dwDst,*(pTarget + dwOffset));
      }
      else
      {
         if (*(pTarget + dwOffset) == (BYTE)dwSrc)
            *(pTarget + dwOffset) = (BYTE)dwDst;
         else
            Error(iLine,dwOffset,(BYTE)dwSrc,*(pTarget + dwOffset));
      }
   }

   // Cleanup

   if (!UnmapViewOfFile(pTarget_View))
      Abort();

   if (!CloseHandle(Target_map_handle))
      Abort();

   if (!CloseHandle(Target_handle))
      Abort();

   return (0);
}

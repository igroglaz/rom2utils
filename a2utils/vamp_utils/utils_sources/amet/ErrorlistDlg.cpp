// ErrorlistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Changer.h"
#include "ErrorlistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char FileName[];
extern UINT maxX;
extern UINT maxY;
extern char* int2char(unsigned int, char*);

/////////////////////////////////////////////////////////////////////////////
// CErrorlistDlg dialog


CErrorlistDlg::CErrorlistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorlistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CErrorlistDlg)
	m_String = _T("");
	//}}AFX_DATA_INIT
}


void CErrorlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorlistDlg)
	DDX_Control(pDX, IDC_ERRORLIST, m_Errorlist);
	DDX_LBString(pDX, IDC_ERRORLIST, m_String);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorlistDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorlistDlg)
	ON_BN_CLICKED(ID_BUTTON_FIXALL, OnButtonFixall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorlistDlg message handlers

BOOL CErrorlistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	if (FileName[0] == '\0') 
	{
		MessageBox("No file selected");
		EndDialog(FALSE);
		return FALSE;
	}

	
	unsigned char ObjectCode[185] = {0x5c,0x5d,0x5e,0x16,0x17,0x18,0x4e,0x4f,0x50,0x51,0x52,0x07,0x09,0x31,0x32,0x33,0x34,0x62,0x63,0x64,0x94,0x95,0x96,0x53,0x54,0x56,0x57,0x58,0x5f,0x60,0x61,0x65,0x66,0x67,0x69,0x6b,0x81,0x83,0x85,0x87,0x6d,0x7F,0x6F,0x71,0x73,0x75,0x77,0x79,0x7b,0x7d,0x68,0x6a,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x90,0x91,0x92,0x93,0x0a,0x0c,0x35,0x36,0x37,0x38,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x0d,0x0f,0x39,0x3a,0x3b,0x3c,0x10,0x12,0x3d,0x3e,0x3f,0x40,0xac,0xad,0xae,0xaf,0xb0,0xb1,0x13,0x15,0x41,0x42,0x43,0x44,0x80,0x82,0x84,0x86,0x01,0x03,0x29,0x2a,0x2b,0x2c,0x1f,0x20,0x21,0x22,0x23,0x24,0x19,0x1a,0x1b,0x59,0x5a,0x5b,0x55,0x1c,0x1d,0x1e,0x45,0x46,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0x26,0x25,0xbe,0xbf,0xc0,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0x27,0x28,0x97,0x98,0x99,0x6c,0x7e,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0x04,0x06,0x2d,0x2e,0x2f,0x30};
	// HEX codes for objects on a map

	
//	We can open and read the file here
	
	CFile f;
	if (f.Open(FileName, CFile::modeRead) == FALSE)
	{
		MessageBox("File can't be open");
		EndDialog(FALSE);
		return FALSE;
	}

	unsigned long i, FileLength = (unsigned long) f.GetLength();	// Get length of file
	if (FileLength == 0)
	{
		f.Close();
		MessageBox("File has zero length");
		EndDialog(FALSE);
		return FALSE;
	}

	unsigned char *FileData;
	FileData = new unsigned char [FileLength];			// Allocate memory for a file data
	if (FileData == NULL)
	{
		f.Close();
		MessageBox("Not enough memory");
		EndDialog(FALSE);
		return FALSE;
	}

	if (f.Read(FileData, FileLength) != FileLength) // Read file to memory
	{
		f.Close();
		delete FileData;
		MessageBox("File can't be read");
		EndDialog(FALSE);
		return FALSE;
	}

	f.Close();

	
	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".al~");

	if (f.Open(BakFileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
	{
		delete FileData;
		MessageBox("Back-up file can't be written");
		EndDialog(FALSE);
		return FALSE;
	}

	f.Write(FileData, FileLength); // Write file from memory
	f.Close();


// =======================================
// === Main cycle for searching groups ===
// =======================================

	unsigned long StartGroup = 0, EndGroup = 0;

    for (i = 0x28; i < FileLength - 7; i++)
    {
	
// We must check group ID. It looks like this: 03 00 00 00 6D 2B F6 BF

		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

		if (FileData[i] == 3)	// Group number of Objects (Trees) allocation table
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			StartGroup = i + 8;
			break;
		}

	}

	if (StartGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		EndDialog(FALSE);
		return FALSE; // Group not found
	}


	for (i = StartGroup + 16; i < FileLength - 7; i++)
    {
		if (FileData[i-12] == FileData[0x14])
		if (FileData[i-11] == FileData[0x15])
		if (FileData[i-10] == FileData[0x16])
		if (FileData[i-9] == FileData[0x17])
		if (FileData[i-8] == FileData[0x18])
		if (FileData[i-7] == FileData[0x19])
		if (FileData[i-6] == FileData[0x1A])
		if (FileData[i-5] == FileData[0x1B])

//		if (FileData[i] == 3)
		if (FileData[i+1] == 0)
		if (FileData[i+2] == 0)
		if (FileData[i+3] == 0)
		if (FileData[i+4] == FileData[0x24])
		if (FileData[i+5] == FileData[0x25])
		if (FileData[i+6] == FileData[0x26])
		if (FileData[i+7] == FileData[0x27])
		{
			EndGroup = i - 12;
			break;
		}
	}
	
	if (EndGroup == 0) 
	{
		delete FileData;
		MessageBox("File is damaged or not a map");
		EndDialog(FALSE);
		return FALSE; // Group not found
	}

// ======================================
// === Perform Objects verify process ===
// ======================================
	
	UINT X, Y, ErrorsNumber = 0;
	char String[4]; // String for conversion
	char OutString[50]; // String for output
	
	for (Y = 0, X = 0, i = StartGroup; i < EndGroup; i++, X++)
	{
		char Verified = FALSE;
		if (X >= maxX) 
		{
			X = 0;
			Y++;
		}

				
		if (FileData[i] == 0) Verified = TRUE; // Check for empty places (tiles)
		else for (int j = 0; j < 185; j++)
		{
			if (FileData[i] == ObjectCode[j])  // Check for correct objects
			{
				Verified = TRUE;
				break;
			}
		}

		if (FileData[i] != 0 && (X < 8 || Y < 8 || (X > maxX - 9) || (Y > maxY - 5))) // If error exists here, it is outside map range
			{
				FileData[i] = 0;
				if (Verified == FALSE) 
					strcpy(OutString, "Incorrect object out of map range at (");			//
				else
					strcpy(OutString, "Object out of map range at (");		//
				strcat(OutString, int2char(X, String));					//
				strcat(OutString, ", ");								//
				strcat(OutString, int2char(Y, String));					//
				strcat(OutString, ")");									// Report tile number
				m_String = OutString;

				m_Errorlist.AddString(m_String);
				ErrorsNumber++;
			}
		else if (Verified == FALSE) 
			{
				FileData[i] = 0;										//
				strcpy(OutString, "CRITICAL ERROR! Incorrect Object found at (");		//
				strcat(OutString, int2char(X, String));					//
				strcat(OutString, ", ");								//
				strcat(OutString, int2char(Y, String));					//
				strcat(OutString, ")");									// Report tile number
				m_String = OutString;

				m_Errorlist.AddString(m_String);
				ErrorsNumber++;
			}
	}
	

	if (ErrorsNumber > 0)
	{

	// Write file
		if (f.Open(FileName, CFile::modeCreate | CFile::modeWrite) == FALSE) 
		{
			delete FileData;
			MessageBox("File can't be written");
			EndDialog(FALSE);
			return FALSE;
		}

		f.Write(FileData, FileLength);	// Write file from memory
		f.Close();
	
		m_Errorlist.SetSel(0, TRUE);
//		MessageBox("Check complete!");
	}
	else 
	{
		CFile::Remove(BakFileName);	// Delete *.bak file
//		MessageBox("No errors found!");
		delete FileData;
		EndDialog(TRUE);
		return TRUE;
	}

	delete FileData; // Free allocated memory for file data	
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



///////////////////////////////////////////////////////////////////////////////////
void CErrorlistDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//	We can open and restore the file here
	CFileStatus rStatus; // No use for that, just to check file existance

	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".al~");

	if (CFile::GetStatus(BakFileName, rStatus))
	{
		CFile::Remove(FileName);
		CFile::Rename(BakFileName, FileName);
//		MessageBox("Backup copy restored!");		
	}
	else MessageBox("Backup copy is damaged!");

	CDialog::OnCancel();
}


void CErrorlistDlg::OnButtonFixall() 
{
	// TODO: Add your control notification handler code here
	CFileStatus rStatus; // No use for that, just to check file existance

	char BakFileName[500];
	strcpy(BakFileName, FileName);
	strcpy(strchr(BakFileName, '.'), ".al~");

	if (CFile::GetStatus(BakFileName, rStatus))	CFile::Remove(BakFileName);
	//
	CDialog::OnCancel();
}

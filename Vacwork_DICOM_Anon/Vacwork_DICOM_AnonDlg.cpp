// Vacwork_DICOM_AnonDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Vacwork_DICOM_Anon.h"
#include "Vacwork_DICOM_AnonDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CVacworkDICOMAnonDlg dialog

CVacworkDICOMAnonDlg::CVacworkDICOMAnonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VACWORK_DICOM_ANON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVacworkDICOMAnonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SOURCESIZE, m_size);
	DDX_Text(pDX, IDC_FREEBYTES, m_freeBytes);
	DDX_Text(pDX, IDC_USEDBYTES, m_usedBytes);
	DDX_Text(pDX, IDC_CAP, m_capacity);
	DDX_Text(pDX, IDC_NUMDCMFILES, m_dcmFiles);
	DDX_Text(pDX, IDC_FILESDONE, m_dcmFilesComplt);
	DDX_Text(pDX, IDC_PROGRESS1, m_progress);

}

BEGIN_MESSAGE_MAP(CVacworkDICOMAnonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CVacworkDICOMAnonDlg::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE2, &CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse2)
	ON_STN_CLICKED(IDC_FILESDONE, &CVacworkDICOMAnonDlg::OnStnClickedFilesdone)
	ON_BN_CLICKED(IDC_BUTTON3, &CVacworkDICOMAnonDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

// CVacworkDICOMAnonDlg message handlers

BOOL CVacworkDICOMAnonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog(); 
	DriveAttributes(m_outputDestination);
	UpdateData(FALSE);
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// enable close and minimize buttons
	//close box
	//EnableMenuItem(GetSystemMenu(FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
	
	//minimize box
	//SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) | WS_MINIMIZEBOX);


	//Registry data retrival 
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\WOW6432Node\\LODOX\\DBSERVER"), NULL, KEY_READ, &hKey) != ERROR_SUCCESS) //Opening Registry Key
	{
		return FALSE; //Cannot open key
	}
	PCReturn = RegQueryValueEx(hKey, _T("ComputerName"), NULL, &type, (LPBYTE)&cbDataPC, &sizePC);
	pathReturn = RegQueryValueEx(hKey, _T("ShareName"), NULL, &type, (LPBYTE)&cbDataPath, &sizePath);
	RegCloseKey(hKey);

	// TODO: Add extra initialization here
	CString m_sDataPath;

	if (strlen(cbDataPC) > 0)
	{
		m_sDataPath.Format(_T("\\\\%s\\"), cbDataPC);
	}

	if (strlen(cbDataPath) > 0)
	{
		m_sDataPath.AppendFormat(_T("%s"), cbDataPath);
	}

	m_sourceDestination = m_sDataPath;
	m_size= 0;
	CalculateSize(m_sDataPath);
	//call on image moving function

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVacworkDICOMAnonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVacworkDICOMAnonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVacworkDICOMAnonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVacworkDICOMAnonDlg::OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  Add your control notification handler code here

	CFolderPickerDialog m_dlg;
	//CString m_Folder;

	m_dlg.m_ofn.lpstrTitle = _T("Source Folder");
	m_dlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if (m_dlg.DoModal() == IDOK) {
		m_sourceDestination = m_dlg.GetPathName();   // Use this to get the selected folder name 								  // after the dialog has closed
		DriveAttributes(m_sourceDestination);// recalculate disk info for source folder selected
		UpdateData(FALSE);   // To show updated folder in GUI
	}
}

void CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse2()
{
	// TODO:  Add your control notification handler code here
	CFolderPickerDialog m_dlg;
	//CString m_Folder;

	m_dlg.m_ofn.lpstrTitle = _T("Destination Folder");
	m_dlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if (m_dlg.DoModal() == IDOK) {
		m_outputDestination = m_dlg.GetPathName();   // Use this to get the selected folder name 								  // after the dialog has closed
		DriveAttributes(m_outputDestination);// recalculate disk info for source folder selected
		UpdateData(TRUE);   // To show updated folder in GUI
	}

}
// Write own functions here

//calc disk space
void CVacworkDICOMAnonDlg::DriveAttributes(CString DirName)
{
	
	GetDiskFreeSpaceEx(DirName, (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);

	m_freeBytes = lpFreeBytesAvailableToCaller / 1000000000;
	m_capacity = lpTotalNumberOfBytes / 1000000000;
	m_usedBytes = m_capacity - m_freeBytes;

}
//find size of files
void CVacworkDICOMAnonDlg::CalculateSize(CString DirName) {

	WIN32_FIND_DATAA data;
	HANDLE sh = FindFirstFileA((DirName + "\\*"), &data);

	if (sh == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		// skip current and parent
		if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0)
		{
			// if found object is ...
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
				// directory, then search it recursievly
				this->CalculateSize(DirName + _T("\\") + (LPCTSTR)data.cFileName);
			}
			else
			{
				// otherwise get object size and add it to directory size
				this->m_size += (__int64)(data.nFileSizeHigh * (MAXDWORD)+data.nFileSizeLow);
			}
		}

	} while (FindNextFileA(sh, &data)); // do
	FindClose(sh);
	float a = 1000.00;
	m_size = (float)m_size / a; // MB
	//m_sizeDisp.Format(_T("%.2f"), m_size);

	UpdateData(FALSE);
}

// invalid dir handler

//no space handler
void CVacworkDICOMAnonDlg::OnStnClickedFilesdone()
{
	// TODO: Add your control notification handler code here
}

void CVacworkDICOMAnonDlg::MoveFiles() {
	//info about path
	// get address of .raw image in source directory
	std::ifstream sourceFile("C:\\Source Folder\\STN911_Uncorr_201977_15h56_7168x1920.raw", std::ifstream::binary);
	std::ofstream destFile("C:\\Destination Folder\\NewFile.raw", std::ofstream::binary);
	
	//get size of file
	sourceFile.seekg(0, sourceFile.end);
	std::streamsize size = sourceFile.tellg();
	sourceFile.seekg(0);

	// allocate memory for file content
	char* buffer = new char[(long)fileSize];
	// read content of infile
	sourceFile.read(buffer, fileSize);
	// write to outfile
	destFile.write(buffer, fileSize);

	// release dynamically-allocated memory
	delete[] buffer;

	sourceFile.close();
	destFile.close();
	
}
/*
void CVacworkDICOMAnonDlg::Convert() {
	int retcode = DbInitLib(_T(""));
	CString errorMessage = "Database not initialized.";
	if (retcode != DB_OK)
	{
		AfxMessageBox(errorMessage);
		return;
	}
	else
	{


	}
}*/

void CVacworkDICOMAnonDlg::OnBnClickedButton3()
{//RUN BUTTON
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//if (lpFreeBytesAvailable < m_dirSize) //1.0 GB
	//{
	//	AfxMessageBox("Not enough disk space available..");
	//	return;
	//}
	//else{
		MoveFiles();
		//m_progressCount = 100;
		//m_progress = m_progressCount;
		//SetPos(m_progressCount);
		//AfxMessageBox("Complete");
		//m_progressCount = RESET;
		//m_progress.SetPos(m_progressCount);
		//m_numDCM = RESET;
	//}
		UpdateData(TRUE);
	OnInitDialog();

}

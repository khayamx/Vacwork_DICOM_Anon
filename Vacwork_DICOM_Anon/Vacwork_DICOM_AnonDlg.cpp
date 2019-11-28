
// Vacwork_DICOM_AnonDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Vacwork_DICOM_Anon.h"
#include "Vacwork_DICOM_AnonDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// create variables for testing
/*int m_size = 150;
int  m_freeBytes = 281;
int m_usedBytes = 140;
int m_capacity = 230;
int m_dcmFiles = 4;
int m_dcmFilesComplt = 2;*/


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
	DDX_Text(pDX, IDC_SOURCESIZE, m_sizeDisp);
	DDX_Text(pDX, IDC_FREEBYTES, m_freeBytes);
	DDX_Text(pDX, IDC_USEDBYTES, m_usedBytes);
	DDX_Text(pDX, IDC_CAP, m_capacity);
	DDX_Text(pDX, IDC_NUMDCMFILES, m_dcmFiles);
	DDX_Text(pDX, IDC_FILESDONE, m_dcmFilesComplt);
}


BEGIN_MESSAGE_MAP(CVacworkDICOMAnonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CVacworkDICOMAnonDlg::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE2, &CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse2)
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

	// TODO: Add extra initialization here

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
	CString m_Folder;

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
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
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
	m_size = m_size / 1000000; // MB
	m_sizeDisp.Format(_T("%.2f"), m_size);

	UpdateData(FALSE);
}

// invalid dir handler

//no space handler

//

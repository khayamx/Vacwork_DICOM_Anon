// Vacwork_DICOM_AnonDlg.cpp : implementation file

#include "pch.h"
#include "framework.h"
#include "Vacwork_DICOM_Anon.h"
#include "Vacwork_DICOM_AnonDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <string>


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
	DDX_Text(pDX, IDC_NUMDCMFILES, m_Files);
	DDX_Text(pDX, IDC_FILESDONE, m_FilesComplete);
	DDX_Text(pDX, IDC_PROGRESSCOUNT, m_progressCount);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_sourceDestination);
	DDX_Text(pDX, IDC_MFCEDITBROWSE2, m_outputDestination);
}

BEGIN_MESSAGE_MAP(CVacworkDICOMAnonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CVacworkDICOMAnonDlg::OnNMCustomdrawProgress1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE2, &CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse2)
	ON_BN_CLICKED(IDC_RUN, &CVacworkDICOMAnonDlg::OnBnClickedRUN)
END_MESSAGE_MAP()

//CVacworkDICOMAnonDlg message handlers

BOOL CVacworkDICOMAnonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog(); 
	m_size =0;
	
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

	DriveAttributes(_T("C://"));
	UpdateData(FALSE);
	
	//SourceList(m_sDataPath + _T("\\Images\\"));

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
	m_progress.SetRange(0, 100);
	//m_progress.SetPos(75);
}


void CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse1()
{
	UpdateData(TRUE);
	CalculateSize(m_sourceDestination);
	SourceList(m_sourceDestination);
	UpdateData(TRUE);
}

void CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse2()
{

	UpdateData(TRUE);
	CString destPath = m_outputDestination;

	UpdateData(TRUE);
}
// Write own functions here

//calc disk space
void CVacworkDICOMAnonDlg::DriveAttributes(CString DirName)
{
	lpFreeBytesAvailableToCaller = lpTotalNumberOfBytes = lpTotalNumberOfFreeBytes = 0;
	GetDiskFreeSpaceEx(DirName, (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);

	m_freeBytes = lpFreeBytesAvailableToCaller / 1000000000;
	m_capacity = lpTotalNumberOfBytes / 1000000000;
	m_usedBytes = m_capacity - m_freeBytes;
	UpdateData(FALSE);
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
	UpdateData(FALSE);
}

// LIST
BOOL CVacworkDICOMAnonDlg::SourceList(CString DirName) {
	if (!::SetCurrentDirectory(DirName))
	{
		return FALSE;
	}

	WIN32_FIND_DATA findData;
	//HANDLE hFind = ::FindFirstFile(_T("*.dcm"), &findData);
	HANDLE hFind = ::FindFirstFile(_T("*.raw"), &findData);
	int count = 0;
	while (hFind != INVALID_HANDLE_VALUE)
	{
		CString fileName = findData.cFileName;
		mylist.push_back(fileName);
		count++;

		if (!::FindNextFile(hFind, &findData))
		{
			::FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;

			break;
		}
	}
	m_Files = count;
	UpdateData(FALSE);
}

void CVacworkDICOMAnonDlg::moveFile(CString inputFName) {//input parameter needs to be a constacnt char w/ souce file name
	//need to refer to input file name of source file name of source path
	//using namespace ATL;
	CString sourcePath = m_sourceDestination + "\\";
	CString destinationPath = m_outputDestination + "\\";
	//add the two
	//sourcePath = m_sourceDestination + "\\";
	sourcePath += (inputFName);
	CT2A pszConvertedAnsiString1(sourcePath);
	std::string strInputDest(pszConvertedAnsiString1);
	//refer to input file
	//std::ifstream sourceFile("C:\\Source Folder\\STN911_Uncorr_201977_15h56_7168x1920.raw", std::ifstream::binary);
	std::ifstream sourceFile(strInputDest, std::ifstream::binary);


	destinationPath += (inputFName);
	CT2A pszConvertedAnsiString2(destinationPath);
	std::string strOutputDest(pszConvertedAnsiString2);
    // create destination file
	std::ofstream destFile(strOutputDest, std::ofstream::binary);
	
	//get size of file
	sourceFile.seekg(0, sourceFile.end);
	std::streamsize size = sourceFile.tellg();
	sourceFile.seekg(0);


	m_progress.SetPos(m_progressCount);
	// allocate memory for file content
	char* buffer = new char[fileSize];

	// read content of sourceFile
	sourceFile.read(buffer, fileSize);
	
	// write to destFile
	destFile.write(buffer, fileSize);

	//clean up
	// release dynamically-allocated memory 
	delete[] buffer;
	sourceFile.close();
	destFile.close();
}

void CVacworkDICOMAnonDlg::StepThroughFiles() {
	//USE NUMBER OF DEICOM FILES FOUND AS COUNTER
	int i;
	for (i = 0; i < m_Files; i++) {
		//make i number of copies of the file
		moveFile(mylist[i]);
		m_progressCount = (i/m_Files)*100;
		m_progress.SetPos(m_progressCount);
		m_FilesComplete = i;
		UpdateData(FALSE);
		//name them the same as OG files
	}
}

void CVacworkDICOMAnonDlg::OnBnClickedRUN()
{//RUN BUTTON
	UpdateData(TRUE);
	//MoveFile(fileName);
	StepThroughFiles();
	m_progressCount = 100;
	m_progress.SetPos(m_progressCount);
	UpdateData(FALSE);
}
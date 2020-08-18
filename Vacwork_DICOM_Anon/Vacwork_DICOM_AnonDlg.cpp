// Vacwork_DICOM_AnonDlg.cpp : implementation file
//#define _CRTDBG_MAP_ALLOC


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
#include <vector>
#include <crtdbg.h>
//#include "DCMhandler.h"


//#include "DicomImporter.h"
//#include "DicomExporter.h"




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

//CVacworkDICOMAnonDlg::CVacworkDICOMAnonDlg(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_VACWORK_DICOM_ANON_DIALOG, pParent)
//{ 
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//}

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
	DDX_Control(pDX, IDC_LIST, m_Status); 
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
	UpdateStatus(_T("opening DICOM anonymizer"));
	UpdateStatus(_T("please select a source directory"));
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
	
	//DISABLE RUN and directory browse buttons
	GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCEDITBROWSE2)->EnableWindow(FALSE);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	DriveAttributes(_T("C://"));
	m_progress.SetPos(0);
	UpdateData(FALSE);

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
	GetDlgItem(IDC_MFCEDITBROWSE2)->EnableWindow(TRUE);
	UpdateData(TRUE);
	UpdateStatus(_T(" source directory selected please select a destination directory"));
}


void CVacworkDICOMAnonDlg::OnEnChangeMfceditbrowse2()
{

	UpdateData(TRUE);
	CString destPath = m_outputDestination;
	GetDlgItem(IDC_RUN)->EnableWindow(TRUE);
	UpdateStatus(_T("click RUN to anonymize"));

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
	HANDLE hFind = ::FindFirstFile(_T("*.dcm"), &findData);   //find DCM file
	//HANDLE hFind = ::FindFirstFile(_T("*.raw"), &findData);  //find raw file
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



void CVacworkDICOMAnonDlg::moveFile2(CString inputFName) {
	//DICOM compatible move files function
	UpdateStatus(inputFName);
	CString destinationPath = m_outputDestination + "\\";
	destinationPath += (inputFName);

	//DCM.Convert(inputFName, destinationPath);
	importer.Import(inputFName);
	exporter.Export(inputFName, importer.m_image.data(), destinationPath, importer.nImageRows, importer.nImageCols);
}

void CVacworkDICOMAnonDlg::StepThroughFiles() {
	//USE NUMBER OF DEICOM FILES FOUND AS COUNTER
	
	for	(std::vector<CString>::iterator i = mylist.begin(); i != mylist.end(); ++i)
	{
		//make i number of copies of the file
		CString DCMFile = *i;
		moveFile2(DCMFile);
		m_FilesComplete++;
		m_progressCount = (m_FilesComplete  * 100)/m_Files;
		m_progress.SetPos(m_progressCount);
		
		CleanUp();
		UpdateData(FALSE);
		//name them the same as OG files
	}
	////_CrtDumpMemoryLeaks();
	//return;
}

void CVacworkDICOMAnonDlg::OnBnClickedRUN()
{//RUN BUTTON
	//UpdateData(TRUE);
	//MoveFile(fileName);
	
	StepThroughFiles();
	m_progressCount = 100;
	m_progress.SetPos(m_progressCount);

	//RESET
	m_progressCount = 100;

	m_progress.SetPos(m_progressCount);

	AfxMessageBox(_T("Complete. Click OK."));

	m_progressCount = 0;
	m_progress.SetPos(m_progressCount);
	m_Files = m_FilesComplete = 0;

	m_sourceDestination = m_outputDestination = _T("");

	GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCEDITBROWSE2)->EnableWindow(FALSE);

	UpdateStatus(_T("Anonimyzation complete. Please select new source directory to anonymize."));
	UpdateData(FALSE);
}

void CVacworkDICOMAnonDlg::CleanUp()
{
	//dcmDataDict.clear();   no honey
	//delete DSet;

	importer.m_image.resize(0);
	std::vector<unsigned short> vector_temp;
	vector_temp.swap(importer.m_image);

	vector_temp.clear();
	vector_temp.shrink_to_fit();
}

void CVacworkDICOMAnonDlg::UpdateStatus(CString message)
{
	// TODO: Add your control notification handler code here
	GetLocalTime(&st);
	StatusMessage.Format((_T("% 02d:% 02d: %02d - ")+ message), st.wHour, st.wMinute, st.wSecond);

	m_Status.SetHorizontalExtent(400);

	if (m_Status.GetCount() > 400)
	{
		m_Status.DeleteString(0);
	}

	m_Status.InsertString(m_Status.GetCount(), (LPCTSTR)StatusMessage);
	m_Status.SetCurSel(m_Status.GetCount() - 1);

	UpdateData(FALSE);
}

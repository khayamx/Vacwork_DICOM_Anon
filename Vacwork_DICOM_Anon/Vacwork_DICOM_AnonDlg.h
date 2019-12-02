
// Vacwork_DICOM_AnonDlg.h : header file
//

#pragma once


// CVacworkDICOMAnonDlg dialog
class CVacworkDICOMAnonDlg : public CDialogEx
{
// Construction
public:
	CVacworkDICOMAnonDlg(CWnd* pParent = nullptr);	// standard constructor
	//insert functions I have crreated
	//void fc(type paramName);
	void DriveAttributes(CString DirName);
	void CalculateSize(CString DirName);
	//void Convert();
	//void MoveFiles(CString sourceDir, CString destDir);
	void MoveFiles();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VACWORK_DICOM_ANON_DIALOG };
#endif

protected:

		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMCustomdrawProgress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnEnChangeMfceditbrowse2();

private:
	CString m_sourceDestination;
	CString m_outputDestination;
	
	//For registry path on machine
	char cbDataPC[MAX_PATH];
	char cbDataPath[MAX_PATH];
	unsigned long sizePC = 1024;
	unsigned long sizePath = 1024;
	DWORD type = REG_SZ;
	LSTATUS PCReturn, pathReturn;


	//variables for calculating Drive Info
	char(*DirName);
	__int64 lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;

	int m_size;
	CString m_sizeDisp;

	int  m_freeBytes;
	int m_usedBytes;
	int m_capacity;

	//variables for reading files
	char* buffer = nullptr; //pointer definition pointing to address in memory of pointer 
	int fileSize = 55050240;
	//int pBuffer = newchar[fileSize];
	int width = 7168;
	int height = 1920;
	BYTE readBuffer[7168][1920];
	BYTE writeBuffer[7168][1920];
	// name of .raw file    'STN911_Uncorr_201977_15h56_7168x1920'
	const char* fileName = "STN911_Uncorr_201977_15h56_7168x1920.raw";

	//progress counter variables
	//1 pixel is one unit of processing = 4 bytes/32 bits

	int posInFile = 0;
	int array = fileSize / 4;

	int m_progress=0;
	int m_progressCount = 0;
	int m_dcmFiles = 4;
	int m_dcmFilesComplt = 2;


public:
	afx_msg void OnStnClickedFilesdone();
	afx_msg void OnBnClickedButton3();
};

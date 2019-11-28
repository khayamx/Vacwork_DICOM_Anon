
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
	int m_dcmFiles = 4;
	int m_dcmFilesComplt = 2;
public:
	afx_msg void OnStnClickedFilesdone();
};

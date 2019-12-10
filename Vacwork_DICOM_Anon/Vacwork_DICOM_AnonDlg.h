
// Vacwork_DICOM_AnonDlg.h : header file
//

#pragma once
#include <iostream>
#include <vector>
#include "DicomImporter.h"
#include "DicomExporter.h"


// CVacworkDICOMAnonDlg dialog
class CVacworkDICOMAnonDlg : public CDialogEx
{
	
// Construction
public:
	CVacworkDICOMAnonDlg(CWnd* pParent = nullptr);	// standard constructor
	//insert functions I have created
	void DriveAttributes(CString DirName);
	void CalculateSize(CString DirName);
	//void MoveFiles();
	void moveFile(CString inputFName);
	void moveFile2(CString inputFName);
	BOOL SourceList(CString DirName);
	void StepThroughFiles();


	std::vector<CString> mylist;
	//std::vector<CString> NewNames = {"File1.","",""};

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

	CProgressCtrl m_progress;
	int m_progressCount = 0;

	//For registry path on machine
	unsigned long sizePC = 1024;
	unsigned long sizePath = 1024;
	DWORD type = REG_SZ;
	LSTATUS PCReturn, pathReturn;
	//

	//variables for calculating Drive Info
	char(*DirName);
	__int64 lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;

	int m_size;
	int m_freeBytes;
	int m_usedBytes;
	int m_capacity;

	//variables for reading files
	int fileSize = 55050240;
	

	int m_Files;
	int m_FilesComplete;

	/*CDicomImporter importer;
	CDicomExporter exporter;*/
	UINT Rrows, Rcols;


public:
	afx_msg void OnBnClickedRUN();
	};

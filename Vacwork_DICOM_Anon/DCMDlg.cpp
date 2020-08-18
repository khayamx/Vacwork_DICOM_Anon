#include "pch.h"
#include "DCMDlg.h"

// DCMDlg.cpp : implementation file
//
//
//#include "stdafx.h"
//#include "DBR2DICOM.h"
#//include "App.h"
#include "afxdialogex.h"
#include "Vacwork_DICOM_Anon.h"

// DCMDlg dialog

IMPLEMENT_DYNAMIC(DCMDlg, CDialogEx)

DCMDlg::DCMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

DCMDlg::~DCMDlg()
{
}

void DCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_MISSINGDBR, m_missing_sDCM);
}

BEGIN_MESSAGE_MAP(DCMDlg, CDialogEx)

END_MESSAGE_MAP()


// DCMDlg message handlers
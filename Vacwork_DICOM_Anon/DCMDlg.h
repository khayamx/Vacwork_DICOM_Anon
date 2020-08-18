#pragma once
class DCMDlg
{

	DECLARE_DYNAMIC(DCMDlg)

public:
	int m_missing_sDCM = 0;
	DCMDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DCMDlg();

	DECLARE_MESSAGE_MAP()

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
};


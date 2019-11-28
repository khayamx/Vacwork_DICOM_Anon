
// Vacwork_DICOM_Anon.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVacworkDICOMAnonApp:
// See Vacwork_DICOM_Anon.cpp for the implementation of this class
//

class CVacworkDICOMAnonApp : public CWinApp
{
public:
	CVacworkDICOMAnonApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVacworkDICOMAnonApp theApp;

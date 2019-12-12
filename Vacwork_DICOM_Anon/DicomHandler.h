#pragma once
///////////////////////////////////////////////////////////////////////////////
// Header File Includes and Forward Declarations
///////////////////////////////////////////////////////////////////////////////

#include "dcmtk\dcmdata\dctk.h"
#include <vector>

class CDicomHandler

{
public:
	BOOL Convert(CString FName, std::string outputDest);
	
		
	CString File;

private:
	BOOL GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);


};
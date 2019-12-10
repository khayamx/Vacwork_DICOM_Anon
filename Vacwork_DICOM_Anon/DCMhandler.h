#pragma once

#include "dcmtk\dcmdata\dctk.h"
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <vector>

class DCMhandler
{
public:
	DCMhandler();
	~DCMhandler();
	BOOL Convert(CString FName, std::string destPath);

private:
	CString FileName;
	DcmDataset* ds;
	std::vector<unsigned short> m_image;

	BOOL GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);
	BOOL Export(CString fName, std::string destPath);
	UINT nImageCols = 0, nImageRows = 0;

};


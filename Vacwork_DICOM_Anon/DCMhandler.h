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
	DcmFileFormat* oldFile;//already saved in Convert function
	DcmDataset* DSet;//already saved in Convert function
	OFCondition cond;//already saved in Convert function
	std::vector<unsigned short> m_image;

	//DcmFileFormat* newFile;
	//DcmDataset* newDSet;
	//OFCondition OFret;

	BOOL GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);
	BOOL Export(CString fName, std::string destPath);
	UINT nImageCols = 0, nImageRows = 0;

	Uint16 NumberOfFrames;
	Uint16 SamplesPerPixel;
	Uint16 bitsStored;
};


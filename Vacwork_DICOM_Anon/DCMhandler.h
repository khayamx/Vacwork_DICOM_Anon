#pragma once

#include "dcmtk\dcmdata\dctk.h"
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <vector>

class DCMhandler
{
public:
	//DCMhandler();
	//~DCMhandler();
	//BOOL Convert(CString FName, CString destPath);
	//std::vector<unsigned short> m_image;

private:
	//CString FileName;
	//DcmFileFormat* oldFile;//already saved in Convert function
	//DcmDataset* DSet;//already saved in Convert function
	//OFCondition cond;//already saved in Convert function
	/*

	USHORT* ImagePointer;

	BOOL GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);
	BOOL Export(CString destPath, USHORT* pData);
	void CleanUp();

	UINT nImageCols = 0, nImageRows = 0;

	Uint16 NumberOfFrames;
	Uint16 SamplesPerPixel;
	Uint16 bitsStored;
	int fileSize;*/
};

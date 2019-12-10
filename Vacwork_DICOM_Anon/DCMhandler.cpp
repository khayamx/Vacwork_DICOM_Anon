#include "pch.h"
//#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "DCMhandler.h"

using namespace std;


DCMhandler::DCMhandler()
{
	//FileName = FName;
	//m_image = new vector<unsigned short>;

}


DCMhandler::~DCMhandler()
{
	ds = nullptr;

	m_image.clear();
	nImageCols = nImageRows = 0;
	int x;
	//maybe clear the dataset
}


BOOL DCMhandler::Convert(CString FName, std::string destPath) {
	//LOAD FILE
	CT2A strFileName(FName);

	DcmFileFormat file;
	OFCondition cond = file.loadFile(strFileName);

	if (!cond.good())
	{
		AfxMessageBox(_T("Error reading file!"), MB_OK || MB_APPLMODAL || MB_ICONEXCLAMATION, 0);
		return FALSE;
	}

	ds = file.getDataset();
	//maybe use getLength method
	if (ds == NULL)
	{
		return FALSE;
	}

	// --------------------------------------------------------------------
	// Modality Check (DX of CR)
	// --------------------------------------------------------------------

	const char* value = "";

	cond = ds->findAndGetString(DCM_Modality, value);
	if (cond.good())
	{
		if (strcmp(value, "CR"))  // I don't know why but I appear to *have* to do it this way
		{
			if (strcmp(value, "DX"))
			{
				//AfxMessageBox(_T("Only CR or DX images can be imported!"));
				return FALSE;
			}
		}
	}

	GetImagePixelDataFromDataset(nImageCols, nImageRows);

	if (GetImagePixelDataFromDataset(nImageCols, nImageRows) == FALSE)
	{
		AfxMessageBox(_T("ERROR: Unable to get image pixel data from DICOM."));
		return FALSE;
	}


	//EXPORT FILE
	Export(FName, destPath);
	if (Export(FName, destPath)) {
		return TRUE;

	}

	return TRUE;
}

BOOL DCMhandler::GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows) {
	//ASSERT(ds != NULL);
	if (ds == NULL)
	{
		return FALSE;
	}

	// ------------------------------------------------------------------------
	//
	// ------------------------------------------------------------------------

	OFCondition cond;

	Uint16 rows;
	cond = ds->findAndGetUint16(DCM_Rows, rows);
	nRows = rows;

	Uint16 cols;
	cond = ds->findAndGetUint16(DCM_Columns, cols);
	nCols = cols;

	Uint16 bitsAllocated;
	cond = ds->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);

	if (rows == 0 || cols == 0 || bitsAllocated == 0)
	{
		return FALSE;
	}

	// ------------------------------------------------------------------------
	//
	// ------------------------------------------------------------------------

	(m_image).resize(rows * cols);

	if (bitsAllocated == 8)
	{
		const Uint8* value = NULL;
		ds->findAndGetUint8Array(DCM_PixelData, value);

		if (value != NULL)
		{
			for (int x = 0; x < cols * rows; x++)
			{
				(m_image)[x] = value[x];
			}
		}
		else
		{
			return FALSE;
		}
		value = NULL;


	}
	else if (bitsAllocated == 16)
	{
		const Uint16* value = NULL;
		ds->findAndGetUint16Array(DCM_PixelData, value);

		if (value != NULL)
		{
			memcpy(m_image.data(), value, rows * cols * sizeof(unsigned short));
		}
		else
		{
			return FALSE;
		}
		value = NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
	
}


BOOL DCMhandler::Export(CString fName, std::string destPath) {
	DcmFileFormat* dFile = new DcmFileFormat();

	DcmDataset *DSet = dFile->getDataset();

	OFCondition OFret;

	OFret = DSet->putAndInsertString(DCM_PatientName, "NameSurname");
	OFret = DSet->putAndInsertString(DCM_PatientID, "123467890"/* + simagecount*/);
	OFret = DSet->putAndInsertString(DCM_PatientBirthDate, "00-00-0000");
	OFret = DSet->putAndInsertString(DCM_PatientSex, "X");
	OFret = DSet->putAndInsertString(DCM_StudyInstanceUID, "1234567890" /*+ simagecount*/);
	OFret = DSet->putAndInsertString(DCM_StudyDate, "00-00-0000");
	OFret = DSet->putAndInsertString(DCM_StudyTime, "00:00:00");
	OFret = DSet->putAndInsertString(DCM_ReferringPhysicianName, "Physician");
	OFret = DSet->putAndInsertString(DCM_StudyID, "1234567890"/* + simagecount*/);
	OFret = DSet->putAndInsertString(DCM_AccessionNumber, "1234567890" /*+ simagecount*/);
	OFret = DSet->putAndInsertString(DCM_SeriesInstanceUID, "1234567890" /*+ simagecount*/);
	OFret = DSet->putAndInsertString(DCM_SeriesNumber, "1234567890" /*+ simagecount*/);

	Uint16 Uint16val = 1;
	OFret = DSet->putAndInsertUint16(DCM_SamplesPerPixel, Uint16val);
	OFret = DSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");

	OFret = DSet->putAndInsertUint16(DCM_Rows, nImageRows);
	OFret = DSet->putAndInsertUint16(DCM_Columns,nImageCols);
	OFret = DSet->putAndInsertUint16(DCM_BitsAllocated, 16);
	OFret = DSet->putAndInsertUint16(DCM_BitsStored, 14);
	OFret = DSet->putAndInsertUint16(DCM_HighBit, 13);
	OFret = DSet->putAndInsertUint16(DCM_PixelRepresentation, 0);


	// create destination file

	USHORT* buffer = new USHORT[nImageCols * nImageRows * sizeof(USHORT)];
	memcpy(buffer, m_image.data(), nImageCols * nImageRows * sizeof(USHORT));

	OFret = DSet->putAndInsertUint16Array(DCM_PixelData, buffer, nImageCols * nImageRows * sizeof(USHORT));
	//destFile.write((char*)buffer, m_importer.nImageRows * m_importer.nImageCols * sizeof(USHORT));
	//write string as const char*

	char * destination = new char[destPath.size() + 1];
	std::copy(destPath.begin(), destPath.end(), destination);
	destination[destPath.size()] = '\0'; // don't forget the terminating 0
	
	dFile->saveFile(destination, EXS_LittleEndianExplicit);
	

	delete[] buffer;
	dFile = nullptr;
	// don't forget to free the string after finished using it
	delete[] destination;


	//return S_OK;
	return TRUE;
}
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
	//DSet = nullptr;
	//delete oldFile;
	//delete DSet;
	
	//delete cond;
	m_image.clear();
	nImageCols = nImageRows = 0;
	dcmDataDict.clear();
	//maybe clear the dataset
	//newDSet = nullptr;

}


BOOL DCMhandler::Convert(CString FName, std::string destPath) {
	//LOAD FILE
	CT2A strFileName(FName);

	DcmFileFormat oldFile;
	//oldFile = new DcmFileFormat();
	DSet = oldFile.getDataset();

	//DcmFileFormat file;
	cond = oldFile.loadFile(strFileName);
	//OFCondition cond = dFile ->loadFile(strFileName);
	//cond = oldFile->loadFile(strFileName);
	//AfxMessageBox(_T("File loaded"), MB_OK || MB_APPLMODAL || MB_ICONEXCLAMATION, 0);

	if (!cond.good())
	{
		AfxMessageBox(_T("Error reading file!"), MB_OK || MB_APPLMODAL || MB_ICONEXCLAMATION, 0);
		return FALSE;
	}

	//ds = file.getDataset();
	//maybe use getLength method
	if (DSet == NULL)
	{
		return FALSE;
	}

	// --------------------------------------------------------------------
	// Modality Check (DX of CR)
	// --------------------------------------------------------------------

	const char* value = "";

	cond = DSet->findAndGetString(DCM_Modality, value);
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

	

	if (GetImagePixelDataFromDataset(nImageCols, nImageRows) == FALSE)
	{
		AfxMessageBox(_T("ERROR: Unable to get image pixel data from DICOM."));
		return FALSE;
	}


	//EXPORT FILE
	
	if (Export(FName, destPath)) {
		return TRUE;

	}

	//oldFile.clear();
	//dcmDataDict.clear();

	return TRUE;
}

BOOL DCMhandler::GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows) {
	//ASSERT(ds != NULL);
	if (DSet == NULL)
	{
		return FALSE;
	}

	// ------------------------------------------------------------------------
	//
	// ------------------------------------------------------------------------

	OFCondition cond;

	Uint16 rows;
	cond = DSet->findAndGetUint16(DCM_Rows, rows);
	nRows = rows;

	Uint16 cols;
	cond = DSet->findAndGetUint16(DCM_Columns, cols);
	nCols = cols;

	Uint16 bitsAllocated;
	cond = DSet->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);

	cond = DSet->findAndGetUint16(DCM_NumberOfFrames, NumberOfFrames);

	cond = DSet->findAndGetUint16(DCM_SamplesPerPixel, SamplesPerPixel);

	cond = DSet->findAndGetUint16(DCM_BitsStored, bitsStored);

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
		DSet->findAndGetUint8Array(DCM_PixelData, value);

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
		DSet->findAndGetUint16Array(DCM_PixelData, value);

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
	DcmFileFormat* newFile = new DcmFileFormat();
	DcmDataset* newDSet = newFile->getDataset();

	OFCondition OFret;
	//OFret = cond;

	//newDSet = DSet; system crashes

	//edit DICom tags - anonymize process
	OFret = newDSet->putAndInsertString(DCM_PatientName, "NameSurname");
	OFret = newDSet->putAndInsertString(DCM_PatientID, "123467890");
	OFret = newDSet->putAndInsertString(DCM_PatientBirthDate, "00-00-0000");
	OFret = newDSet->putAndInsertString(DCM_PatientSex, "X");
	OFret = newDSet->putAndInsertString(DCM_StudyInstanceUID, "1234567890");
	OFret = newDSet->putAndInsertString(DCM_StudyDate, "00-00-0000");
	OFret = newDSet->putAndInsertString(DCM_StudyTime, "00:00:00");
	OFret = newDSet->putAndInsertString(DCM_ReferringPhysicianName, "Physician");
	OFret = newDSet->putAndInsertString(DCM_StudyID, "1234567890");
	OFret = newDSet->putAndInsertString(DCM_AccessionNumber, "1234567890" );
	OFret = newDSet->putAndInsertString(DCM_SeriesInstanceUID, "1234567890" );
	OFret = newDSet->putAndInsertString(DCM_SeriesNumber, "1234567890" );

	Uint16 Uint16val = 1;
	OFret = newDSet->putAndInsertUint16(DCM_SamplesPerPixel, Uint16val);
	OFret = newDSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");

	OFret = newDSet->putAndInsertUint16(DCM_Rows, nImageRows);
	OFret = newDSet->putAndInsertUint16(DCM_Columns,nImageCols);
	OFret = newDSet->putAndInsertUint16(DCM_BitsAllocated, 16);
	OFret = newDSet->putAndInsertUint16(DCM_BitsStored, 14);
	OFret = newDSet->putAndInsertUint16(DCM_HighBit, 13);
	OFret = newDSet->putAndInsertUint16(DCM_PixelRepresentation, 0);


	
	////add pixel data //assess this logic  too
	//USHORT* buffer = new USHORT[nImageCols * nImageRows * sizeof(USHORT)];
	//memcpy(buffer, m_image.data(), nImageCols * nImageRows * sizeof(USHORT));
	//OFret = newDSet->putAndInsertUint16Array(DCM_PixelData,(Uint16*) buffer, nImageCols * nImageRows * sizeof(USHORT));
	//
	////write string as const char*
	//// create destination file
	char * destination = new char[destPath.size() + 1];
	copy(destPath.begin(), destPath.end(), destination);
	destination[destPath.size()] = '\0'; // don't forget the terminating 0
	//
	//newFile->saveFile(destination, EXS_LittleEndianExplicit);
	
	//vector <char> pData = m_image.data();

	//Mako's code
	//vector<Uint16>* Vbuffer = new vector<Uint16>;

	Uint16* buffer = new Uint16[nImageCols * nImageRows * sizeof(USHORT)];
	memcpy(buffer, m_image.data(), nImageCols * nImageRows * sizeof(USHORT));
	
	DSet->putAndInsertUint16Array(DCM_PixelData, /*(Uint16*)*/buffer, nImageCols * nImageRows * sizeof(USHORT));

	delete[] buffer;
	//Vbuffer->clear();

	newFile->saveFile(destination, EXS_LittleEndianExplicit);
	delete[] destination;
	delete newFile;



	//create .raw file
	
	string destPathRaw = destPath;
	destPathRaw.erase(destPathRaw.end() - 4, destPathRaw.end());//remove .dcm
	destPathRaw = destPathRaw + "_" + to_string(nImageCols) + "x" + to_string(nImageRows);
	destPathRaw += ".raw";//add .raw

	// create destination file
	ofstream destFile(destPathRaw, ofstream::binary);

	USHORT* bufferRaw = new USHORT[nImageRows * nImageCols * sizeof(USHORT)];
	memcpy(bufferRaw, m_image.data(), nImageRows * nImageCols * sizeof(USHORT));
	destFile.write((char*)bufferRaw, nImageRows * nImageCols * sizeof(USHORT));
	
	destFile.close();
	delete[] bufferRaw;


	// don't forget to free the string after finished using it
	
	dcmDataDict.clear();

	return S_OK;
}
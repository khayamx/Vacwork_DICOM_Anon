#include "pch.h"
//#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "DCMhandler.h"

//using namespace std;
//
//
//DCMhandler::DCMhandler()
//{
//	//FileName = FName;
//	//m_image = new vector<unsigned short>;
//}
//
//
//DCMhandler::~DCMhandler()
//{
//	m_image.clear();
//	nImageCols = nImageRows = 0;
//	dcmDataDict.clear();
//	
//}

//
//BOOL DCMhandler::Convert(CString FName, CString destPath) {
//	//LOAD FILE
//	CT2A strFileName(FName);
//	FileName = FName;
//
//	DcmFileFormat oldFile;
//	DcmDataset* DSet = new DcmDataset;
//
//	OFCondition cond;
//
//	DSet = oldFile.getDataset();
//
//	cond = oldFile.loadFile(strFileName);
//	
//	if (!cond.good())
//	{
//		AfxMessageBox(_T("Error reading file!"), MB_OK || MB_APPLMODAL || MB_ICONEXCLAMATION, 0);
//		return FALSE;
//	}
//
//	
//	if (DSet == NULL)
//	{
//		return FALSE;
//	}
//
//	// --------------------------------------------------------------------
//	// Modality Check (DX of CR)
//	// --------------------------------------------------------------------
//
//	const char* value = "";
//
//	cond = DSet->findAndGetString(DCM_Modality, value);
//	if (cond.good())
//	{
//		if (strcmp(value, "CR"))  // I don't know why but I appear to *have* to do it this way
//		{
//			if (strcmp(value, "DX"))
//			{
//				//AfxMessageBox(_T("Only CR or DX images can be imported!"));
//				return FALSE;
//			}
//		}
//	}
//
//	
//
//	if (GetImagePixelDataFromDataset(nImageCols, nImageRows) == FALSE)
//	{
//		AfxMessageBox(_T("ERROR: Unable to get image pixel data from DICOM."));
//		return FALSE;
//	}
//

	//EXPORT FILE
	
	//if (Export(FName, destPath, m_image.data())) {
	//	//CleanUp();
	//	return TRUE;
	//}

	
//}
//
//BOOL DCMhandler::GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows, DcmDataset* DS) {
//	//ASSERT(ds != NULL);
//	/*if (DSet == NULL)
//	{
//		return FALSE;
//	}*/
//
//	// ------------------------------------------------------------------------
//	//
//	// ------------------------------------------------------------------------
//
//	OFCondition cond2;
//
//	Uint16 rows;
//	cond2 = DS->findAndGetUint16(DCM_Rows, rows);
//	nRows = rows;
//
//	Uint16 cols;
//	cond2 = DS->findAndGetUint16(DCM_Columns, cols);
//	nCols = cols;
//
//	Uint16 bitsAllocated;
//	cond2 = DS->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);
//
//	cond2 = DS->findAndGetUint16(DCM_NumberOfFrames, NumberOfFrames);
//
//	cond2 = DS->findAndGetUint16(DCM_SamplesPerPixel, SamplesPerPixel);
//
//	cond2 = DS->findAndGetUint16(DCM_BitsStored, bitsStored);
//
//	if (rows == 0 || cols == 0 || bitsAllocated == 0)
//	{
//		return FALSE;
//	}
//
//	// ------------------------------------------------------------------------
//	//
//	// ------------------------------------------------------------------------
//
//	(m_image).resize(rows * cols);
//
//	if (bitsAllocated == 8)
//	{
//		const Uint8* value = NULL;
//		DS->findAndGetUint8Array(DCM_PixelData, value);
//
//		if (value != NULL)
//		{
//			for (int x = 0; x < cols * rows; x++)
//			{
//				m_image[x] = value[x];
//			}
//		}
//		else
//		{
//			return FALSE;
//		}
//		value = NULL;
//	}
//
//	else if (bitsAllocated == 16)
//	{
//		const Uint16* value = NULL;
//		DS->findAndGetUint16Array(DCM_PixelData, value);
//
//		if (value != NULL)
//		{
//			memcpy(m_image.data(), value, rows * cols * sizeof(unsigned short));
//		}
//		else
//		{
//			return FALSE;
//		}
//		value = NULL;
//	}
//	else
//	{
//		return FALSE;
//	}
//	return TRUE;
//	
//}

//
//BOOL DCMhandler::Export(CString destPath,  USHORT* pData) {
//	DcmFileFormat* newFile = new DcmFileFormat();
//	DcmDataset* newDSet = newFile->getDataset();
//	OFCondition OFret;
//	
//	//edit DICom tags - anonymize process
//	OFret = newDSet->putAndInsertString(DCM_PatientName, "NameSurname");
//	OFret = newDSet->putAndInsertString(DCM_PatientID, "123467890");
//	OFret = newDSet->putAndInsertString(DCM_PatientBirthDate, "00-00-0000");
//	OFret = newDSet->putAndInsertString(DCM_PatientSex, "X");
//	OFret = newDSet->putAndInsertString(DCM_StudyInstanceUID, "1234567890");
//	OFret = newDSet->putAndInsertString(DCM_StudyDate, "00-00-0000");
//	OFret = newDSet->putAndInsertString(DCM_StudyTime, "00:00:00");
//	OFret = newDSet->putAndInsertString(DCM_ReferringPhysicianName, "Physician");
//	OFret = newDSet->putAndInsertString(DCM_StudyID, "1234567890");
//	OFret = newDSet->putAndInsertString(DCM_AccessionNumber, "1234567890" );
//	OFret = newDSet->putAndInsertString(DCM_SeriesInstanceUID, "1234567890" );
//	OFret = newDSet->putAndInsertString(DCM_SeriesNumber, "1234567890" );
//
//	Uint16 Uint16val = 1;
//	OFret = newDSet->putAndInsertUint16(DCM_SamplesPerPixel, Uint16val);
//	OFret = newDSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
//
//	OFret = newDSet->putAndInsertUint16(DCM_Rows, nImageRows);
//	OFret = newDSet->putAndInsertUint16(DCM_Columns,nImageCols);
//	OFret = newDSet->putAndInsertUint16(DCM_BitsAllocated, 16);
//	OFret = newDSet->putAndInsertUint16(DCM_BitsStored, 14);
//	OFret = newDSet->putAndInsertUint16(DCM_HighBit, 13);
//	OFret = newDSet->putAndInsertUint16(DCM_PixelRepresentation, 0);
//
//	//add pixel data //assess this logic  too
//	// create destination file
//	CT2A destName(destPath);
//
//	USHORT* buffer = new USHORT[nImageCols * nImageRows * sizeof(USHORT)];
//	memcpy(buffer, pData, nImageCols * nImageRows * sizeof(USHORT));

//validate raw
	//CString destinationPath = _T("C:\\Destination_Folder\\");
	////destinationPath += (FName);
	//CT2A pszConvertedAnsiString2(destinationPath);
	//std::string strOutputDest(pszConvertedAnsiString2);

	//std::ofstream destfile("C:\\Destination_Folder\\"+_T(FName), std::ofstream::binary);


	//USHORT* buffer = new USHORT[rows * cols * sizeof(USHORT)];
	////std::vector<USHORT> bufferV;

	//memcpy(buffer, m_image.data(), rows * cols * sizeof(USHORT));
	////memcpy(bufferV.data(), m_image.data(), rows * cols * sizeof(USHORT));

	//destFile.write((char*)buffer, rows * cols * sizeof(USHORT));
	////destfile.write((char*)bufferV.data(), rows * cols * sizeof(USHORT));

	//destFile.close();


	//make dcm file
//	OFret = newDSet->putAndInsertUint16Array(DCM_PixelData, (Uint16*)buffer, nImageCols * nImageRows * sizeof(USHORT));
//	delete[] buffer;
//	newFile->saveFile(destName, EXS_LittleEndianExplicit);
//	
//	delete newFile;
//
//	return TRUE;
//}
//
//void DCMhandler::CleanUp()
//{
//	dcmDataDict.clear();
//	//delete DSet;
//	
//
//	m_image.resize(0);
//	std::vector<unsigned short> vector_temp;
//	vector_temp.swap(m_image);
//
//	vector_temp.clear();
//	vector_temp.shrink_to_fit();
//
//
//}
///////////////////////////////////////////////////////////////////////////////
// Header File Includes and Forward Declarations
///////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "pch.h"

#include "DicomExporter.h"
//#include "AESettings.h"

#include <iostream>
#include <fstream>
#include <sstream>

BOOL CDicomExporter::Export(CString fileName, USHORT *pData, CString outputDest, int nRows, int nCols)
{
#ifdef KHAYA_CODE
	DcmFileFormat* dFile = new DcmFileFormat();

	DcmDataset *DSet = dFile->getDataset();

	OFCondition OFret;

	OFret = DSet->putAndInsertString(DCM_PatientName, "NameSurname");
	OFret = DSet->putAndInsertString(DCM_PatientID, "123467890");
	OFret = DSet->putAndInsertString(DCM_PatientBirthDate, "00-00-0000");
	OFret = DSet->putAndInsertString(DCM_PatientSex, "X");
	OFret = DSet->putAndInsertString(DCM_StudyInstanceUID, "1234567890" );
	OFret = DSet->putAndInsertString(DCM_StudyDate, "00-00-0000");
	OFret = DSet->putAndInsertString(DCM_StudyTime, "00:00:00");
	OFret = DSet->putAndInsertString(DCM_ReferringPhysicianName, "Physician");
	OFret = DSet->putAndInsertString(DCM_StudyID, "1234567890");
	OFret = DSet->putAndInsertString(DCM_AccessionNumber, "1234567890");
	OFret = DSet->putAndInsertString(DCM_SeriesInstanceUID, "1234567890");
	OFret = DSet->putAndInsertString(DCM_SeriesNumber, "1234567890" );

	Uint16 Uint16val = 1;
	OFret = DSet->putAndInsertUint16(DCM_SamplesPerPixel, Uint16val);
	OFret = DSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");

	OFret = DSet->putAndInsertUint16(DCM_Rows, m_importer.nImageRows);
	OFret = DSet->putAndInsertUint16(DCM_Columns, m_importer.nImageCols);
	OFret = DSet->putAndInsertUint16(DCM_BitsAllocated, 16);
	OFret = DSet->putAndInsertUint16(DCM_BitsStored, 14);
	OFret = DSet->putAndInsertUint16(DCM_HighBit, 13);
	OFret = DSet->putAndInsertUint16(DCM_PixelRepresentation, 0);
	
	// create buffer
	USHORT* buffer = new USHORT[m_importer.nImageCols * m_importer.nImageRows * sizeof(USHORT)];
	memcpy(buffer, m_importer.m_image.data(), m_importer.nImageCols * m_importer.nImageRows * sizeof(USHORT));
	//write dmc first
	OFret = DSet->putAndInsertUint16Array(DCM_PixelData, (Uint16*)buffer, m_importer.nImageCols * m_importer.nImageRows * sizeof(USHORT));
	CT2A destName(outputDest);
	dFile->saveFile(destName, EXS_LittleEndianExplicit);

	//validate w/ raw file
	outputDest.Format((outputDest + _T("_%ux%u.raw")), m_importer.nImageCols, m_importer.nImageRows);
	// create destination file
	std::ofstream destFile(outputDest, std::ofstream::binary);

	//write to raw file
	
	destFile.write((char*)buffer, m_importer.nImageCols * m_importer.nImageRows * sizeof(USHORT));
	destFile.close();



    delete[] buffer;
	delete dFile;
	// don't forget to free the string after finished using it
	


	return TRUE;

#endif

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

	OFret = DSet->putAndInsertUint16(DCM_Rows, nRows);
	OFret = DSet->putAndInsertUint16(DCM_Columns, nCols);
	OFret = DSet->putAndInsertUint16(DCM_BitsAllocated, 16);
	OFret = DSet->putAndInsertUint16(DCM_BitsStored, 14);
	OFret = DSet->putAndInsertUint16(DCM_HighBit, 13);
	OFret = DSet->putAndInsertUint16(DCM_PixelRepresentation, 0);

	//std::ofstream outfile("C:\\Destination2\\NewFile.raw", std::ofstream::binary);
	USHORT* buffer = new USHORT[nRows * nCols * sizeof(USHORT)];
	memcpy(buffer, pData, nRows * nCols * sizeof(USHORT));
	//outfile.write((char*)buffer, m_importer.nImageCols * m_importer.nImageRows * sizeof(USHORT));
	//outfile.close();

	OFret = DSet->putAndInsertUint16Array(DCM_PixelData, (Uint16*)buffer, nRows * nCols * sizeof(USHORT));

	delete[] buffer;

	CT2A DCMDestination(outputDest);
	dFile->saveFile(DCMDestination, EXS_LittleEndianExplicit);

	delete dFile;

	return S_OK;

}

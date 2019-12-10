/**
 *  @file DicomImporter.h
 */

#pragma once

 ///////////////////////////////////////////////////////////////////////////////
 // Header File Includes and Forward Declarations
 ///////////////////////////////////////////////////////////////////////////////

//#include "DbLibIF.h"
//#include "DBLibStruct.h"

#include "dcmtk\dcmdata\dctk.h"

#include <vector>

 ///////////////////////////////////////////////////////////////////////////////
 // Class Definition (Interface)
 ///////////////////////////////////////////////////////////////////////////////

class CDicomImporter
{

public:

    // ------------------------------------------------------------------------
    // Functions
    // ------------------------------------------------------------------------
	//deconstructor
	~CDicomImporter();

   // BOOL Import(CString fileName, std::string outputDest);
	BOOL Import(CString fileName);

	BOOL GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);
	/*void ImportPixelData(_In_ const CStringA fileName);*/

	//UINT16 nRows = 0;
	//UINT16 nCols = 0;
	UINT nImageCols = 0, nImageRows = 0;

	DcmDataset* ds;

	std::vector<unsigned char> m_icon;
	std::vector<unsigned short> m_image;

	/*DbPatientInfo pi;
	DbStudyInfo si;
	DbImageInfo ii;*/

private:

    // ------------------------------------------------------------------------
    // Functions: DICOM
    // ------------------------------------------------------------------------

    //BOOL GetIconPixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);

    //BOOL GetImageInfoFromDataset(_Out_ DbImageInfo& ii);

    //BOOL GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows);

    //BOOL GetPatientInfoFromDataset(_Out_ DbPatientInfo& pi);

    //BOOL GetStudyInfoFromDataset(_Out_ DbStudyInfo& si);

    // ------------------------------------------------------------------------
    // Functions: Database
    // ------------------------------------------------------------------------

    //BOOL AddIconPixelData(
    //    _In_ const int hPatient,
    //    _In_ const int hStudy,
    //    _In_ const int hImage,
    //    _In_ const UINT nCols,
    //    _In_ const UINT nRows);

    /*BOOL AddImagePixelData(
        _In_ const int hPatient,
        _In_ const int hStudy,
        _In_ const int hImage,
        _In_ const UINT nCols,
        _In_ const UINT nRows);
*/
    //BOOL FindOrCreateImage(
    //    _In_ const int hPatient,
    //    _In_ const int hStudy,
    //    _Out_ DbImageInfo& ii,
    //    _Out_ int* phImage,
    //    _Out_ BOOL* sopInstanceUIDExists);

    //BOOL FindOrCreatePatient(
    //    _Out_ DbPatientInfo& pi,
    //    _Out_ int* phPatient);

    //BOOL FindOrCreateStudy(
    //    _In_ const int hPatient,
    //    _Out_ DbStudyInfo& si,
    //    _Out_ int* phStudy);

    // ------------------------------------------------------------------------
    // Member Variables
    // ------------------------------------------------------------------------



	friend class CMainDlg;
};

/**
 *  @file DicomImporter.cpp
 */

///////////////////////////////////////////////////////////////////////////////
// Header File Includes and Forward Declarations
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "DicomImporter.h"
#include "Vacwork_DICOM_AnonDlg.h"
#include <stdio.h>

//#include "Global.h"
//#include "HelperUtils.h"

///////////////////////////////////////////////////////////////////////////////
// Class Declaration (Implementation)
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------


CDicomImporter::~CDicomImporter() {
	ds = NULL;
	m_image.clear();
	m_icon.clear();
	nImageCols = nImageRows = 0;
}


BOOL CDicomImporter::Import(CString fileName)
{
	CT2A strFileName(fileName);

    DcmFileFormat file;
    OFCondition cond = file.loadFile(strFileName);

    if (!cond.good())
    {
        //AfxMessageBox(_T("Error reading file!"), MB_OK || MB_APPLMODAL || MB_ICONEXCLAMATION, 0);
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


    // ------------------------------------------------------------------------
    // Image Pixel Data
    // ------------------------------------------------------------------------

	//GetImagePixelDataFromDataset(nImageCols, nImageRows);

	if (GetImagePixelDataFromDataset(nImageCols, nImageRows) == FALSE)
    {
        //AfxMessageBox(_T("ERROR: Unable to get image pixel data from DICOM."));
        return FALSE;
    }

    
    return TRUE;
}

// ----------------------------------------------------------------------------
// Functions: DICOM
// ----------------------------------------------------------------------------


BOOL CDicomImporter::GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows)
{
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

    m_image.resize(rows * cols);

    if (bitsAllocated == 8)
    {
        const Uint8* value = NULL;
        ds->findAndGetUint8Array(DCM_PixelData, value);

        if (value != NULL)
        {
            for (int x = 0; x < cols * rows; x++)
            {
                m_image[x] = value[x];
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



//BOOL CDicomImporter::FindOrCreateImage(
//    _In_ const int hPatient,
//    _In_ const int hStudy,
//    _Out_ DbImageInfo& ii,
//    _Out_ int* phImage,
//    _Out_ BOOL* sopInstanceUIDExists)
//{
//    ASSERT(hPatient > 0);
//    ASSERT(hStudy > 0);
//    if (hPatient <= 0 || hStudy <= 0)
//    {
//        return FALSE;
//    }
//
//    ASSERT(phImage != 0);
//    if (phImage == NULL)
//    {
//        return FALSE;
//    }
//
//    // ------------------------------------------------------------------------
//    // Try and find a study for this patient with the same Study Instance UID
//    // ------------------------------------------------------------------------
//
//    int nCount = 0;
//    int* pData = NULL;
//    
//    int rc = DbSearchImages(hPatient, hStudy, &nCount, &pData, 1000);
//
//    if (rc != DB_OK)
//    {
//        DbCloseSearch(pData);
//        *phImage = -1;
//        return FALSE;
//    }
//
//    BOOL bFound = FALSE;
//
//    if (nCount > 0)
//    {
//        ASSERT(pData != NULL);
//        if (pData == NULL)
//        {
//            DbCloseSearch(pData);
//            *phImage = -1;
//            return FALSE;
//        }
//
//        DbImageInfo dbImage;
//        dbImage.nMask = DB_IMAGE_INFO_SOP_INSTANCE_UID;
//
//        for (int i = 0; i < nCount; i++)
//        {
//            ASSERT(pData[i] > 0);
//            if (pData[i] <= 0)
//            {
//                DbCloseSearch(pData);
//                *phImage = -1;
//                return FALSE;
//            }
//
//            rc = DbImageInfoGet(hPatient, hStudy, pData[i], &dbImage);
//
//            if (rc != DB_OK)
//            {
//                DbCloseSearch(pData);
//                *phImage = -1;
//                return FALSE;
//            }
//
//            if (!_tcscmp(ii.strSOPInstanceUID, dbImage.strSOPInstanceUID))
//            {
//                bFound = TRUE;
//                *phImage = pData[i];
//                *sopInstanceUIDExists = TRUE;
//                break;
//            }
//        }
//    }
//
//    DbCloseSearch(pData);
//
//    // ------------------------------------------------------------------------
//    // If NOT found, create a new image
//    // ------------------------------------------------------------------------
//
//    if (bFound == FALSE)
//    {
//        rc = DbImageNew(hPatient, hStudy, phImage);
//
//        if (rc != DB_OK)
//        {
//            *phImage = -1;
//            return FALSE;
//        }
//
//        ASSERT(*phImage > 0);
//        if (*phImage <= 0)
//        {
//            return FALSE;
//        }
//
//        ii.nMask = DB_IMAGE_INFO_ALL;
//
//        rc = DbImageInfoSet(hPatient, hStudy, *phImage, &ii);
//
//        if (rc != DB_OK)
//        {
//            *phImage = -1;
//            return FALSE;
//        }
//    }
//
//    return TRUE;
//}
//
//BOOL CDicomImporter::FindOrCreatePatient(
//    _Out_ DbPatientInfo& pi,
//    _Out_ int* phPatient)
//{
//    ASSERT(phPatient != NULL);
//    if (phPatient == NULL)
//    {
//        return FALSE;
//    }
//
//    int nCount = 0;
//    int* pData = NULL;
//
//    int rc = DbSearch(pi.strName, pi.strSurname, pi.strRefNo, &nCount, &pData, 10000);
//
//    if (rc != DB_OK)
//    {
//        DbCloseSearch(pData);
//        *phPatient = -1;
//        return FALSE;
//    }
//
//    if (nCount > 0)
//    {
//        ASSERT(pData != NULL);
//        if (pData == NULL)
//        {
//            return FALSE;
//        }
//
//        ASSERT(pData[0] > 0);
//        if (pData[0] <= 0)
//        {
//            return FALSE;
//        }
//
//        *phPatient = pData[0];
//    }
//    else
//    {
//        rc = DbPatientNew(phPatient);
//
//        if (rc != DB_OK)
//        {
//            DbCloseSearch(pData);
//            *phPatient = -1;
//            return FALSE;
//        }
//
//        ASSERT(*phPatient > 0);
//        if (*phPatient <= 0)
//        {
//            return FALSE;
//        }
//
//        pi.nMask = DB_PATIENT_INFO_ALL;
//
//        int hLock;
//        DbLockPatientRecord(*phPatient, &hLock);
//
//        int rc = DbPatientInfoSet(*phPatient, &pi, hLock);
//
//        if (rc != DB_OK)
//        {
//            DbCloseSearch(pData);
//            *phPatient = -1;
//            return FALSE;
//        }
//
//        DbUnlockPatientRecord(hLock);
//    }
//
//    DbCloseSearch(pData);
//
//    return TRUE;
//}
//
//BOOL CDicomImporter::FindOrCreateStudy(
//    _In_ const int hPatient,
//    _Out_ DbStudyInfo& si,
//    _Out_ int* phStudy)
//{
//    ASSERT(phStudy != NULL);
//    if (phStudy == NULL)
//    {
//        return FALSE;
//    }
//
//    // ------------------------------------------------------------------------
//    // Try and find a study for this patient with the same Study Instance UID
//    // ------------------------------------------------------------------------
//
//    int nCount = 0;
//    int* pData = NULL;
//
//    int rc = DbSearchStudies(hPatient, &nCount, &pData, 100);
//
//    if (rc != DB_OK)
//    {
//        DbCloseSearch(pData);
//        *phStudy = -1;
//        return FALSE;
//    }
//
//    BOOL bFound = FALSE;
//
//    if (nCount > 0)
//    {
//        ASSERT(pData != NULL);
//        if (pData == NULL)
//        {
//            DbCloseSearch(pData);
//            *phStudy = -1;
//            return FALSE;
//        }
//
//        DbStudyInfo dbStudy;
//        dbStudy.nMask = DB_STUDY_INFO_STUDY_INSTANCE_UID;
//
//        for (int i = 0; i < nCount; i++)
//        {
//            ASSERT(pData[i] > 0);
//            if (pData[i] <= 0)
//            {
//                DbCloseSearch(pData);
//                *phStudy = -1;
//                return FALSE;
//            }
//
//            rc = DbStudyInfoGet(hPatient, pData[i], &dbStudy);
//
//            if (rc != DB_OK)
//            {
//                DbCloseSearch(pData);
//                *phStudy = -1;
//                return FALSE;
//            }
//
//            if (!_tcscmp(si.strStudyInstanceUID, dbStudy.strStudyInstanceUID))
//            {
//                bFound = TRUE;
//                *phStudy = pData[i];
//                break;
//            }
//        }
//    }
//
//    DbCloseSearch(pData);
//
//    // ------------------------------------------------------------------------
//    // If NOT found, create a new study
//    // ------------------------------------------------------------------------
//
//    if (bFound == FALSE)
//    {
//        rc = DbStudyNew(hPatient, phStudy);
//
//        if (rc != DB_OK)
//        {
//            *phStudy = -1;
//            return FALSE;
//        }
//
//        ASSERT(*phStudy > 0);
//        if (*phStudy <= 0)
//        {
//            return FALSE;
//        }
//
//        if (!_tcscmp(si.strDate, _T("")) || !_tcscmp(si.strTime, _T("")))
//        {
//            si.nMask = DB_STUDY_INFO_REF_DOCTOR | DB_STUDY_INFO_LABEL | DB_STUDY_INFO_STUDY_INSTANCE_UID | DB_STUDY_INFO_ACCESSION_NUM;
//        }
//        else
//        {
//            si.nMask = DB_STUDY_INFO_ALL;
//        }
//
//        int hLock = 0;
//        DbLockStudyRecord(hPatient, *phStudy, &hLock);
//
//        rc = DbStudyInfoSet(hPatient, *phStudy, &si, hLock);
//
//        if (rc != DB_OK)
//        {
//            *phStudy = -1;
//            return FALSE;
//        }
//
//        DbUnlockStudyRecord(hLock);
//    }
//
//    return TRUE;
//}
//

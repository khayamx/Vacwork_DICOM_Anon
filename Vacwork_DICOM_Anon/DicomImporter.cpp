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

//void CDicomImporter::ImportPixelData(_In_ const CStringA fileName)
//{
//	DcmFileFormat file;
//	OFCondition cond = file.loadFile((LPCSTR)fileName);
//
//	if (!cond.good())
//	{
//		AfxMessageBox(_T("Error reading file!"), MB_OK || MB_APPLMODAL || MB_ICONEXCLAMATION, 0);
//		return;
//	}
//
//	ds = file.getDataset();
//
//	if (ds == NULL)
//	{
//		return;
//	}
//
//	// --------------------------------------------------------------------
//	// Modality Check (DX of CR)
//	// --------------------------------------------------------------------
//
//	const char* value = "";
//
//	cond = ds->findAndGetString(DCM_Modality, value);
//	if (cond.good())
//	{
//		if (strcmp(value, "CR"))  // I don't know why but I appear to *have* to do it this way
//		{
//			if (strcmp(value, "DX"))
//			{
//				AfxMessageBox(_T("Only CR or DX images can be imported!"));
//				return;
//			}
//		}
//	}
//
//	ASSERT(ds != NULL);
//	if (ds == NULL)
//	{
//		return;
//	}
//
//	// ------------------------------------------------------------------------
//	//
//	// ------------------------------------------------------------------------
//
//	//OFCondition cond;
//
//	Uint16 rows;
//	cond = ds->findAndGetUint16(DCM_Rows, rows);
//	nRows = rows;
//
//	Uint16 cols;
//	cond = ds->findAndGetUint16(DCM_Columns, cols);
//	nCols = cols;
//
//	Uint16 bitsAllocated;
//	cond = ds->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);
//
//	m_image.resize(rows * cols);
//
//	if (bitsAllocated == 8)
//	{
//		const Uint8* value = NULL;
//		ds->findAndGetUint8Array(DCM_PixelData, value);
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
//			return;
//		}
//	}
//	else if (bitsAllocated == 16)
//	{
//		const Uint16* value = NULL;
//		ds->findAndGetUint16Array(DCM_PixelData, value);
//
//		if (value != NULL)
//		{
//			memcpy(m_image.data(), value, rows * cols * sizeof(unsigned short));
//		}
//		else
//		{
//			return;
//		}
//	}
//	else
//	{
//		return;
//	}
//}


CDicomImporter::~CDicomImporter() {
	ds = NULL;
	m_image.clear();
	m_icon.clear();
	nImageCols = nImageRows = 0;
}


BOOL CDicomImporter::Import(CString fileName /*, std::string outputDest*/)
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


    // --------------------------------------------------------------------
    // Patient Info
    // --------------------------------------------------------------------

 /*   if (GetPatientInfoFromDataset(pi) == FALSE)
    {
        AfxMessageBox(_T("ERROR: Unable to get patient info from DICOM."));
        return FALSE;
    }*/

    //int hPatient = -1;

   /* if (FindOrCreatePatient(pi, &hPatient) == FALSE)
    {
        AfxMessageBox(_T("ERROR: Unable to add patient info to database."));
        return FALSE;
    }*/

    // --------------------------------------------------------------------
    // Study Info
    // --------------------------------------------------------------------

    //if (GetStudyInfoFromDataset(si) == FALSE)
    //{
    //    AfxMessageBox(_T("ERROR: Unable to get study info from DICOM."));
    //    return FALSE;
    //}

    //int hStudy = -1;

    /*if (FindOrCreateStudy(hPatient, si, &hStudy) == FALSE)
    {
        AfxMessageBox(_T("ERROR: Unable to add study info to database."));
        return FALSE;
    }*/

    // --------------------------------------------------------------------
    // Image Info
    // --------------------------------------------------------------------

    //if (GetImageInfoFromDataset(ii) == FALSE)
    //{
    //    AfxMessageBox(_T("ERROR: Unable to get image info from DICOM."));
    //    return FALSE;
    //}

    //int hImage = -1;
    //BOOL sopInstanceUIDExists = FALSE;

    //if (FindOrCreateImage(hPatient, hStudy, ii, &hImage, &sopInstanceUIDExists) == FALSE)
    //{
    //    AfxMessageBox(_T("ERROR: Unable to add image info to database."));
    //    return FALSE;
    //}

    //if (sopInstanceUIDExists == TRUE)
    //{
    //    CString msg;
    //    msg.Format(_T("An image with this SOP Instance UID already exists.\nSee image with ID: %d."), hImage);
    //    AfxMessageBox(msg);
    //    return TRUE;
    //}

    // ------------------------------------------------------------------------
    // Image Pixel Data
    // ------------------------------------------------------------------------

	GetImagePixelDataFromDataset(nImageCols, nImageRows);

	if (GetImagePixelDataFromDataset(nImageCols, nImageRows) == FALSE)
    {
        //AfxMessageBox(_T("ERROR: Unable to get image pixel data from DICOM."));
        return FALSE;
    }

    //if (nImageCols <= 0 || nImageRows <= 0)
    //{
    //    AfxMessageBox(_T("ERROR: Unable to get image pixel data from DICOM."));
    //    return FALSE;
    //}

    /*if (AddImagePixelData(hPatient, hStudy, hImage, nImageCols, nImageRows) == FALSE)
    {
        AfxMessageBox(_T("ERROR: Unable to add image pixel data to database."));
        return FALSE;
    }*/

    // ------------------------------------------------------------------------
    // Icon Pixel Data
    // ------------------------------------------------------------------------

    //UINT nIconCols = 0, nIconRows = 0;

    //if (GetIconPixelDataFromDataset(nIconCols, nIconRows) == FALSE)
    //{
    //    // TODO (ShaneA): Don't like including this whole CHelperUtils class for two functions
    //    unsigned short cols = nImageCols, rows = nImageRows;
    //    CHelperUtils hu;
    //    hu.ReduceRawImage(m_image.data(), rows, cols);
    //    hu.UMaskRawImage(rows, cols);

    //    m_icon.resize(rows * cols);

    //    memcpy(m_icon.data(), hu.getUMptr(), rows * cols);

    //    nIconCols = cols;
    //    nIconRows = rows;

    //    if (hu.DeallocateCondMemory())
    //    {
    //        OutputDebugString(_T("Cannot delete conditioned memory!\n"));
    //    }
    //}

    //if (nIconCols <= 0 || nIconRows <= 0)
    //{
    //    AfxMessageBox(_T("ERROR: Unable to get icon pixel data from DICOM."));
    //    return FALSE;
    //}

    //if (AddIconPixelData(hPatient, hStudy, hImage, nIconCols, nIconRows) == FALSE)
    //{
    //    AfxMessageBox(_T("ERROR: Unable to add icon pixel data to database."));
    //    return FALSE;
    //}



    return TRUE;
}

// ----------------------------------------------------------------------------
// Functions: DICOM
// ----------------------------------------------------------------------------

//BOOL CDicomImporter::GetIconPixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows)
//{
//    ASSERT(ds != NULL);
//    if (ds == NULL)
//    {
//        return FALSE;
//    }
//
//    // ------------------------------------------------------------------------
//    //
//    // ------------------------------------------------------------------------
//
//    OFCondition cond;
//
//    DcmItem* seq = NULL;
//    cond = ds->findAndGetSequenceItem(DCM_IconImageSequence, seq);
//
//    if (!cond.good())
//    {
//        return FALSE;
//    }
//
//    // ------------------------------------------------------------------------
//    //
//    // ------------------------------------------------------------------------
//
//    Uint16 rows = 0;
//    cond = seq->findAndGetUint16(DCM_Rows, rows);
//    nRows = rows;
//
//    Uint16 cols = 0;
//    cond = seq->findAndGetUint16(DCM_Columns, cols);
//    nCols = cols;
//
//    Uint16 nBitsAllocated = 0;
//    cond = seq->findAndGetUint16(DCM_BitsAllocated, nBitsAllocated);
//
//    if (nRows <= 0 || nCols <= 0 || nBitsAllocated != 8)
//    {
//        return FALSE;
//    }
//
//    // ------------------------------------------------------------------------
//    //
//    // ------------------------------------------------------------------------
//
//    // It can't hurt to add extra space (double in total), just in case...
//    m_icon.resize(2 * rows * cols);
//
//    const Uint8* value = NULL;
//    cond = seq->findAndGetUint8Array(DCM_PixelData, value);
//
//    if (cond.good())
//    {
//        ASSERT(value != NULL);
//        if (value == NULL)
//        {
//            return FALSE;
//        }
//
//        memcpy(m_icon.data(), value, rows * cols);
//    }
//
//    return TRUE;
//}



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


//
//BOOL CDicomImporter::GetImageInfoFromDataset(_Out_ DbImageInfo& ii)
//{
//    ASSERT(ds != NULL);
//    if (ds == NULL)
//    {
//        return FALSE;
//    }
//
//    const char* value = NULL;
//
//    OFCondition cond;
//    cond = ds->findAndGetString(DCM_SOPInstanceUID, value);
//    if (value == NULL)
//    {
//        _tcscpy(ii.strSOPInstanceUID, _T(""));
//    }
//    else
//    {
//        _tcscpy(ii.strSOPInstanceUID, CA2T(value));
//    }
//
//    cond = ds->findAndGetString(DCM_PhotometricInterpretation, value);
//    if (value == NULL || !strcmp(value, "MONOCHROME2"))
//    {
//        ii.ViewSetup.bInvert = false;
//    }
//    else
//    {
//        ii.ViewSetup.bInvert = true;
//    }
//
//    ii.ViewSetup.cUM = 0;
//
//    _tcscpy(ii.strLabel, _T(""));
//    _tcscpy(ii.strComments, _T(""));
//
//    cond = ds->findAndGetFloat64(DCM_KVP, ii.dKV);
//    ii.dKV *= 1000.0;
//
//    // Area dose product from dGy.cm^2 to uGy.m^2
//    cond = ds->findAndGetFloat64(DCM_ImageAndFluoroscopyAreaDoseProduct, ii.dMeasuredDose);
//    ii.dMeasuredDose *= 10.0;
//
//    long int templong;
//    cond = ds->findAndGetLongInt(DCM_XRayTubeCurrent, templong);
//    ii.dmA = (double)templong / 1000.0;
//
//    cond = ds->findAndGetLongInt(DCM_ExposureTime, templong);
//    ii.dExposureTime = (double)templong;
//
//    ii.dScanTime = 0.0;
//
//    Float64 tempfloat;
//
//    cond = ds->findAndGetFloat64(DCM_WindowWidth, tempfloat);
//    ii.ViewSetup.nPlainPalGain = (int)tempfloat;
//
//    cond = ds->findAndGetFloat64(DCM_WindowCenter, tempfloat);
//    ii.ViewSetup.nPlainPalScroll = (int)tempfloat - (ii.ViewSetup.nPlainPalGain / 2);
//
//    cond = ds->findAndGetFloat64(DCM_SpatialResolution, tempfloat);
//    ii.dResolution = tempfloat;
//
//    cond = ds->findAndGetFloat64(DCM_ScanVelocity, tempfloat);
//    ii.dScanSpeed = tempfloat;
//
//    cond = ds->findAndGetFloat64(DCM_PositionerPrimaryAngle, tempfloat);
//    ii.dRotationAngle = -tempfloat;
//
//    Float64 SID = 0.0;
//    cond = ds->findAndGetFloat64(DCM_DistanceSourceToDetector, SID);
//    if (cond.good())
//    {
//        if (!SID)
//        {
//            SID = DISTANCE_FROM_SOURCE_TO_DETECTOR;
//        }
//    }
//    else
//    {
//        SID = DISTANCE_FROM_SOURCE_TO_DETECTOR;
//    }
//
//    cond = ds->findAndGetFloat64(DCM_DistanceSourceToPatient, tempfloat);
//    if (cond.good())
//    {
//        if (tempfloat)
//        {
//            ii.dDetectorToSubject = SID - tempfloat;
//            ii.bAspectCorrection = true;
//        }
//        else
//        {
//            ii.dDetectorToSubject = 0.0;
//            ii.bAspectCorrection = false;
//        }
//
//    }
//    else
//    {
//        ii.dDetectorToSubject = 0.0;
//        ii.bAspectCorrection = false;
//    }
//
//    DcmTag ButtStartTag(0x0019, 0x1001);
//    ButtStartTag.setPrivateCreator("LODOX_STATSCAN");
//    ButtStartTag.setVR(EVR_IS);
//    cond = ds->findAndGetString(ButtStartTag, value);
//
//    long mListVM = getVMFromString(value);
//    char* tmpVR = NULL;
//    long tmplong = 0;
//    for (long loop = 0; loop < mListVM; loop++)
//    {
//        tmpVR = getFirstValueFromString((const char*&)value);
//#ifndef _UNICODE
//        ii.nButStart[loop] = atoi(tmpVR);
//#endif
//        delete tmpVR;
//    }
//
//    DcmTag ButtSizeTag(0x0019, 0x1002);
//    ButtSizeTag.setPrivateCreator("LODOX_STATSCAN");
//    ButtSizeTag.setVR(EVR_IS);
//    templong = 0;
//    cond = ds->findAndGetLongInt(ButtSizeTag, templong);
//#ifndef _UNICODE
//    ii.nButSize = templong;
//#endif
//
//    DcmTag StartScanTag(0x0019, 0x1003);
//    StartScanTag.setPrivateCreator("LODOX_STATSCAN");
//    StartScanTag.setVR(EVR_DS);
//    tempfloat = 0.0;
//    cond = ds->findAndGetFloat64(StartScanTag, tempfloat);
//    ii.dStartOfScan = tempfloat;
//
//    DcmTag StopScanTag(0x0019, 0x1004);
//    StopScanTag.setPrivateCreator("LODOX_STATSCAN");
//    StopScanTag.setVR(EVR_DS);
//    tempfloat = 0.0;
//    cond = ds->findAndGetFloat64(StopScanTag, tempfloat);
//    ii.dStopOfScan = tempfloat;
//
//    DcmTag ScanWidthTag(0x0019, 0x1005);
//    ScanWidthTag.setPrivateCreator("LODOX_STATSCAN");
//    ScanWidthTag.setVR(EVR_DS);
//    tempfloat = 0.0;
//    cond = ds->findAndGetFloat64(ScanWidthTag, tempfloat);
//    ii.dScanWidth = tempfloat;
//
//    DcmTag StartColPosTag(0x0019, 0x1006);
//    StartColPosTag.setPrivateCreator("LODOX_STATSCAN");
//    StartColPosTag.setVR(EVR_DS);
//    cond = ds->findAndGetFloat64(StartColPosTag, tempfloat);
//    ii.dStartColPos = tempfloat;
//
//    DcmTag EndColPosTag(0x0019, 0x1007);
//    EndColPosTag.setPrivateCreator("LODOX_STATSCAN");
//    EndColPosTag.setVR(EVR_DS);
//    cond = ds->findAndGetFloat64(EndColPosTag, tempfloat);
//    ii.dEndColPos = tempfloat;
//
//    DcmTag SlotThicknessTag(0x0019, 0x1008);
//    SlotThicknessTag.setPrivateCreator("LODOX_STATSCAN");
//    SlotThicknessTag.setVR(EVR_DS);
//    cond = ds->findAndGetFloat64(SlotThicknessTag, tempfloat);
//    ii.dSlotThickness = tempfloat;
//
//    cond = ds->findAndGetString(DCM_BodyPartExamined, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(ii.strProcedureSelected, _T(""));
//        }
//        else
//        {
//            _tcscpy(ii.strProcedureSelected, CA2T(value));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_ContentTime, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(ii.strTime, _T(""));
//        }
//        else
//        {
//            char Time[20];
//            strcpy(Time, value);
//
//            OFString time;
//            DcmTime::getISOFormattedTimeFromString((OFString)Time, time);
//
//            _tcscpy(ii.strTime, CA2T(time.c_str()));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_ContentDate, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(ii.strDate, _T(""));
//        }
//        else
//        {
//            char Date[20];
//            strcpy(Date, value);
//
//            OFString date;
//            DcmDate::getISOFormattedDateFromString((OFString)Date, date);
//
//            _tcscpy(ii.strDate, CA2T(date.c_str()));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_OperatorsName, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(ii.strUserName, _T(""));
//        }
//        else
//        {
//            _tcscpy(ii.strUserName, CA2T(value));
//        }
//    }
//
//    return TRUE;
//}
//
//

//
//BOOL CDicomImporter::GetPatientInfoFromDataset(_Out_ DbPatientInfo& pi)
//{
//    ASSERT(ds != NULL);
//    if (ds == NULL)
//    {
//        return FALSE;
//    }
//
//    const char* value = "";
//
//    OFCondition cond;
//    cond = ds->findAndGetString(DCM_PatientName, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(pi.strName, _T(""));
//            _tcscpy(pi.strSurname, _T(""));
//        }
//        else
//        {
//            char PatientName[255];
//            strcpy(PatientName, value);
//
//            OFString surname;
//            OFString firstname;
//            DcmPersonName::getNameComponentsFromString((OFString)PatientName, surname, firstname, (OFString)"", (OFString)"", (OFString)"");
//
//            _tcscpy(pi.strName, CA2T(firstname.c_str()));
//            _tcscpy(pi.strSurname, CA2T(surname.c_str()));
//
//            pi.strInitials[0] = CA2T(firstname.c_str())[0];
//            pi.strInitials[1] = CA2T(surname.c_str())[0];
//            pi.strInitials[2] = _T('\0');
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_PatientID, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(pi.strRefNo, _T(""));
//        }
//        else
//        {
//            _tcscpy(pi.strRefNo, CA2T(value));
//        }
//    }
//
//	CString dicomImportValue = _T("  - -");
//
//    cond = ds->findAndGetString(DCM_PatientBirthDate, value);
//    if (cond.good())
//    {
//        if ((value == NULL) || (CString)(value) == dicomImportValue) //If the Date of Birth is not set, set as current date.
//        {
//			SYSTEMTIME lt;
//			memset(&lt, 0, sizeof(SYSTEMTIME));
//
//			GetLocalTime(&lt);
//			_stprintf_s(pi.strDateOfBirth, _T("%4d-%2d-%2d"), lt.wYear, lt.wMonth, lt.wDay);
//        }
//        else
//        {
//            char Date[20];
//            strcpy(Date, value);
//
//            OFString date;
//            DcmDate::getISOFormattedDateFromString((OFString)Date, date);
//
//            _tcscpy(pi.strDateOfBirth, CA2T(date.c_str()));
//        }
//    }
//
//    _tcscpy(pi.strIDNo, _T(""));
//
//    cond = ds->findAndGetString(DCM_PatientSex, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(pi.strSex, _T("U"));
//        }
//        else
//        {
//            if (value[0] == 'M')
//            {
//                _tcscpy(pi.strSex, _T("M"));
//            }
//            else if (value[0] == 'F')
//            {
//                _tcscpy(pi.strSex, _T("F"));
//            }
//            else
//            {
//                _tcscpy(pi.strSex, _T("U"));
//            }
//        }
//    }
//
//    return TRUE;
//}
//
//BOOL CDicomImporter::GetStudyInfoFromDataset(_Out_ DbStudyInfo& si)
//{
//    ASSERT(ds != NULL);
//    if (ds == NULL)
//    {
//        return FALSE;
//    }
//
//    const char* value = NULL;
//
//    OFCondition cond;
//    cond = ds->findAndGetString(DCM_StudyInstanceUID, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(si.strStudyInstanceUID, _T(""));
//        }
//        else
//        {
//            _tcscpy(si.strStudyInstanceUID, CA2T(value));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_AccessionNumber, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(si.strAccessionNumber, _T("-1"));
//        }
//        else
//        {
//            _tcscpy(si.strAccessionNumber, CA2T(value));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_StudyDate, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(si.strDate, _T(""));
//        }
//        else
//        {
//            char Date[20];
//            strcpy(Date, value);
//
//            OFString date;
//            DcmDate::getISOFormattedDateFromString((OFString)Date, date);
//
//            _tcscpy(si.strDate, CA2T(date.c_str()));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_StudyTime, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(si.strTime, _T(""));
//        }
//        else
//        {
//            char Time[20];
//            strcpy(Time, value);
//
//            OFString time;
//            DcmTime::getISOFormattedTimeFromString((OFString)Time, time);
//
//            _tcscpy(si.strTime, CA2T(time.c_str()));
//        }
//    }
//
//    cond = ds->findAndGetString(DCM_ReferringPhysicianName, value);
//    if (cond.good())
//    {
//        if (value == NULL)
//        {
//            _tcscpy(si.strRefDoc, _T(""));
//        }
//        else
//        {
//            _tcscpy(si.strRefDoc, CA2T(value));
//        }
//    }
//
//    _tcscpy(si.strLabel, _T(""));
//
//    return TRUE;
//}

// ----------------------------------------------------------------------------
// Functions: Database
// ----------------------------------------------------------------------------

//BOOL CDicomImporter::AddIconPixelData(
//    _In_ const int hPatient,
//    _In_ const int hStudy,
//    _In_ const int hImage,
//    _In_ const UINT nCols,
//    _In_ const UINT nRows)
//{
//    int rc = DbImageAddLinesUM(hPatient, hStudy, hImage, nRows, nCols, m_icon.data());
//
//    if (rc != DB_OK)
//    {
//        return FALSE;
//    }
//
//    rc = DbSaveImageUM(hPatient, hStudy, hImage);
//
//    if (rc != DB_OK)
//    {
//        return FALSE;
//    }
//
//    return TRUE;
//}

//BOOL CDicomImporter::AddImagePixelData(
//    _In_ const int hPatient,
//    _In_ const int hStudy,
//    _In_ const int hImage,
//    _In_ const UINT nCols,
//    _In_ const UINT nRows)
//{
//    int rc = DbImageAddLinesRaw(hPatient, hStudy, hImage, nRows, nCols, m_image.data());
//
//    if (rc != DB_OK)
//    {
//        return FALSE;
//    }
//
//    rc = DbSaveImageRaw(hPatient, hStudy, hImage);
//
//    if (rc != DB_OK)
//    {
//        return FALSE;
//    }
//
//    return TRUE;
//}

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

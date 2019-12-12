//------------------------------------------------------------------
//Class for importing and exportinf dicom files
//------------------------------------------------------------------
#include "stdafx.h"
#include "pch.h"


#include <iostream>
#include <fstream>
#include <sstream>

BOOL CDicomHandler::Convert(CString FName, std::string outputDest) {
	return TRUE;

}


BOOL CDicomHandler::GetImagePixelDataFromDataset(_Out_ UINT& nCols, _Out_ UINT& nRows)
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
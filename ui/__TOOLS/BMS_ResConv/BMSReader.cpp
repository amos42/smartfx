// BMSReader.cpp: implementation of the CBMSReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMSReader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CBMSReader::CBMSReader()
{
	memset(&m_hHeader, NULL, sizeof(m_hHeader));
}

CBMSReader::~CBMSReader()
{
	Close();
}

//////////////////////////////////////////////////////////////////////
// CBMSReader operations

BOOL CBMSReader::Open(char* lpszFilename)
{
	Close();

	m_hHeader.file = fopen(lpszFilename, "rb");
	if (!m_hHeader.file) return FALSE;

	fread(&m_hHeader.count, sizeof(m_hHeader.count), 1, m_hHeader.file);
	
	if (!m_hHeader.count)
	{
		Close();
		return FALSE;
	}

	m_hHeader.index = new BMSINDEX[m_hHeader.count];
	fread(m_hHeader.index, sizeof(BMSINDEX), m_hHeader.count, m_hHeader.file);

	return TRUE;
}

void CBMSReader::Close()
{
	if (m_hHeader.file) fclose(m_hHeader.file);
	if (m_hHeader.index) delete[] m_hHeader.index;

	memset(&m_hHeader, NULL, sizeof(m_hHeader));
}

BOOL CBMSReader::IsOpened()
{
	return m_hHeader.file && m_hHeader.index ? TRUE : FALSE;
}

BOOL CBMSReader::GetString(DWORD dwId, wchar_t* lpszString, long lMaxLen)
{
	if (!m_hHeader.file || !lpszString) return FALSE;

	long lIndex = SearchIndex(dwId);
	if (lIndex < 0 || lIndex >= (long)m_hHeader.count) return FALSE;

	fseek(m_hHeader.file, m_hHeader.index[lIndex].offset, SEEK_SET);
	if ((lMaxLen-1)*sizeof(wchar_t) < m_hHeader.index[lIndex].size)
	{
		fread(lpszString, (lMaxLen-1)*sizeof(wchar_t), 1, m_hHeader.file);
		lpszString[lMaxLen-1] = NULL;
	}
	else
		fread(lpszString, m_hHeader.index[lIndex].size, 1, m_hHeader.file);

	return TRUE;
}

// retval: -1 is not found
#define CEIL(var)	((var)-(int)(var) > 0 ? (int)(var+1) : (int)(var))

long CBMSReader::SearchIndex(DWORD dwId)
{
	if(m_hHeader.count <= 0) return -1;

	//ASSERT(strlen(lpszId) < BMS_MAX_ID);

	BMSINDEX* pIndex = m_hHeader.index;
	
	long lS = 0;
	long lM = m_hHeader.count/2;
	long lE = m_hHeader.count-1;

	long lRes;

	while (1)
	{
		if (pIndex[lM].idx == dwId) return lM;
		
		lRes = pIndex[lM].idx > dwId ? 1 : -1;

		if (lE-lS <= 1)
		{
			if (lE == lS) break;

			if (lM == lS)
			{
				lM = lE;
				lS = lE;
			}
			else if (lM == lE)
			{
				lM = lS;
				lE = lS;
			}
		}

		if (lRes > 0)
		{
			lE = lM;
			lM = (lS+lE)>>1;
		}
		else if (lRes < 0)
		{
			lS = lM;
			lM = CEIL((lS+lE)/2.);
		}
	}
	
	return -1;
}

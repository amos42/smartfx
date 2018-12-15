// BMSWriter.cpp: implementation of the CBMSWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMSWriter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int qsortproc_record(const void *src, const void *dst)
{
	LPSTR lpstrFilenameA = ((BMSWINDEX*)src)->id;
	LPSTR lpstrFilenameB = ((BMSWINDEX*)dst)->id;

	long lStrLenA = strlen(lpstrFilenameA);
	long lStrLenB = strlen(lpstrFilenameB);

	long lStrLenMin = min(lStrLenA, lStrLenB);

	for (long i=0; i<lStrLenMin; i++)
	{
		if (lpstrFilenameA[i] < lpstrFilenameB[i]) return -1;
		else if (lpstrFilenameA[i] > lpstrFilenameB[i]) return 1;
	}

	if (lStrLenA < lStrLenB) return -1;
	else if (lStrLenA > lStrLenB) return 1;
	
	return 0;
}

int qsortproc_index(const void *src, const void *dst)
{
	long dwIndex1 = (long)((BMSWINDEX*)src)->idx_id;
	long dwIndex2 = (long)((BMSWINDEX*)dst)->idx_id;

	if (dwIndex1 < dwIndex2) return -1;
	else if (dwIndex1 > dwIndex2) return 1;
	
	BMSWINDEX* pIDX = (BMSWINDEX*)src;
	char msg[256];
	sprintf(msg, "%d %s is already exist!", dwIndex1, pIDX->id);
	AfxGetMainWnd()->MessageBox(msg, "Header parsing Error!", MB_OK|MB_ICONERROR);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CBMSWriter::CBMSWriter()
{
	memset(&m_hHeader, NULL, sizeof(m_hHeader));
}

CBMSWriter::~CBMSWriter()
{
	Close();
}

//////////////////////////////////////////////////////////////////////
// CBMSWriter operatons

void CBMSWriter::Close()
{
	if (m_hHeader.file) fclose(m_hHeader.file);
	if (m_hHeader.index) delete[] m_hHeader.index;

	memset(&m_hHeader, NULL, sizeof(m_hHeader));
}

DWORD CBMSWriter::ReadFile(char* lpszFilename)
{
	if (!lpszFilename || !lpszFilename[0]) return NULL;

	CFile file(lpszFilename, CFile::modeRead);

	DWORD dwSize = file.GetLength();
	BYTE* pBuffer = new BYTE[dwSize*2];
	memset(pBuffer, NULL, dwSize*2);

	file.Read(pBuffer, dwSize);

	file.Close();

	// check uni-code
	DWORD dwCount;

	BOOL bUnicode = ((WORD*)pBuffer)[0] == 0xFEFF ? TRUE : FALSE;
	
	if (bUnicode)
	{
		memmove(pBuffer, pBuffer+2, dwSize-2);
		dwCount = (dwSize-2)/2;
		dwSize -= 2;
	}
	else
	{
		for (long i=(long)(dwSize-1); i>=0; i--)
		{
			BYTE tVal = pBuffer[i];
			pBuffer[i] = 0x00;
			pBuffer[i*2] = tVal;
		}

		dwCount = dwSize;
		dwSize *= 2;
	}
	
	m_arrItem.RemoveAll();

	BMSRECORD item;
	
	DWORD i, j;
	DWORD len;
	WORD cmp;
	wchar_t* pPtr = (wchar_t*)pBuffer;

	for (i=0; i<dwCount; i++, pPtr++)
	{
		if (wcsncmp(pPtr, L"IDS_", 4)) continue;
		
		memset(&item, NULL, sizeof(item));

		for (j=i; j<dwCount; j++)
		{
			len = j-i;
			cmp = (WORD)(pPtr[len]);

			if (cmp != 0x0000 && cmp != 0x0009 && cmp != 0x0020 ) continue;

			item.sizeid = (WORD)len;

			ASSERT(item.sizeid < BMS_MAX_ID);

			wcstombs(item.id, pPtr, item.sizeid);
			i += item.sizeid+1;
			pPtr += item.sizeid+1;
			break;
		}

		if (!item.sizeid) continue;
		item.sizeid = WORD_ALIGN_LENGTH(item.sizeid+1);

		DWORD dwStart = 0xFFFFFFFF;

		for (j=i; j<dwCount; j++)
		{
			len = j-i;
			cmp = (WORD)(pPtr[len]);

			if (cmp != L'\"') continue;

			if (dwStart==0xFFFFFFFF)
			{
				dwStart = len+1;
				continue;
			}

			item.sizestr = (WORD)(len-dwStart);

			ASSERT(item.sizestr < BMS_MAX_STRING);

			wcsncpy(item.str, pPtr+dwStart, item.sizestr);
			i += item.sizestr+1;
			pPtr += item.sizestr+1;

			break;
		}

		//if (!item.sizestr) continue;
		item.sizestr = WORD_ALIGN_LENGTH(sizeof(wchar_t)*(item.sizestr+1));

		m_arrItem.Add(item);
	}//endof for


	DWORD dwItemCnt = m_arrItem.GetSize();
	if (!dwItemCnt) return 0;

	// sort
	BMSRECORD* pData = m_arrItem.GetData();
	//qsort(pData, dwItemCnt, sizeof(BMSRECORD), qsortproc_record);

	// create header
	// 68380
	DWORD dwOffset = sizeof(DWORD)+sizeof(BMSINDEX)*dwItemCnt;

	m_hHeader.count = dwItemCnt;
	if (m_hHeader.index) delete[] m_hHeader.index;
	m_hHeader.index = new BMSWINDEX[dwItemCnt];

	for (i=0; i<dwItemCnt; i++, pData++)
	{
		strcpy(m_hHeader.index[i].id, pData->id);
		m_hHeader.index[i].idx_id = 0xFFFFFFFF;
		m_hHeader.index[i].idx_rec = i;
		m_hHeader.index[i].offset = dwOffset;
		m_hHeader.index[i].size = pData->sizestr;

		dwOffset += pData->sizestr;
	}

	return dwItemCnt;
}

#define CEIL(var)	((var)-(int)(var) > 0 ? (int)(var+1) : (int)(var))

long CBMSWriter::SearchIndex(char* lpszId)
{
	if(m_hHeader.count <= 0) return -1;
	if(!lpszId || *lpszId == NULL) return -1;

	ASSERT(strlen(lpszId) < BMS_MAX_ID);

	BMSWINDEX* pIndex = m_hHeader.index;
	
	long lS = 0;
	long lM = m_hHeader.count/2;
	long lE = m_hHeader.count-1;

	long lRes;

	while (1)
	{
		lRes = strcmp(pIndex[lM].id, lpszId);
		if (lRes == 0) return lM;

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


/* 
     불필요한 문자 제거
 *	 szBuffer   [in/out]
 */
bool removeDumyChars(char *buffer)
{
	char szTemp[1024];

	char* pp = buffer;
	char* pDst = szTemp;
	int  state = 0;
	bool terminate = false;

	while(*pp && !terminate)
	{
		switch(state)
		{
		case 0 : 
			if(*pp == ' ' || *pp == '\r' || *pp == '\n')
			{
				pp++;
				continue;
			}
			state = 1;
		case 1 : 
			if(*pp == '/' && *(pp+1) && (*(pp+1) == '*' || *(pp+1) == '/'))
			{
				state = 2;  
				pp++;
			}
			else
			{
				*pDst = *pp;
				pDst++;
				pp++;
			}
			break;
		case 2 :
			if(*pp == '*')
			{
				state = 3;
				pp++;
			}
			else if(*pp == '/')
			{
				terminate = true;
				break;
			}
			break;
		case 3 : 
			if(*pp == '*')
			{
				state = 4;
			}
			pp++;
			break;
		case 4 :
			if(*pp == '/')
			{
				state = 1;
			}
			pp++;
		case 5 :
			pp++;
			break;
		}
	}

	*pDst = '\0';

	strcpy(buffer, szTemp);

	return true;
}

BOOL CBMSWriter::MergeIndex(char* lpszFilename)
{
	if (!lpszFilename || !lpszFilename[0]) return FALSE;

	BOOL bRetVal = FALSE;

	const long MAXHEADERLIST = 100;
	char arrHeaderList[MAXHEADERLIST][_MAX_PATH];
	long lHeaderCnt = 0;

	DWORD dwLen = strlen(lpszFilename);
	DWORD st, ed;
	char tFilename[_MAX_PATH];
		
	for (st=0; st<dwLen-1; st++)
	{
		if (lpszFilename[st] == ';' || lpszFilename[st] == ' ') continue;

		for (ed=st+1; ed<dwLen; ed++)
		{
			if (lpszFilename[ed] != ';') continue;

			strncpy(tFilename, lpszFilename+st, ed-st);
			tFilename[ed-st] = NULL;

			for (long i=(ed-st)-1; i >=0; i--)
			{
				if (tFilename[i] != ' ') break;
				tFilename[i] = NULL;
			}

			st = ed;

			strcpy(arrHeaderList[lHeaderCnt], tFilename);
			lHeaderCnt++;

			break;
		}

		if (lHeaderCnt >= MAXHEADERLIST) break;
	}

	qsort(m_hHeader.index, m_hHeader.count, sizeof(BMSWINDEX), qsortproc_record);

	while (lHeaderCnt > 0)
	{
		FILE* hFile = fopen(arrHeaderList[lHeaderCnt-1], "rt");
		if (hFile)
		{
			char szBuffer[_MAX_PATH];		// Reading buffer
			char* lpParameter;
			
			char szToken[_MAX_PATH];
			DWORD dwParam;
			CString strFilename;

			while(!feof(hFile))
			{
				// Reading buffer
				memset(szBuffer, 0x00, _MAX_PATH);
				fgets(szBuffer, _MAX_PATH, hFile);

				removeDumyChars(szBuffer);
			
				// 주석 처리
				if ((lpParameter = strstr(szBuffer, "//"))) *lpParameter = NULL;
			
				if ((lpParameter = strstr(szBuffer, "#define")))
				{
					lpParameter += strlen("#define");
					if (sscanf(lpParameter, "%s %d", szToken, &dwParam) < 0) continue;

					strFilename = szToken;
					strFilename.TrimLeft();
					strFilename.TrimRight();

					long lIndex = SearchIndex(strFilename.GetBuffer(strFilename.GetLength()));
					if (lIndex < 0) continue;

					if (m_hHeader.index[lIndex].idx_id != 0xFFFFFFFF)
					{
						ASSERT(0 && "ERROR");
						if (m_hHeader.index[lIndex].idx_id != dwParam)
						{
							char msg[256];
							sprintf(msg, "%s is has two values!", strFilename.GetBuffer(0));
							AfxGetMainWnd()->MessageBox(msg, "Header parsing Error!", MB_OK|MB_ICONERROR);
				
							fclose(hFile);
							return FALSE;
						}
						else
						{
							char msg[256];
							sprintf(msg, "%s is already exist!", strFilename.GetBuffer(0));
							AfxGetMainWnd()->MessageBox(msg, "Header parsing Error!", MB_OK|MB_ICONERROR);
						}
					}

					m_hHeader.index[lIndex].idx_id = dwParam;
				}
			}
			
			fclose(hFile);

			bRetVal = TRUE;
		}
		else
		{
			bRetVal = FALSE;
			break;
		}
	
		lHeaderCnt--;
	}

	if (bRetVal)
		qsort(m_hHeader.index, m_hHeader.count, sizeof(BMSWINDEX), qsortproc_index);

	return bRetVal;
}

BOOL CBMSWriter::BuildFile(char* lpszFilename)
{
	int i;
	if (!lpszFilename || !lpszFilename[0]) return FALSE;

	CFile file(lpszFilename, CFile::modeCreate|CFile::modeWrite);

	file.Write(&m_hHeader.count, sizeof(m_hHeader.count));
	for (i=0; i<m_hHeader.count; i++)
		file.Write(m_hHeader.index+i, sizeof(BMSINDEX));

	BMSRECORD* pData = m_arrItem.GetData();

	for (i=0; i<m_hHeader.count; i++, pData++)
	{
		DWORD pos = file.GetPosition();
		//ASSERT(m_hHeader.index[i].offset == pos);

		if (pData->sizestr > 0)
			file.Write(pData->str, pData->sizestr);
	}

	file.Close();

	return TRUE;
}

void CBMSWriter::MakeLogFile(char* lpszOutput, char* lpszMiddle)
{
	FILE* hIn = fopen(lpszOutput, "rb");
	FILE* hOut = fopen(lpszMiddle, "wb");

	BMSHEADER header;
	fread(&header.count, sizeof(header.count), 1, hIn);
	ASSERT(header.count == m_hHeader.count);
	header.index = new BMSINDEX[header.count];
	fread(header.index, sizeof(BMSINDEX), header.count, hIn);

	// .txt파일로 writing할 때만 사용하면 된다.
	WORD tag = 0xFEFF;
	fwrite(&tag, sizeof(tag), 1, hOut);

	wchar_t pBuffer[BMS_MAX_STRING];
	wchar_t pName[BMS_MAX_STRING];

	mbstowcs(pBuffer, lpszOutput, strlen(lpszOutput)+1);
	_wsplitpath(pBuffer, NULL, NULL, pName, NULL);
	swprintf(pBuffer, L"// %s\r\n", pName);
	fputws(pBuffer, hOut);
	swprintf(pBuffer, L"// Record Count: %d\r\n\r\n", header.count);
	fputws(pBuffer, hOut);

	for (DWORD i=0; i<m_hHeader.count; i++)
	{
		ASSERT(m_hHeader.index[i].offset == header.index[i].offset);

		mbstowcs(pBuffer, m_hHeader.index[i].id, strlen(m_hHeader.index[i].id)+1);
		fputws(pBuffer, hOut);

		fseek(hIn, header.index[i].offset, SEEK_SET);
		fread(pName, header.index[i].size, 1, hIn);

		for (DWORD j=5; j<30; j+=5)
			if (wcslen(pBuffer) < j)
				fputws(L"\t", hOut);
		fputws(L"\t\"", hOut);
		fputws(pName, hOut);
		fputws(L"\"\r\n", hOut);
	}

	delete[] header.index;

	fclose(hIn);
	fclose(hOut);
}

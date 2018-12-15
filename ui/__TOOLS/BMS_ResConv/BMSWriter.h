// BMSWriter.h: interface for the CBMSWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMSWRITER_H__FFE5CA52_2B1A_4E56_9D45_3B13C8EA86A0__INCLUDED_)
#define AFX_BMSWRITER_H__FFE5CA52_2B1A_4E56_9D45_3B13C8EA86A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BMSReader.h"
#include "afxtempl.h"

#define BMS_MAX_ID		48
#define BMS_MAX_STRING	512

struct BMSRECORD
{
	DWORD	sizeid;
	DWORD	sizestr;
	char	id[BMS_MAX_ID];
	wchar_t str[BMS_MAX_STRING];
};

struct BMSWINDEX
{
	DWORD idx_id;
	DWORD offset;
	DWORD size;

	char id[BMS_MAX_ID];
	DWORD idx_rec;
};

struct BMSWHEADER
{
	FILE* file;
	DWORD count;
	BMSWINDEX* index;
};

#define WORD_ALIGN_LENGTH(a)	(((a)+3)&0xfffffffc)

class CBMSWriter  
{
public:
	CBMSWriter();
	virtual ~CBMSWriter();

public:
	BMSWHEADER	m_hHeader;
	CArray<BMSRECORD, BMSRECORD> m_arrItem;

public:
	void Close();

	DWORD ReadFile(char* lpszFilename);
	BOOL MergeIndex(char* lpszFilename);
	BOOL BuildFile(char* lpszFilename);

	long SearchIndex(char* lpszId);

	void MakeLogFile(char* lpszOutput, char* lpszMiddle);
};

#endif // !defined(AFX_BMSWRITER_H__FFE5CA52_2B1A_4E56_9D45_3B13C8EA86A0__INCLUDED_)

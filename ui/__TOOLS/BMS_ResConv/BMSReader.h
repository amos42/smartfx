// BMSReader.h: interface for the CBMSReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMSREADER_H__6C46EA61_C9FC_4F3C_8683_50A7C7957DEA__INCLUDED_)
#define AFX_BMSREADER_H__6C46EA61_C9FC_4F3C_8683_50A7C7957DEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct BMSINDEX
{
	DWORD idx;
	DWORD offset;
	DWORD size;
};

struct BMSHEADER
{
	FILE* file;
	DWORD count;
	BMSINDEX* index;
};

class CBMSReader  
{
public:
	CBMSReader();
	virtual ~CBMSReader();

public:
	BMSHEADER m_hHeader;

protected:
	long SearchIndex(DWORD dwId);

public:
	BOOL Open(char* lpszFilename);
	void Close();
	BOOL IsOpened();

	BOOL GetString(DWORD dwId, wchar_t* lpszString, long lMaxLen);
};

#endif // !defined(AFX_BMSREADER_H__6C46EA61_C9FC_4F3C_8683_50A7C7957DEA__INCLUDED_)

#ifndef __TOKEN_STRING_FUNCTION_HEADER_TONY_FILE_H__
#define __TOKEN_STRING_FUNCTION_HEADER_TONY_FILE_H__


#include "AT_Types.h"


class xCToken
{

private:

	atINT		m_iMaxCnt;
	atINT		m_iCount ;
	atCHAR	m_cpNull[2];	// atNULL ����.
	atCHAR	m_cpDeli[2];	// ������ �ִ� 2������ ��밡��.
	atCHAR**	m_cppHead;		// ���� ����Ʈ.

public:
	xCToken ();
	~xCToken ();

	// ��� �޸� ����.
	atVOID Free ();

	// �����ڹ� �ִ� �׸� ������ �״�� �ΰ�, ���ڿ��� �ʱ�ȭ ��Ŵ
	atVOID Reset ();

	// ���� �����ڿ����ؼ� �������� �׸��� ������ ������.
	atINT	 Count();
	atINT	 size();

	// �־��� �ʵ尳���� �ִ� ������ �ʰ������� ���� ����Ʈ�� ���� alloc�Ѵ�.
	atBOOL Realloc (atINT _iNewMax);

	// �ʱ�ȭ�ϱ�
	// iMax: �и��� �ʵ��� �ִ밳���� ����Ѵ�.
	// cDelimitor : ������
	atINT InitMaxCnt(atINT _iMax, atCHAR _cDelimitor = ',', atCHAR _cDelimitor2 = -1); 
	
	// �����ڷ� ���ڿ� ������.
	// ���е� �ʵ� ������ �ǵ���.
	atINT Splite (atCHAR* _cpStr);

	// zero base iNth��° ���ڿ����� �����ش�.
	// ���α׷� ���ǻ� NULL���� �������� �ʴ´�.
	atCHAR* Get (atINT _iNth);

	atVOID operator= (xCToken & _oTkn);
};


//////////////////////////////////////////////////////////////////////////
//  ��뿹��.
//
//	xCToken token;
//
//	token.InitMaxCnt(20);
//	int iCnt = token.Splite("234,aaa, 4344, 453, abcd,,rrr, \"TONY KIM Made Navigation\", dir,");
//	
//	TRACE("TOKEN Count(%d,%d)\n", iCnt, token.Count());
//
//	for (int kk=0; kk<iCnt;kk++)
//	{
//		char* cpStr = token.Get(kk);
//		TRACE("TOKEN(%d): %s\n", kk, cpStr);
//	}
//
// ���
//TOKEN Count(10,10)
//TOKEN(0): 234
//TOKEN(1): aaa
//TOKEN(2):  4344
//TOKEN(3):  453
//TOKEN(4):  abcd
//TOKEN(5): 
//TOKEN(6): rrr
//TOKEN(7): TONY KIM Made Navigation
//TOKEN(8):  dir
//TOKEN(9): 
//////////////////////////////////////////////////////////////////////////


#endif

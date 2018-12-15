#ifndef __TOKEN_STRING_FUNCTION_HEADER_TONY_FILE_H__
#define __TOKEN_STRING_FUNCTION_HEADER_TONY_FILE_H__


#include "AT_Types.h"


class xCToken
{

private:

	atINT		m_iMaxCnt;
	atINT		m_iCount ;
	atCHAR	m_cpNull[2];	// atNULL 문자.
	atCHAR	m_cpDeli[2];	// 구분자 최대 2개까지 사용가능.
	atCHAR**	m_cppHead;		// 버퍼 포인트.

public:
	xCToken ();
	~xCToken ();

	// 모든 메모리 해제.
	atVOID Free ();

	// 구분자및 최대 항목 개수는 그대로 두고, 문자열만 초기화 시킴
	atVOID Reset ();

	// 실제 구분자에의해서 나뉘어진 항목의 개수를 돌려줌.
	atINT	 Count();
	atINT	 size();

	// 주어진 필드개수의 최대 개수를 초과했을때 버퍼 포인트를 새로 alloc한다.
	atBOOL Realloc (atINT _iNewMax);

	// 초기화하기
	// iMax: 분리될 필드의 최대개수를 사용한다.
	// cDelimitor : 구분자
	atINT InitMaxCnt(atINT _iMax, atCHAR _cDelimitor = ',', atCHAR _cDelimitor2 = -1); 
	
	// 구분자로 문자열 나누기.
	// 구분된 필드 개수를 되돌림.
	atINT Splite (atCHAR* _cpStr);

	// zero base iNth번째 문자열값을 돌려준다.
	// 프로그램 편의상 NULL값을 리턴하지 않는다.
	atCHAR* Get (atINT _iNth);

	atVOID operator= (xCToken & _oTkn);
};


//////////////////////////////////////////////////////////////////////////
//  사용예제.
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
// 결과
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

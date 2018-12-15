#include "_token.h"
#include "_heap.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////
xCToken::xCToken()
{
	m_iMaxCnt = 0;
	m_iCount  = 0;
	m_cpNull[0] = atNULL;	// atNULL 문자.
	m_cpDeli[0] = ',';	// 구분자 최대 2개까지 사용가능.
	m_cpDeli[1] = ',';	// 구분자 최대 2개까지 사용가능.
	m_cppHead = atNULL;		// 버퍼 포인트.
}
xCToken::~xCToken ()
{
	Free ();
}


// 구분자및 최대 항목 개수는 그대로 두고, 문자열만 초기화 시킴
atVOID xCToken::Reset ()
{
	if (!m_cppHead)
	{
//		ASS_TEMP_ERT(0); // InitMaxCnt() 함수를 호출해서 초기화를 해주세요. ^^
		return;
	}

	for (atINT nn=0; nn<m_iMaxCnt; nn++)
	{
		if (m_cppHead[nn])
			NMEA_free(m_cppHead[nn]);
		m_cppHead[nn] = atNULL;
	}
	m_iCount = 0;

	//PublicHeapCompact();
}

atVOID xCToken::Free ()
{
	if (m_cppHead)
	{
		for (atINT nn=0; nn<m_iMaxCnt; nn++)
		{
			if (m_cppHead[nn])
				NMEA_free(m_cppHead[nn]);
			m_cppHead[nn] = atNULL;
		}

		NMEA_free(m_cppHead);
		m_cppHead = atNULL;
		m_iCount  = 0;
		m_iMaxCnt = 0;

		//PublicHeapCompact();
	}
}

// 초기화하기
// iMax: 분리될 필드의 최대개수를 사용한다.
// cDelimitor : 구분자
atINT xCToken::InitMaxCnt(atINT _iMax, atCHAR _cDelimitor, atCHAR _cDelimitor2)
{
	if (_iMax<1)
		return 0;

	Free ();

	m_cppHead = (atCHAR**)NMEA_alloc(sizeof(atCHAR*)*_iMax);
	if (!m_cppHead) return 0;

	for (atINT nn=0; nn<_iMax; nn++)
		m_cppHead[nn] = atNULL;

	m_cpDeli[0] = _cDelimitor;
	if (_cDelimitor2<0)
		m_cpDeli[1] = _cDelimitor;
	else
		m_cpDeli[1] = _cDelimitor2;

	m_iMaxCnt = _iMax;
	return m_iMaxCnt;
}

atBOOL xCToken::Realloc (atINT _iNewMax)
{
	if (_iNewMax <= m_iMaxCnt)
		return atTRUE;

	atCHAR**	cppHeadNew;		// 새로운 버퍼 포인트.
	cppHeadNew = (atCHAR**)NMEA_alloc(sizeof(atCHAR*)*_iNewMax);
	if (!cppHeadNew) return atFALSE;

	atINT nn;
	for (nn=0; nn<m_iMaxCnt; nn++)
		cppHeadNew[nn] = m_cppHead[nn];
	for (;nn<_iNewMax; nn++)
		cppHeadNew[nn] = atNULL;

	NMEA_free(m_cppHead);
	m_cppHead = cppHeadNew;
	m_iMaxCnt = _iNewMax;

	return atTRUE;
}


// 구분자로 문자열 나누기.
// 구분된 필드 개수를 되돌림.
atINT xCToken::Splite (atCHAR* _cpStr)
{
	Reset ();

	atCHAR* cpF = _cpStr;

	atINT xx = 0;
	atINT yy = 0;
	atINT iOpenFlag = 0;
	atCHAR cpBuff[2048];

	atINT iLen = strlen (cpF);

	// 구분자 중에 공백이 있는가?
	atINT bDeliHasSpace = (m_cpDeli[0] == ' ' || m_cpDeli[1] == ' ');

	for (atINT nn=0; nn<iLen; nn++)
	{
		if ( ((*cpF == m_cpDeli[0]) || (*cpF == m_cpDeli[1]) ) && (iOpenFlag == 0))
		{
			// 연속된 구분자는 공백일 경우에는 무시한다.
			if( bDeliHasSpace )
			{
				if( nn==0 || (nn && cpF[-1]==' ') )
				{
					cpF++;
					continue;
				}
			}

			if (m_iMaxCnt<yy+1)
				Realloc (m_iMaxCnt + 10);

			cpBuff[xx++] = 0;
			m_cppHead[yy] = (atCHAR*)NMEA_alloc(xx+2);
			strncpy (m_cppHead[yy], cpBuff, xx+2);
			yy++;

			if (nn && nn == iLen-1)
			{
				if (m_iMaxCnt<yy+1)
					Realloc (m_iMaxCnt + 10);

				m_cppHead[yy] = (atCHAR*)NMEA_alloc(2);
				m_cppHead[yy][0] = atNULL;
				yy++;
			}

			xx = 0;
			iOpenFlag = 0;
		}
		else if ( *cpF == '"')
		{
			if (iOpenFlag)
			{
				iOpenFlag = 0;

				// 쌍따옴표가 닫히면 뒤의 문자열은 구분자가 나올대까지 무시한다. (NULL에 의해서.)
				cpBuff[xx++] = atNULL;
			}
			else
			{
				xx = 0;
				iOpenFlag = 1;
			}

			if (nn == iLen-1)
			{
				if (m_iMaxCnt<yy+1)
					Realloc (m_iMaxCnt + 10);

				m_cppHead[yy] = (atCHAR*)NMEA_alloc(xx+2);
				strncpy (m_cppHead[yy], cpBuff, xx+2);
				yy++;
				xx = 0;
			}
		}
		else
		{
			cpBuff[xx+1] = atNULL;
			cpBuff[xx++] = *cpF;

			if (nn == iLen-1)
			{
				if (m_iMaxCnt<yy+1)
					Realloc (m_iMaxCnt + 10);

				m_cppHead[yy] = (atCHAR*)NMEA_alloc(xx+2);
				strncpy (m_cppHead[yy], cpBuff, xx+2);
				yy++;
				xx = 0;
			}
		}
		cpF++;
	}

	// 마지막에 공백 분리자만 있을 경우 마지막 것은 무시한다.
	if (yy && bDeliHasSpace && m_cppHead[yy-1][0] == 0)
		yy--;

	m_iCount = yy;
	return yy;
}

atCHAR* xCToken::Get (atINT _iNth)
{
	if (m_iCount <=_iNth)
		return m_cpNull;

	if (_iNth<0)
		return m_cpNull;

	return m_cppHead[_iNth];
}

// 실제 구분자에의해서 나뉘어진 항목의 개수를 돌려줌.
atINT xCToken::Count()
{
	return m_iCount;
}

atINT xCToken::size()
{
	return m_iCount;
}

atVOID xCToken::operator= (xCToken & _oTkn)
{
	Free ();

	m_iCount = _oTkn.Count();
	m_iMaxCnt = _oTkn.m_iMaxCnt;
	
	m_cppHead = (atCHAR**)NMEA_alloc(sizeof(atCHAR*)*m_iMaxCnt);
	if (!m_cppHead) return;
	memset (m_cppHead, 0x00, sizeof(atCHAR*)*m_iMaxCnt);

	for (atINT nn=0; nn<m_iMaxCnt; nn++)
	{
		m_cppHead[nn] = atNULL;

		atCHAR* cpStr = _oTkn.Get (nn);
		if (cpStr == atNULL)
			continue;

		atINT iLen = strlen (cpStr);
		if (iLen)
		{
			m_cppHead[nn] = (atCHAR*)NMEA_alloc(iLen+1);
			memcpy (m_cppHead[nn], cpStr, iLen+1); // null 문자 포함.
		}
	}
}

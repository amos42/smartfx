#include "_token.h"
#include "_heap.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////
xCToken::xCToken()
{
	m_iMaxCnt = 0;
	m_iCount  = 0;
	m_cpNull[0] = atNULL;	// atNULL ����.
	m_cpDeli[0] = ',';	// ������ �ִ� 2������ ��밡��.
	m_cpDeli[1] = ',';	// ������ �ִ� 2������ ��밡��.
	m_cppHead = atNULL;		// ���� ����Ʈ.
}
xCToken::~xCToken ()
{
	Free ();
}


// �����ڹ� �ִ� �׸� ������ �״�� �ΰ�, ���ڿ��� �ʱ�ȭ ��Ŵ
atVOID xCToken::Reset ()
{
	if (!m_cppHead)
	{
//		ASS_TEMP_ERT(0); // InitMaxCnt() �Լ��� ȣ���ؼ� �ʱ�ȭ�� ���ּ���. ^^
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

// �ʱ�ȭ�ϱ�
// iMax: �и��� �ʵ��� �ִ밳���� ����Ѵ�.
// cDelimitor : ������
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

	atCHAR**	cppHeadNew;		// ���ο� ���� ����Ʈ.
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


// �����ڷ� ���ڿ� ������.
// ���е� �ʵ� ������ �ǵ���.
atINT xCToken::Splite (atCHAR* _cpStr)
{
	Reset ();

	atCHAR* cpF = _cpStr;

	atINT xx = 0;
	atINT yy = 0;
	atINT iOpenFlag = 0;
	atCHAR cpBuff[2048];

	atINT iLen = strlen (cpF);

	// ������ �߿� ������ �ִ°�?
	atINT bDeliHasSpace = (m_cpDeli[0] == ' ' || m_cpDeli[1] == ' ');

	for (atINT nn=0; nn<iLen; nn++)
	{
		if ( ((*cpF == m_cpDeli[0]) || (*cpF == m_cpDeli[1]) ) && (iOpenFlag == 0))
		{
			// ���ӵ� �����ڴ� ������ ��쿡�� �����Ѵ�.
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

				// �ֵ���ǥ�� ������ ���� ���ڿ��� �����ڰ� ���ô���� �����Ѵ�. (NULL�� ���ؼ�.)
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

	// �������� ���� �и��ڸ� ���� ��� ������ ���� �����Ѵ�.
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

// ���� �����ڿ����ؼ� �������� �׸��� ������ ������.
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
			memcpy (m_cppHead[nn], cpStr, iLen+1); // null ���� ����.
		}
	}
}

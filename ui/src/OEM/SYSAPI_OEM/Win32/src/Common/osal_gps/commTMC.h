
#if !defined (____COMM_TMC_H____) 
#define		____COMM_TMC_H____	

#include "comm.h"

class CCommTMC : public CComm
{
public:
	CCommTMC ( void ) ;
	virtual  ~CCommTMC( void ) ;

public:
	virtual BOOL process(const char* szData, UINT len);
	virtual BOOL parse(const char* _msg, UINT _nLen);
	virtual BOOL openComm(UINT port, UINT baudRate);
	virtual BOOL closeComm(void);
};

#endif	// ____COMM_TMC_H____

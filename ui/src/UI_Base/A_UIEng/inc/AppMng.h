/** 
  @file AppMng.h
  @brief Application Manager�� ���� API
  @remark AppMng.c�� �������̽� ���
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see AppMng.c App.h
*/
#ifndef __APPMNG_H_
#define __APPMNG_H_

#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif
#include "A_UIENG.h"
#include "App.h"


#ifdef __cplusplus
extern "C" {
#endif


atHAPP 		atAPPMNG_GetCurApp(void);
atHAPP 		atAPPMNG_SetCurApp( atHAPP hApp );
atVOID		atAPPMNG_Exit( atINT nExitCode );


#ifdef __cplusplus
}
#endif


#endif // __APPMNG_H_


/** 
  @file AppMng.h
  @brief Application Manager에 관한 API
  @remark AppMng.c의 인터페이스 헤더
  @author Amos
  @date 2010-04-19 오전 11:41:00
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


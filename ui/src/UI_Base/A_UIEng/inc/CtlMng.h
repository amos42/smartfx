/** 
  @file CtlMng.h
  @brief Control Manager에 관한 API
  @remark CtlMng.c의 인터페이스
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see CtlMng.h CtlMng.c Control.h WinObj.h WObjMng.h
*/
#ifndef __CTLMNG_H_
#define __CTLMNG_H_


#include "UI_Types.h"
#include "Control.h"
#include "WObjMng.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atHANDLE  atHCTLMNG;			/**< Control Manager의 핸들 타입 */


/**
 @brief Control Manager 개체
*/
typedef struct _tagatCTLMNG {
	atWINOBJMNG	    Base;						/**< WinMng, CtlMng 등의 기본 속성. 반드시 맨 앞에 있어야 한다. */
} atCTLMNG, *atLPCTLMNG;


#ifdef __cplusplus
}
#endif


/** 
 @include CtlMng.inl 
*/
#include "CtlMng.inl"


#endif


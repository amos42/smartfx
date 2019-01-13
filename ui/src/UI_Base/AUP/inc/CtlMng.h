/** 
  @file CtlMng.h
  @brief Control Manager�� ���� API
  @remark CtlMng.c�� �������̽�
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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


typedef atHANDLE  atHCTLMNG;			/**< Control Manager�� �ڵ� Ÿ�� */


/**
 @brief Control Manager ��ü
*/
typedef struct _tagatCTLMNG {
	atWINOBJMNG	    Base;						/**< WinMng, CtlMng ���� �⺻ �Ӽ�. �ݵ�� �� �տ� �־�� �Ѵ�. */
} atCTLMNG, *atLPCTLMNG;


#ifdef __cplusplus
}
#endif


/** 
 @include CtlMng.inl 
*/
#include "CtlMng.inl"


#endif


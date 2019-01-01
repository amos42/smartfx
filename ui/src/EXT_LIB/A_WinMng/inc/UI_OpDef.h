/**
  @file UI_OpDef.h
  @brief UI엔진에서 사용하는 매크로 연산자들 정의
  @remark UI엔진에서 사용하는 매크로 연산자들 정의
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see UI_Types.h
*/
#ifndef __UI_OPDEF_H_
#define __UI_OPDEF_H_


#include "UI_Types.h"


#ifdef __cplusplus
  extern "C" {
#endif


#define RESET_LAYOUTBITMAP(a) atUIAPI_memset(&(a), 0, sizeof(atLAYOUTBITMAP))



#ifdef __cplusplus
}
#endif


#endif

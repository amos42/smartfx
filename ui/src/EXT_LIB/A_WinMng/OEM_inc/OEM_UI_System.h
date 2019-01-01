/**
  @file OEM_UI_System.h
  @brief System�� ���� ���� OEM API ����
  @remark System�� ���� ���� OEM API ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
  @see UI_SysUtil.h
*/
#ifndef __OEM_UI_SYSTEM_H_
#define __OEM_UI_SYSTEM_H_

#include "UI_Types.h"
#include "UI_API/UI_API.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef atLONG	FN_OEM_UI_Initialize( atUIAPI_INIT_DATA *lpInitData );
typedef void	FN_OEM_UI_Finalize(void);


typedef struct tagOEM_UI_SYSTEM {
	FN_OEM_UI_Initialize		*fnInitialize;
	FN_OEM_UI_Finalize			*fnFinalize;
} OEM_UI_SYSTEM_T;


atVOID	OEM_UI_System_SetOemFn( OEM_UI_SYSTEM_T *fnOem );
atVOID	OEM_UI_System_GetOemFn( OEM_UI_SYSTEM_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif // __OEM_UI_SYSTEM_H_

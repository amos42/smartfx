#ifndef __OEM_SA_KEY_H_
#define __OEM_SA_KEY_H_


#include "SA_Types.h"
#include "SA_Key.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef int		FN_OEM_SYSAPI_InitKeypad( atLONG lSysInst, atLONG lInitData1, atLONG lInitData2 );
typedef void	FN_OEM_SYSAPI_FinalKeypad(void);

typedef void	FN_OEM_SYSAPI_SetKeypadNotifyFunc( SYS_FUNC_KEYPADNOTIFY *fnInputFunc );

typedef int		FN_OEM_SYSAPI_ConvOEMKey2VKey( int OEMKey );
typedef int		FN_OEM_SYSAPI_ConvVKey2OEMKey( int VKey );


typedef struct tagOEM_SYSAPI_KEY_T {
	FN_OEM_SYSAPI_InitKeypad			*fnInitKeypad;
	FN_OEM_SYSAPI_FinalKeypad			*fnFinalKeypad;

	FN_OEM_SYSAPI_SetKeypadNotifyFunc	*fnSetKeypadNotifyFunc;

	FN_OEM_SYSAPI_ConvOEMKey2VKey		*fnConvOEMKey2VKey;
	FN_OEM_SYSAPI_ConvVKey2OEMKey		*fnConvVKey2OEMKey;
} OEM_SYSAPI_KEY_T;


atVOID	OEM_SYSAPI_KEY_SetOemFn( OEM_SYSAPI_KEY_T *fnOem );
atVOID	OEM_SYSAPI_KEY_GetOemFn( OEM_SYSAPI_KEY_T *fnOem );


#ifdef __cplusplus
}
#endif


#endif //  _KEYPAD_H_INCLUDED

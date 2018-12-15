//============================================================================
//
// Header  : PlatformDefine.h
// Function: Platform Definition
//
//

#ifndef _PLATFORM_DEFINE_H_
#define _PLATFORM_DEFINE_H_

#ifndef _CURRENT_PLATFORM_

	#define _PC_PLATFORM_			1
	#define _WINCE_PLATFORM_		2
	#define _SYMBIAN_PLATFORM_		3
	#define _WINMOBILE_PLATFORM_	4
	// Platform 확장시 추가...

	
	#ifdef _WIN32_WCE
		#define _CURRENT_PLATFORM_	_WINCE_PLATFORM_
	#elif defined(__SYMBIAN32__)
		#define _CURRENT_PLATFORM_	_SYMBIAN_PLATFORM_
	#elif defined(WIN32_PLATFORM_PSPC)
//	#elif (WIN32_PLATFORM_PSPC >= 400)
		#define _CURRENT_PLATFORM_	_WINMOBILE_PLATFORM_
	#elif defined(_WIN32)
		#define _CURRENT_PLATFORM_	_PC_PLATFORM_
	#else
		UNDEFINED_PLATFORM
	#endif 

		
	///////////////////////////////////////////////////////////////
	// 실제로 사용할 플랫폼 식별용 Macro
	///////////////////////////////////////////////////////////////
	#define _IS_PC_			(_CURRENT_PLATFORM_ == _PC_PLATFORM_)
	#define _IS_WINCE_		(_CURRENT_PLATFORM_ == _WINCE_PLATFORM_)
	#define _IS_SYMBIAN_	(_CURRENT_PLATFORM_ == _SYMBIAN_PLATFORM_)
	#define _IS_WINMOBILE_	(_CURRENT_PLATFORM_ == _WINMOBILE_PLATFORM_)
		
	#define _IS_WIN32_		( _IS_PC_ || _IS_WINCE_ || _IS_WINMOBILE_ )

#endif 

#endif //_PLATFORM_DEFINE_H_

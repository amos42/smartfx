/*
 ============================================================================
 Name		: TornadoS.pan
 Author	  : 
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __TORNADOS_PAN__
#define __TORNADOS_PAN__

/** TornadoS application panic codes */
enum TTornadoSPanics
	{
	ETornadoSUi = 1
	// add further panics here
	};

inline void Panic(TTornadoSPanics aReason)
	{
	_LIT(applicationName, "TornadoS");
	User::Panic(applicationName, aReason);
	}

#endif // __TORNADOS_PAN__

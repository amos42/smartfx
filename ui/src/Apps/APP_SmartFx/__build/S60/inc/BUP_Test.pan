/*
 ============================================================================
 Name		: BUP_Test.pan
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __BUP_TEST_PAN__
#define __BUP_TEST_PAN__

/** BUP_Test application panic codes */
enum TBUP_TestPanics
	{
	EBUP_TestUi = 1
	// add further panics here
	};

inline void Panic(TBUP_TestPanics aReason)
	{
	_LIT(applicationName, "BUP_Test");
	User::Panic(applicationName, aReason);
	}

#endif // __BUP_TEST_PAN__

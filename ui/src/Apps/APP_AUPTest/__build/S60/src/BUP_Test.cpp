/*
 ============================================================================
 Name		: BUP_Test.cpp
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "BUP_TestApplication.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CBUP_TestApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}


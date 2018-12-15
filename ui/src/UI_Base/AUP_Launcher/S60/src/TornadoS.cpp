/*
 ============================================================================
 Name		: TornadoS.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "TornadoSApplication.h"

LOCAL_C CApaApplication* NewApplication()
{
	return new CTornadoSApplication;
}

GLDEF_C TInt E32Main()
{
	return EikStart::RunApplication( NewApplication );
}


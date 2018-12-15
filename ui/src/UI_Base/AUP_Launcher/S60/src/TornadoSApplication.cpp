/*
 ============================================================================
 Name		: TornadoSApplication.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "TornadoS.hrh"
#include "TornadoSDocument.h"
#include "TornadoSApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTornadoSApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CTornadoSApplication::CreateDocumentL()
	{
	// Create an TornadoS document, and return a pointer to it
	return CTornadoSDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CTornadoSApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CTornadoSApplication::AppDllUid() const
	{
	// Return the UID for the TornadoS application
	return KUidTornadoSApp;
	}

// End of File

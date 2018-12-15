/*
 ============================================================================
 Name		: BUP_TestApplication.cpp
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "BUP_Test.hrh"
#include "BUP_TestDocument.h"
#include "BUP_TestApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBUP_TestApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CBUP_TestApplication::CreateDocumentL()
	{
	// Create an BUP_Test document, and return a pointer to it
	return CBUP_TestDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CBUP_TestApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CBUP_TestApplication::AppDllUid() const
	{
	// Return the UID for the BUP_Test application
	return KUidBUP_TestApp;
	}

// End of File

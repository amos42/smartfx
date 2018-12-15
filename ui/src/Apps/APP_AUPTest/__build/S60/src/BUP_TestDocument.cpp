/*
 ============================================================================
 Name		: BUP_TestDocument.cpp
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : CBUP_TestDocument implementation
 ============================================================================
 */

// INCLUDE FILES
#include "BUP_TestAppUi.h"
#include "BUP_TestDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBUP_TestDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBUP_TestDocument* CBUP_TestDocument::NewL(CEikApplication& aApp)
	{
	CBUP_TestDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CBUP_TestDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBUP_TestDocument* CBUP_TestDocument::NewLC(CEikApplication& aApp)
	{
	CBUP_TestDocument* self = new ( ELeave ) CBUP_TestDocument( aApp );

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CBUP_TestDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBUP_TestDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CBUP_TestDocument::CBUP_TestDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CBUP_TestDocument::CBUP_TestDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CBUP_TestDocument::~CBUP_TestDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CBUP_TestDocument::~CBUP_TestDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CBUP_TestDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CBUP_TestDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new ( ELeave )CBUP_TestAppUi;
	}

// End of File

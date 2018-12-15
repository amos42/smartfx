/*
 ============================================================================
 Name		: TornadoSDocument.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : CTornadoSDocument implementation
 ============================================================================
 */

// INCLUDE FILES
#include "TornadoSAppUi.h"
#include "TornadoSDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTornadoSDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTornadoSDocument* CTornadoSDocument::NewL(CEikApplication& aApp)
	{
	CTornadoSDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CTornadoSDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTornadoSDocument* CTornadoSDocument::NewLC(CEikApplication& aApp)
	{
	CTornadoSDocument* self = new ( ELeave ) CTornadoSDocument( aApp );

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CTornadoSDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTornadoSDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CTornadoSDocument::CTornadoSDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTornadoSDocument::CTornadoSDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CTornadoSDocument::~CTornadoSDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CTornadoSDocument::~CTornadoSDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CTornadoSDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CTornadoSDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new ( ELeave )CTornadoSAppUi;
	}

// End of File

/*
 ============================================================================
 Name		: BUP_TestAppView.cpp
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Application view implementation
 ============================================================================
 */

// INCLUDE FILES
#include <coemain.h>
#include "BUP_TestAppView.h"

#include "bup.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBUP_TestAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBUP_TestAppView* CBUP_TestAppView::NewL(const TRect& aRect)
	{
	CBUP_TestAppView* self = CBUP_TestAppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CBUP_TestAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBUP_TestAppView* CBUP_TestAppView::NewLC(const TRect& aRect)
	{
	CBUP_TestAppView* self = new ( ELeave ) CBUP_TestAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

// -----------------------------------------------------------------------------
// CBUP_TestAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBUP_TestAppView::ConstructL(const TRect& aRect)
	{
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	}

// -----------------------------------------------------------------------------
// CBUP_TestAppView::CBUP_TestAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CBUP_TestAppView::CBUP_TestAppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CBUP_TestAppView::~CBUP_TestAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CBUP_TestAppView::~CBUP_TestAppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CBUP_TestAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CBUP_TestAppView::Draw(const TRect& aRect) const
	{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
//	TRect drawRect(Rect());

	// Clears the screen
//	gc.Clear(drawRect);

	atUIAPI_ExtGraphicMethod(atNULL, 100, (long)&gc, (long)aRect.iTl.iX, (long)aRect.iTl.iY, (long)aRect.Width(), (long)aRect.Height(), 0);

	}

// -----------------------------------------------------------------------------
// CBUP_TestAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CBUP_TestAppView::SizeChanged()
	{
	TRect aRect = Rect();

	atUIAPI_FlushScreenEnable(atFALSE);
	atUIENG_OnResize( aRect.Width(), aRect.Height() );	
	atUIAPI_FlushScreenEnable(atTRUE);
	
	DrawNow();
	
	CCoeControl::SizeChanged();	
	}

// Called by framework when focus losted
void CBUP_TestAppView::PrepareForFocusLossL()
{
	atUIENG_OnPause();
}

// Called by framework when focus gained
void CBUP_TestAppView::PrepareForFocusGainL()
{
	atUIENG_OnResume();
}
// End of File

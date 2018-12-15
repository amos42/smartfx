/*
 ============================================================================
 Name		: TornadoSAppView.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Application view implementation
 ============================================================================
 */

// INCLUDE FILES
#include <coemain.h>
#include "TornadoSAppView.h"
//#include <TornadoS_0xEBB22191.rsg>

#include "UIShotDef.h"
#include "bup.h"

//#include "A_UIENG.h"
//#include "OEM_UI_GDI.h"

#include "MainWnd.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTornadoSAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTornadoSAppView* CTornadoSAppView::NewL(const TRect& aRect)
{
	CTornadoSAppView* self = CTornadoSAppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
}

// -----------------------------------------------------------------------------
// CTornadoSAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTornadoSAppView* CTornadoSAppView::NewLC(const TRect& aRect)
{
	CTornadoSAppView* self = new ( ELeave ) CTornadoSAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
}

// -----------------------------------------------------------------------------
// CTornadoSAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTornadoSAppView::ConstructL(const TRect& aRect)
{
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
}

// -----------------------------------------------------------------------------
// CTornadoSAppView::CTornadoSAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTornadoSAppView::CTornadoSAppView()
{
	// No implementation required
}

// -----------------------------------------------------------------------------
// CTornadoSAppView::~CTornadoSAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CTornadoSAppView::~CTornadoSAppView()
{
	// No implementation required
}

// -----------------------------------------------------------------------------
// CTornadoSAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CTornadoSAppView::Draw(const TRect& aRect) const
{
	extern CMainWnd* g_pMainWnd;

	//if (g_pMainWnd && g_pMainWnd->GetMainLogicStatus() < EMainLogicStandbyState)
	{
		// Get the standard graphics context
		CWindowGc& gc = SystemGc();
		atUIAPI_ExtGraphicMethod(atNULL, 100, (long)&gc, (long)aRect.iTl.iX, (long)aRect.iTl.iY, (long)aRect.Width(), (long)aRect.Height(), 0);
	}
}

// -----------------------------------------------------------------------------
// CTornadoSAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CTornadoSAppView::SizeChanged()
{
	TRect aRect = Rect();

	atUIAPI_FlushScreenEnable(atFALSE);
	atUIENG_OnResize( aRect.Width(), aRect.Height() );	
	atUIAPI_FlushScreenEnable(atTRUE);
	
	DrawNow();
	
	CCoeControl::SizeChanged();	
}

// Called by framework when focus losted
void CTornadoSAppView::PrepareForFocusLossL()
{
	atUIENG_OnPause();
}

// Called by framework when focus gained
void CTornadoSAppView::PrepareForFocusGainL()
{
	atUIENG_OnResume();
}

// End of File

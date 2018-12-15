/*
 ============================================================================
 Name		: BUP_TestAppView.h
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __BUP_TESTAPPVIEW_h__
#define __BUP_TESTAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>

// CLASS DECLARATION
class CBUP_TestAppView : public CCoeControl
	{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a CBUP_TestAppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of CBUP_TestAppView.
	 */
	static CBUP_TestAppView* NewL(const TRect& aRect);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Create a CBUP_TestAppView object, which will draw itself
	 * to aRect.
	 * @param aRect Rectangle this view will be drawn to.
	 * @return A pointer to the created instance of CBUP_TestAppView.
	 */
	static CBUP_TestAppView* NewLC(const TRect& aRect);

	/**
	 * ~CBUP_TestAppView
	 * Virtual Destructor.
	 */
	virtual ~CBUP_TestAppView();

public:
	// Functions from base classes

	/**
	 * From CCoeControl, Draw
	 * Draw this CBUP_TestAppView to the screen.
	 * @param aRect the rectangle of this view that needs updating
	 */
	void Draw(const TRect& aRect) const;

	/**
	 * From CoeControl, SizeChanged.
	 * Called by framework when the view size is changed.
	 */
	virtual void SizeChanged();
	// Called by framework when focus losted
	virtual void PrepareForFocusLossL();
	// Called by framework when focus gained
	virtual void PrepareForFocusGainL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * CBUP_TestAppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
	 * CBUP_TestAppView.
	 * C++ default constructor.
	 */
	CBUP_TestAppView();

	};

#endif // __BUP_TESTAPPVIEW_h__
// End of File

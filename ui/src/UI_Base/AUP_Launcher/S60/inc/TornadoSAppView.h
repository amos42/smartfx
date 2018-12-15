/*
 ============================================================================
 Name		: TornadoSAppView.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __TORNADOSAPPVIEW_h__
#define __TORNADOSAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>

// CLASS DECLARATION
class CTornadoSAppView : public CCoeControl
	{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a CTornadoSAppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of CTornadoSAppView.
	 */
	static CTornadoSAppView* NewL(const TRect& aRect);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Create a CTornadoSAppView object, which will draw itself
	 * to aRect.
	 * @param aRect Rectangle this view will be drawn to.
	 * @return A pointer to the created instance of CTornadoSAppView.
	 */
	static CTornadoSAppView* NewLC(const TRect& aRect);

	/**
	 * ~CTornadoSAppView
	 * Virtual Destructor.
	 */
	virtual ~CTornadoSAppView();

public:
	// Functions from base classes

	/**
	 * From CCoeControl, Draw
	 * Draw this CTornadoSAppView to the screen.
	 * @param aRect the rectangle of this view that needs updating
	 */
	void Draw(const TRect& aRect) const;

	/**
	 * From CoeControl, SizeChanged.
	 * Called by framework when the view size is changed.
	 */
	virtual void SizeChanged();

	// Focus
	virtual void PrepareForFocusLossL();
	virtual void PrepareForFocusGainL();
	
private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * CTornadoSAppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
	 * CTornadoSAppView.
	 * C++ default constructor.
	 */
	CTornadoSAppView();

	};

#endif // __TORNADOSAPPVIEW_h__
// End of File

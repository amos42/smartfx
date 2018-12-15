/*
 ============================================================================
 Name		: BUP_TestAppUi.h
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __BUP_TESTAPPUI_h__
#define __BUP_TESTAPPUI_h__

// INCLUDES
#include <aknappui.h>

#include <remconcoreapitargetobserver.h>    // link against RemConCoreApi.lib
#include <remconcoreapitarget.h>            // and
#include <remconinterfaceselector.h>        // RemConInterfaceBase.lib

// FORWARD DECLARATIONS
class CBUP_TestAppView;

// CLASS DECLARATION
/**
 * CBUP_TestAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CBUP_TestAppUi : public CAknAppUi, public MRemConCoreApiTargetObserver
	{
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CBUP_TestAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CBUP_TestAppUi();

	/**
	 * ~CBUP_TestAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CBUP_TestAppUi();

private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);
	void HandleForegroundEventL( TBool aForeground );
	virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
	
	void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);
	
private:
	// Data
	CRemConInterfaceSelector* iInterfaceSelector;
	CRemConCoreApiTarget*     iCoreTarget;

	/**
	 * The application view
	 * Owned by CBUP_TestAppUi
	 */
	CBUP_TestAppView* iAppView;

	};

#endif // __BUP_TESTAPPUI_h__
// End of File

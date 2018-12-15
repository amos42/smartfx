/*
 ============================================================================
 Name		: TornadoSAppUi.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __TORNADOSAPPUI_h__
#define __TORNADOSAPPUI_h__

// INCLUDES
#include <aknappui.h>

#include <remconcoreapitargetobserver.h>    // link against RemConCoreApi.lib
#include <remconcoreapitarget.h>            // and
#include <remconinterfaceselector.h>        // RemConInterfaceBase.lib


// FORWARD DECLARATIONS
class CTornadoSAppView;

// CLASS DECLARATION
/**
 * CTornadoSAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CTornadoSAppUi : public CAknAppUi, public MRemConCoreApiTargetObserver
	{
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CTornadoSAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CTornadoSAppUi();

	/**
	 * ~CTornadoSAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CTornadoSAppUi();

private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();
	void HandleForegroundEventL( TBool aForeground );

    virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent,TEventCode aType );

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
//	CArrayFix<TCoeHelpContext>* HelpContextL() const;

	// From MRemConCoreApiTargetObserver
	void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);

private:
	// Data

	CRemConInterfaceSelector* iInterfaceSelector;
	CRemConCoreApiTarget*     iCoreTarget;
	
	/**
	 * The application view
	 * Owned by CTornadoSAppUi
	 */
	CTornadoSAppView* iAppView;

	};

#endif // __TORNADOSAPPUI_h__
// End of File

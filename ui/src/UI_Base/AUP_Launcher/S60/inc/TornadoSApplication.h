/*
 ============================================================================
 Name		: TornadoSApplication.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __TORNADOSAPPLICATION_H__
#define __TORNADOSAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "TornadoS.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidTornadoSApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CTornadoSApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CTornadoSApplication is the application part of the
 * AVKON application framework for the TornadoS example application.
 */
class CTornadoSApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidTornadoSApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CTornadoSDocument document object. The returned
	 * pointer in not owned by the CTornadoSApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __TORNADOSAPPLICATION_H__
// End of File

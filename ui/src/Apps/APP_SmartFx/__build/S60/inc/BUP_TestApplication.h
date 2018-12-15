/*
 ============================================================================
 Name		: BUP_TestApplication.h
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __BUP_TESTAPPLICATION_H__
#define __BUP_TESTAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "BUP_Test.hrh"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidBUP_TestApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CBUP_TestApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CBUP_TestApplication is the application part of the
 * AVKON application framework for the BUP_Test example application.
 */
class CBUP_TestApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidBUP_TestApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CBUP_TestDocument document object. The returned
	 * pointer in not owned by the CBUP_TestApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __BUP_TESTAPPLICATION_H__
// End of File

/*
 ============================================================================
 Name		: BUP_TestDocument.h
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __BUP_TESTDOCUMENT_h__
#define __BUP_TESTDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CBUP_TestAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CBUP_TestDocument application class.
 * An instance of class CBUP_TestDocument is the Document part of the
 * AVKON application framework for the BUP_Test example application.
 */
class CBUP_TestDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CBUP_TestDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CBUP_TestDocument.
	 */
	static CBUP_TestDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CBUP_TestDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CBUP_TestDocument.
	 */
	static CBUP_TestDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CBUP_TestDocument
	 * Virtual Destructor.
	 */
	virtual ~CBUP_TestDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CBUP_TestAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CBUP_TestDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CBUP_TestDocument(CEikApplication& aApp);

	};

#endif // __BUP_TESTDOCUMENT_h__
// End of File

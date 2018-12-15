/*
 ============================================================================
 Name		: TornadoSDocument.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __TORNADOSDOCUMENT_h__
#define __TORNADOSDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CTornadoSAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CTornadoSDocument application class.
 * An instance of class CTornadoSDocument is the Document part of the
 * AVKON application framework for the TornadoS example application.
 */
class CTornadoSDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CTornadoSDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CTornadoSDocument.
	 */
	static CTornadoSDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CTornadoSDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CTornadoSDocument.
	 */
	static CTornadoSDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CTornadoSDocument
	 * Virtual Destructor.
	 */
	virtual ~CTornadoSDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CTornadoSAppUi object and return a pointer to it.
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
	 * CTornadoSDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CTornadoSDocument(CEikApplication& aApp);

	};

#endif // __TORNADOSDOCUMENT_h__
// End of File

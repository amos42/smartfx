/*
 ============================================================================
 Name		: BUP_TestAppUi.cpp
 Author	  : Amos
 Copyright   : Your copyright notice
 Description : CBUP_TestAppUi implementation
 ============================================================================
 */

// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>

#include <BUP_Test_0xED86F74E.rsg>

#include "BUP_Test.hrh"
#include "BUP_Test.pan"
#include "BUP_TestApplication.h"
#include "BUP_TestAppUi.h"
#include "BUP_TestAppView.h"

#include "bup.h"
#include "app_entry.h"


_LIT(KFileName, "C:\\private\\ED86F74E\\BUP_Test.txt");
_LIT(KText, "BUP_Test");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CBUP_TestAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBUP_TestAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	iInterfaceSelector = CRemConInterfaceSelector::NewL();
	iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
	iInterfaceSelector->OpenTargetL();

	// Create view object
//	const TRect& aRect = ClientRect();
	const TRect& aRect = ApplicationRect();
	iAppView = CBUP_TestAppView::NewL(aRect);

	APP_Entry_Init((void*)this, (void*)iAppView, aRect.Width(), aRect.Height());

	APP_Entry_Enter();

	// Create a file to write the text to
	TInt err = CCoeEnv::Static()->FsSession().MkDirAll(KFileName);
	if ( (KErrNone != err) && (KErrAlreadyExists != err))
		{
		return;
		}

	RFile file;
	err = file.Replace(CCoeEnv::Static()->FsSession(), KFileName, EFileWrite);
	CleanupClosePushL(file);
	if (KErrNone != err)
		{
		CleanupStack::PopAndDestroy(1); // file
		return;
		}

	RFileWriteStream outputFileStream(file);
	CleanupClosePushL(outputFileStream);
	outputFileStream << KText;

	CleanupStack::PopAndDestroy(2); // outputFileStream, file
	
	}
// -----------------------------------------------------------------------------
// CBUP_TestAppUi::CBUP_TestAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CBUP_TestAppUi::CBUP_TestAppUi()
	{
	// No implementation required
	iInterfaceSelector = NULL;
	iCoreTarget = NULL;
	}

// -----------------------------------------------------------------------------
// CBUP_TestAppUi::~CBUP_TestAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CBUP_TestAppUi::~CBUP_TestAppUi()
	{
	APP_Entry_Final();
	
	if (iAppView)
		{
		delete iAppView;
		iAppView = NULL;
		}

	if( iInterfaceSelector )
	{
		delete iInterfaceSelector;
		iInterfaceSelector = NULL;
	}

	}

// -----------------------------------------------------------------------------
// CBUP_TestAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CBUP_TestAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
			atUIAPI_SystemOff(0);
			break;
/*			
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
			break;

		case ECommand1:
			{

			// Load a string from the resource file and display it
			HBufC* textResource = StringLoader::LoadLC(R_COMMAND1_TEXT);
			CAknInformationNote* informationNote;

			informationNote = new ( ELeave ) CAknInformationNote;

			// Show the information Note with
			// textResource loaded with StringLoader.
			informationNote->ExecuteLD( *textResource);

			// Pop HBuf from CleanUpStack and Destroy it.
			CleanupStack::PopAndDestroy(textResource);
			}
			break;
		case ECommand2:
			{
			RFile rFile;

			//Open file where the stream text is
			User::LeaveIfError(rFile.Open(CCoeEnv::Static()->FsSession(), KFileName, EFileStreamText));//EFileShareReadersOnly));// EFileStreamText));
			CleanupClosePushL(rFile);

			// copy stream from file to RFileStream object
			RFileReadStream inputFileStream(rFile);
			CleanupClosePushL(inputFileStream);

			// HBufC descriptor is created from the RFileStream object.
			HBufC* fileData = HBufC::NewLC(inputFileStream, 32);

			CAknInformationNote* informationNote;

			informationNote = new ( ELeave ) CAknInformationNote;
			// Show the information Note
			informationNote->ExecuteLD( *fileData);

			// Pop loaded resources from the cleanup stack
			CleanupStack::PopAndDestroy(3); // filedata, inputFileStream, rFile
			}
			break;
		case EHelp:
			{
			}
			break;
		case EAbout:
			{

			CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog();
			dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(); //title
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(); //msg
			dlg->RunLD();
			}
			break;
		default:
			Panic(EBUP_TestUi);
			break;
*/			
		}
	}

void CBUP_TestAppUi::HandleForegroundEventL( TBool aForeground )
{
	CEikAppUi::HandleForegroundEventL( aForeground );
	
	if(aForeground)
	{
		atUIENG_OnResume();
		if(iAppView)
		{
			iAppView->DrawNow();
			atAPP_RedrawAllWindow(atAPPMNG_GetCurApp());
			atUIAPI_FlushScreen();
		}
	}
	else
	{
		atUIENG_OnPause();
	}


}


TKeyResponse CBUP_TestAppUi::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
{
/*
#ifdef __WINSCW__

	if( aType == EEventKeyDown )
	{
		_LIT( KKeyInfo, "scan:%x, code:%x" );
		TBuf<32> szInfo;
		szInfo.Format( KKeyInfo, aKeyEvent.iScanCode, aKeyEvent.iCode );
		
		User::InfoPrint( szInfo );
	}
	
#endif
*/
	/*
	if( aType == EEventKeyDown ){
		switch( aKeyEvent.iScanCode ){
		case EStdKeyMenu:
		case EStdKeyUpArrow:
		case EStdKeyDownArrow:
		case EStdKeyLeftArrow:
		case EStdKeyRightArrow:
		case EStdKeyIncVolume:
		case EStdKeyDecVolume:
		case EStdKeyDevice3:
			atUIENG_OnKeyDown(aKeyEvent.iScanCode);
			return EKeyWasConsumed;
		}
	} else if( aType == EEventKeyUp ){
		switch( aKeyEvent.iScanCode ){
		case EStdKeyMenu:
		case EStdKeyUpArrow:
		case EStdKeyDownArrow:
		case EStdKeyLeftArrow:
		case EStdKeyRightArrow:
		case EStdKeyIncVolume:
		case EStdKeyDecVolume:
		case EStdKeyDevice3:
			atUIENG_OnKeyUp(aKeyEvent.iScanCode);
			return EKeyWasConsumed;
		}
	}
	
	return EKeyWasNotConsumed;
	*/
	
	
//	if( aType == EEventKey )
//	{
//		if( aKeyEvent.iScanCode )
//		{
//			/*
//			if( aKeyEvent.iScanCode == EStdKeyNo )
//			{
//				Exit();
//				return EKeyWasConsumed;
//			}
//			*/
//			atUIENG_OnKeyDown(aKeyEvent.iScanCode);
//		}
//	}
	
	if( aType == EEventKeyDown)
	{
		if( aKeyEvent.iScanCode )
		{
			if( aKeyEvent.iScanCode == EStdKeyNo )
			{
				Exit();
				return EKeyWasConsumed;
			}
			int vk = atUIAPI_ConvOEMKey2VKey(aKeyEvent.iScanCode);
			atUIENG_OnKeyDown(vk);
			return EKeyWasConsumed;
		}
	}
	else if (aType == EEventKeyUp)
	{
		if( aKeyEvent.iScanCode )
		{
			if( aKeyEvent.iScanCode == EStdKeyNo )
			{
				Exit();
				return EKeyWasConsumed;
			}
			int vk = atUIAPI_ConvOEMKey2VKey(aKeyEvent.iScanCode);
			atUIENG_OnKeyUp(vk);
			return EKeyWasConsumed;
		}
	}

	return EKeyWasConsumed;
	
	
}

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CBUP_TestAppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect( ApplicationRect() );
	}

// ----------------------------------------------------------------------------
// MrccatoCommand()
//
// Receives events (press/click/release) from the following buttons:
// ’Play/Pause’, ’Volume Up’, ’Volume Down’, ’Stop’, ’Rewind’, ’Forward’
//
// Note that when pressed and held, media keys do not automatically repeat the commands.
// If repeat functionality is required, it has to be implemented with (CPeriodic) timers. 
// Note that on most devices only volume keys can be used in 3rd party applications using this API. 
// ----------------------------------------------------------------------------

void CBUP_TestAppUi::MrccatoCommand( TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct )
{
    TRequestStatus status;
    
    switch( aOperationId )
    {
    case ERemConCoreApiPausePlayFunction:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonPress:
                // Play/Pause button pressed
                break;
            case ERemConCoreApiButtonRelease:
                // Play/Pause button released
                break;
            case ERemConCoreApiButtonClick:
                // Play/Pause button clicked
                break;
            default:
                // Play/Pause unknown action
                break;
            }                               
	        //Send the response back to Remcon server       
	        iCoreTarget->PausePlayFunctionResponse(status, KErrNone);
	        User::WaitForRequest(status);
	        break;
        }   
      
    case ERemConCoreApiStop:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                break;
            }
	        iCoreTarget->StopResponse(status, KErrNone);
	        User::WaitForRequest(status);
	        break;
        }
    case ERemConCoreApiRewind:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                break;
            }
	        iCoreTarget->RewindResponse(status, KErrNone);
	        User::WaitForRequest(status);   
	        break;
        }       
    case ERemConCoreApiForward:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                break;
            }
	        iCoreTarget->ForwardResponse(status, KErrNone);
	        User::WaitForRequest(status);
	        break; 
        }
        
        
    case ERemConCoreApiVolumeUp:
        {   
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                //atUIENG_OnKeyUp(EStdKeyIncVolume);						// volume up 처리 (main window에 직접 메시지 전송 할 것)
                break;
            default:
            	break;
            }
	        iCoreTarget->VolumeUpResponse(status, KErrNone);
	        User::WaitForRequest(status);   
	        break;
        }       
    case ERemConCoreApiVolumeDown:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                //atUIENG_OnKeyUp(EStdKeyDecVolume);						// volume down 처리 (main window에 직접 메시지 전송할 것)
                break;
            default:
            	break;
            }
	        iCoreTarget->VolumeDownResponse(status, KErrNone);
	        User::WaitForRequest(status);   
	        break;
        }
    case ERemConCoreApiFastForward:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                break;
            }
	        iCoreTarget->FastForwardResponse(status, KErrNone);
	        User::WaitForRequest(status);
	        break; 
        }              
    case ERemConCoreApiBackward:
        {
        	switch (aButtonAct)
            {
            case ERemConCoreApiButtonClick:
                break;
            }
	        iCoreTarget->BackwardResponse(status, KErrNone);
	        User::WaitForRequest(status);
	        break;
        }                   
    default:
        break;
    }

}


// End of File

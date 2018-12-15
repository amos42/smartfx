/*
 ============================================================================
 Name		: TornadoSAppUi.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : CTornadoSAppUi implementation
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

#include <TornadoS_0xEBB22191.rsg>

#include "TornadoS_0xEBB22191.hlp.hrh"
#include "TornadoS.hrh"
#include "TornadoS.pan"
#include "TornadoSApplication.h"
#include "TornadoSAppUi.h"
#include "TornadoSAppView.h"

//#include "AT_Types.h"
#include "UIShotDef.h"
#include "bup.h"
//#include "MPFile.h"

//_LIT(KFileName, "C:\\private\\EBB22191\\TornadoS.txt");
//_LIT(KText, "Hello World!");

// ============================ MEMBER FUNCTIONS ===============================


//#define		R_ABOUT_DIALOG_TEXT		0//_AT("About")
//#define		R_ABOUT_DIALOG_TITLE	1//_AT("Title")
//#define		R_ABOUT_QUERY_DIALOG	2//_AT("Query")
//#define		R_COMMAND1_TEXT			3//_AT("Command")


// -----	------------------------------------------------------------------------
// CTornadoSAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//

void CTornadoSAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	//BaseConstructL(CAknAppUi::EAknEnableSkin);
	BaseConstructL();

	iInterfaceSelector = CRemConInterfaceSelector::NewL();
	iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
	iInterfaceSelector->OpenTargetL();

	// Create view object
	const TRect& aRect = ApplicationRect();
	
	// Create view object
	iAppView = CTornadoSAppView::NewL( aRect );
	
#if USE_RP_TEST
	if (RPTestProc())
		InitBUP( aRect.Width(), aRect.Height(), (int)iAppView, (long)this );
#elif USE_GPS_TEST
	GPSTestProc();
#endif
	
	// BUP initiaialize
#if !USE_RP_TEST && !USE_GPS_TEST
	InitBUP( aRect.Width(), aRect.Height(), (int)iAppView, (long)this );
#endif
	
	
	// Create a file to write the text to
	/*TInt err = CCoeEnv::Static()->FsSession().MkDirAll(KFileName);
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

	CleanupStack::PopAndDestroy(2); // outputFileStream, file*/
	}
// -----------------------------------------------------------------------------
// CTornadoSAppUi::CTornadoSAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTornadoSAppUi::CTornadoSAppUi()
{
	iInterfaceSelector = NULL;
	iCoreTarget = NULL;
}

// -----------------------------------------------------------------------------
// CTornadoSAppUi::~CTornadoSAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CTornadoSAppUi::~CTornadoSAppUi()
{
	// BUP deinitialize
	DeinitBUP();
	
	if (iAppView)
	{
		delete iAppView;
		iAppView = NULL;
	}

	/* 이건 지우지 않아도 된다고 함..
	if( iCoreTarget )
	{
		delete iCoreTarget;
		iCoreTarget = NULL;
	}
	*/

	if( iInterfaceSelector )
	{
		delete iInterfaceSelector;
		iInterfaceSelector = NULL;
	}

}

// -----------------------------------------------------------------------------
// CTornadoSAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CTornadoSAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
//		atAPPMNG_Exit(0);	// app hide 후 키입력 안되는 문제 있음.
		atUIAPI_SystemOff(0);
		break;
		/*
		case EAknCmdTaskSwapper:
			break;
		*/
		
/*
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

			CArrayFix<TCoeHelpContext>* buf = CCoeAppUi::AppHelpContextL();
			HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
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
			Panic(ETornadoSUi);
			break;
*/
		}
	}

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CTornadoSAppUi::HandleStatusPaneSizeChange()
{
	iAppView->SetRect( ApplicationRect() );
}

void CTornadoSAppUi::HandleForegroundEventL( TBool aForeground )
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


TKeyResponse CTornadoSAppUi::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
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

void CTornadoSAppUi::MrccatoCommand( TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct )
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


// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
/*CArrayFix<TCoeHelpContext>* CTornadoSAppUi::HelpContextL() const
	{
#warning "Please see comment about help and UID3..."
	// Note: Help will not work if the application uid3 is not in the
	// protected range.  The default uid3 range for projects created
	// from this template (0xE0000000 - 0xEFFFFFFF) are not in the protected range so that they
	// can be self signed and installed on the device during testing.
	// Once you get your official uid3 from Symbian Ltd. and find/replace
	// all occurrences of uid3 in your project, the context help will
	// work.
	CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidTornadoSApp, KGeneral_Information));
	CleanupStack::Pop(array);
	return array;
	}
*/
// End of File

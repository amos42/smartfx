//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "f_main.h"
#include "abitmap/abitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define AMOS_IMAGE_FILE_VER  2
#define CORE_EXE_FILENAME    "img_conv.exe"

#define READ_BUFFER_SIZE 1048576

static int fmt_type = 0;
static char core_exe_filename[256];

TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
    fmt_type = RadioGroup2->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
    TIniFile *ini = new TIniFile( ChangeFileExt( Application->ExeName, ".ini" ) );

    String core_exe = ini->ReadString("Setting", "CoreExeFilename", CORE_EXE_FILENAME );
    strcpy( core_exe_filename, core_exe.c_str() );
    
    RadioGroup1->ItemIndex = ini->ReadInteger("Setting", "OutputFormat", 4 );
    RadioGroup2->ItemIndex = ini->ReadInteger("Setting", "OutputFileType", 2 );
    
    Edit1->Text = ini->ReadString("Recent", "src_filename", "" );
    Edit2->Text = ini->ReadString("Recent", "img_filename", "" );
    Edit3->Text = ini->ReadString("Recent", "msk_filename", "" );

    this->Left          = ini->ReadInteger("Position", "Left", 0 );
    this->Top           = ini->ReadInteger("Position", "Top", 0 );
    this->Width         = ini->ReadInteger("Position", "Width", 705 );
    this->Height        = ini->ReadInteger("Position", "Height", 660 );
    
    delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
    TIniFile *ini = new TIniFile( ChangeFileExt( Application->ExeName, ".ini" ) );
    if( ini == NULL ) return;

    try {
      ini->WriteString("Setting", "OutputFormat", IntToStr(RadioGroup1->ItemIndex) );
      ini->WriteString("Setting", "OutputFileType", IntToStr(RadioGroup2->ItemIndex) );

      ini->WriteString("Recent", "src_filename", Edit1->Text );
      ini->WriteString("Recent", "img_filename", Edit2->Text );
      ini->WriteString("Recent", "msk_filename", Edit3->Text );

      ini->WriteString("Position", "Left", IntToStr(this->Left) );
      ini->WriteString("Position", "Top", IntToStr(this->Top) );
      ini->WriteString("Position", "Width", IntToStr(this->Width) );
      ini->WriteString("Position", "Height", IntToStr(this->Height) );
    } catch (...){
    }

    delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::RadioGroup2Click(TObject *Sender)
{
    fmt_type = RadioGroup2->ItemIndex;

    String filename = Edit1->Text;

    if( (fmt_type >= 0) && (fmt_type <= 3) ){
      char *ext[] = { ".img.h", ".img.bin", ".img", ".img", "" };
      Edit2->Text = ChangeFileExt( filename, ext[fmt_type] );
      char *ext2[] = { ".msk.h", ".msk.bin", ".msk", ".xxx", "" };
      Edit3->Text = ChangeFileExt( filename, ext2[fmt_type] );
//      Image1->Picture->LoadFromFile( Edit1->Text );
    } else {
      Edit2->Text = "";
      Edit3->Text = "";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SpeedButton1Click(TObject *Sender)
{
    if( !OpenPictureDialog1->Execute() ) return;
    String filename = OpenPictureDialog1->FileName;
//    if( !OpenDialog1->Execute() ) return;
//    String filename = OpenDialog1->FileName;

    Edit1->Text = filename;

    if( (fmt_type >= 0) && (fmt_type <= 3) ){
      char *ext[] = { ".img.h", ".img.bin", ".img", ".img", "" };
      Edit2->Text = ChangeFileExt( filename, ext[fmt_type] );
      char *ext2[] = { ".msk.h", ".msk.bin", ".msk", ".xxx", "" };
      Edit3->Text = ChangeFileExt( filename, ext2[fmt_type] );
      Image1->Picture->LoadFromFile( Edit1->Text );
    } else {
      Edit2->Text = "";
      Edit3->Text = "";
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SpeedButton2Click(TObject *Sender)
{
    if( !SaveDialog1->Execute() ) return;

    Edit2->Text = SaveDialog1->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnConvertClick(TObject *Sender)
{
    Memo1->Text = "";

    SECURITY_ATTRIBUTES Security;
    HANDLE ReadPipe, WritePipe;
    STARTUPINFO Start;
    TProcessInformation ProcessInfo;
    char Buffer[1000];
    DWORD BytesRead, TotalBytesRead;
    DWORD Apprunning, N, BytesLeftThisMessage, TotalBytesAvail;


    Security.nLength              = sizeof(TSecurityAttributes);
    Security.bInheritHandle       = TRUE;
    Security.lpSecurityDescriptor = NULL;

    if( !CreatePipe(&ReadPipe, &WritePipe, &Security, 0) ) return;

    ZeroMemory(&Start, sizeof(Start));
    Start.cb          = sizeof(Start);
    Start.hStdOutput  = WritePipe;
    Start.hStdInput   = ReadPipe;
    Start.hStdError   = stderr;
    Start.dwFlags     = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    Start.wShowWindow = SW_HIDE;

    char str[1024];
    static int bpp_val[7] = { 6, 7, 8, 15, 16, 24, 32 };
    sprintf( str, "%s%s %d %s %s %s %d %d", ExtractFilePath(Application->ExeName).c_str(), core_exe_filename,
                                           AMOS_IMAGE_FILE_VER,
                                           Edit1->Text.c_str(), Edit2->Text.c_str(), Edit3->Text.c_str(),
                                           bpp_val[RadioGroup1->ItemIndex], RadioGroup2->ItemIndex );
    int ret = CreateProcess(NULL, str,
                     &Security, &Security,
                     TRUE, CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS,
                     NULL, NULL,
                     &Start , &ProcessInfo);
    if( ret == NULL ){
        ShowMessage( "Can not create processor : '" + (String)core_exe_filename + "'" );
        return;
    }

    N = 0;
      TotalBytesRead = 0;
      do {
        // Increase counter to prevent an endless loop if the process is dead
        N++;

        // wait for end of child process
        Apprunning = WaitForSingleObject(ProcessInfo.hProcess, 100);

        // it is important to read from time to time the output information
        // so that the pipe is not blocked by an overflow. New information
        // can be written from the console app to the pipe only if there is
        // enough buffer space.

        if(!PeekNamedPipe(ReadPipe, &Buffer[TotalBytesRead], READ_BUFFER_SIZE,
                             &BytesRead, &TotalBytesAvail, &BytesLeftThisMessage) )
          break;
        else if( BytesRead > 0 )
          ReadFile(ReadPipe, &Buffer[TotalBytesRead], BytesRead, &BytesRead, NULL);

        TotalBytesRead += BytesRead;

    //        for( i = 0; i < TotalBytesRead; i++ ){
    //        }

        if( TotalBytesRead > 0 ){
          Buffer[TotalBytesRead] = '\0';
          OemToChar(Buffer, Buffer);
          Memo1->Text = Memo1->Text + Buffer;

          TotalBytesRead = 0;
          N = 0;
        }

        Application->ProcessMessages();
      } while( (Apprunning == WAIT_TIMEOUT) && (N <= 100) );

    GetExitCodeProcess(ProcessInfo.hProcess, (unsigned long *)&ret);

    CloseHandle(ProcessInfo.hProcess);
    CloseHandle(ProcessInfo.hThread);
    CloseHandle(ReadPipe);
    CloseHandle(WritePipe);

    switch( ret ){
        case 0  : ShowMessage( "Success!" ); break;
        case -1 : ShowMessage( "Not enough parameters" ); break;
        case -2 : ShowMessage( "not supported version or format" ); break;
        case -3 : ShowMessage( "Invalide Source Bitmap Filename" ); break;
        case -4 : ShowMessage( "source file is no BMP format" ); break;
        case -5 : ShowMessage( "not support source BPP" ); break;
        case -6 : ShowMessage( "not support destination BPP" ); break;
        case -7 : ShowMessage( "target file open error" ); break;
        default : ShowMessage( "Somthing problem : " + IntToStr(ret) ); break;
    }
}
//---------------------------------------------------------------------------


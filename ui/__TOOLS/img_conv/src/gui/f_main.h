//---------------------------------------------------------------------------

#ifndef f_mainH
#define f_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include <Buttons.hpp>
#include <ExtDlgs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
    TButton *btnConvert;
    TEdit *Edit1;
    TEdit *Edit2;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TOpenPictureDialog *OpenPictureDialog1;
    TPanel *Panel1;
    TImage *Image1;
    TStatusBar *StatusBar1;
    TSaveDialog *SaveDialog1;
    TLabel *Label1;
    TLabel *Label2;
    TRadioGroup *RadioGroup1;
    TRadioGroup *RadioGroup2;
    TLabel *Label3;
    TEdit *Edit3;
    TSpeedButton *SpeedButton3;
    TOpenDialog *OpenDialog1;
    TMemo *Memo1;
    void __fastcall btnConvertClick(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall RadioGroup2Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif

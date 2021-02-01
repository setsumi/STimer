//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Tabs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TLabel *Label1;
	TTimer *Timer2;
	TSpeedButton *btnTopmost;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TEdit *Edit1;
	TUpDown *UpDown1;
	TDateTimePicker *DateTimePicker1;
	TTimer *Timer3;
	TLabel *lblOverdue;
	TTimer *tmrOverdue;
	void __fastcall Edit1KeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Timer2Timer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnTopmostClick(TObject *Sender);
	void __fastcall Label1Click(TObject *Sender);
	void __fastcall FormContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall DateTimePicker1Enter(TObject *Sender);
	void __fastcall Timer3Timer(TObject *Sender);
	void __fastcall tmrOverdueTimer(TObject *Sender);

private:	// User declarations
	void SoundAlarm();
	void StopAlarm();
	void StartStop();
	void Save();
	void Load();

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

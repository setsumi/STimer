//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>
#include <DateUtils.hpp>
#include <stdio.h>
#include <Mmsystem.h>

#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

const wchar_t *gTitle = L"STimer";
const wchar_t *gInactive = L"Inactive";
const wchar_t *gFontFile = L"STimer.ttf";
const wchar_t *gFontName = L"Symbola";

int gCountdown = 0, gMode = 0;
TDateTime gCountTime;
TDateTime gOverdueTime;

//---------------------------------------------------------------------------
int SaveResourceRawBin(LPCTSTR ResID, const wchar_t *FileName)
{
	HRSRC Resource = FindResource(HInstance, ResID, RT_RCDATA);
	if (!Resource) return NULL;

	DWORD Length = SizeofResource(HInstance, Resource);
	if ((Length == 0) && (GetLastError() != 0)) return NULL;

	HGLOBAL Address = LoadResource(HInstance, Resource);
	if (!Address) return NULL;

	PVOID FontData = LockResource(Address);
	if (!FontData) return NULL;

	FILE *pFile = _wfopen(FileName, L"wb");
	if (!pFile) return NULL;
	fwrite(FontData, 1, Length, pFile);
	fclose(pFile);

	return 1;
}

//---------------------------------------------------------------------------
void TForm1::SoundAlarm()
{
	Caption = L"ALARM";
	Label1->Visible = false;
	if (Screen->Fonts->IndexOf(gFontName) != -1) {
		Label1->Font->Name = gFontName;
		Label1->Caption = L"\xD83D\xDD0A"; //Unicode Character "Speaker" (U+1F50A)
	} else {
		Label1->Caption = L"AL";
	}

	FLASHWINFO fo;
	ZeroMemory(&fo, sizeof(fo));
	fo.cbSize = sizeof(fo);
	fo.hwnd = Handle;
	fo.dwFlags = FLASHW_TRAY|FLASHW_TIMER;
	FlashWindowEx(&fo);

	Timer2->Tag = 0;
	Timer2->Interval = 1;
	Timer2->Enabled = true;

	gOverdueTime = 0;
	tmrOverdueTimer(NULL);
	lblOverdue->Left = Label1->Left + Label1->Width + 10;
	lblOverdue->Top = Label1->Top + (Label1->Height - lblOverdue->Height) / 2;
	lblOverdue->Visible = true;
	tmrOverdue->Enabled = true;
}
//---------------------------------------------------------------------------
void TForm1::StopAlarm()
{
	Timer2->Enabled = false;
	PlaySound(NULL, 0, 0); // stop sound immediately
	Caption = gTitle;
	Label1->Visible = false;
	if (Screen->Fonts->IndexOf("Symbola") != -1)
		Label1->Font->Name = L"Tahoma";
	Label1->Caption = gInactive;
	Label1->Visible = true;

	FLASHWINFO fo;
	ZeroMemory(&fo, sizeof(fo));
	fo.cbSize = sizeof(fo);
	fo.hwnd = Handle;
	fo.dwFlags = FLASHW_STOP;
	FlashWindowEx(&fo);

	tmrOverdue->Enabled = false;
	lblOverdue->Visible = false;
}

//---------------------------------------------------------------------------
void TForm1::StartStop()
{
	if (Timer1->Enabled) // STOP
	{
		Timer1->Enabled = false;
		Caption = gTitle;
		Label1->Caption = gInactive;
	}
	else  // START
	{
		gMode = PageControl1->TabIndex;
		if (gMode == 0) // Seconds
		{
			gCountdown = UpDown1->Position;
			Label1->Caption = Edit1->Text;
			Caption = Label1->Caption;
			Timer1->Enabled = true;
		}
		else if (gMode == 1) // Clock
		{
			gCountTime = DateTimePicker1->Time;
			Label1->Caption = gCountTime.TimeString();
			Caption = Label1->Caption;
			Timer1->Enabled = true;
		}
	}
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Edit1KeyPress(TObject *Sender, wchar_t &Key)
{
	switch(Key) {
		case VK_RETURN: // Enter - start or stop timer
			Key = 0;
			Label1Click(NULL);
			break;
		case VK_ESCAPE: // Esc - stop timer or minimize window
			Key = 0;
			if (Timer2->Enabled) {
				StopAlarm();
			} else {
				WindowState = wsMinimized;
			}
			break;
		case 20:  // Ctrl-T - toggle topmost
			Key = 0;
			btnTopmost->Down = !btnTopmost->Down;
			btnTopmostClick(NULL);
			break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;

	if (gMode == 0) // Seconds
	{
		gCountdown--;
		Label1->Caption = IntToStr(gCountdown);
		Caption = Label1->Caption;
		if (gCountdown <= 0) {
			SoundAlarm();
		} else {
			Timer1->Enabled = true;
		}
	}
	else if (gMode == 1) // Clock
	{
		gCountTime = IncSecond(gCountTime, -1);
		Label1->Caption = gCountTime.TimeString();
		Caption = Label1->Caption;
		unsigned short hour, min, sec, msec;
		gCountTime.DecodeTime(&hour, &min, &sec, &msec);
		if (hour == 0 && min == 0 && sec == 0) {
			SoundAlarm();
		} else {
			Timer1->Enabled = true;
		}
	}

}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
	TTimer *t = (TTimer*)Sender;
	t->Enabled = false;

	switch (t->Tag) {
	case 0:
		t->Interval = 250;
		PlaySound(L"ALARM_SOUND", HInstance, SND_RESOURCE|SND_ASYNC);
		break;
	case 1:
		Label1->Visible = !Label1->Visible;
		t->Interval = 750;
		break;
	case 2:
		Label1->Visible = !Label1->Visible;
		t->Tag = -1; // will be zero after increasing below
		t->Interval = 1;
	}
	t->Tag = t->Tag + 1;
	t->Enabled = true;
}

//---------------------------------------------------------------------------
void TForm1::Save()
{
	TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	ini->WriteInteger(L"GENERAL", L"Interval", UpDown1->Position);
	ini->WriteInteger(L"GENERAL", L"Topmost", btnTopmost->Down?1:0);
	ini->WriteInteger(L"GENERAL", L"Mode", PageControl1->TabIndex);
	ini->WriteString (L"GENERAL", L"Time", DateTimePicker1->Time.TimeString());
	delete ini;
}

//---------------------------------------------------------------------------
void TForm1::Load()
{
	TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	UpDown1->Position = ini->ReadInteger(L"GENERAL", L"Interval", 5);
	btnTopmost->Down = (ini->ReadInteger(L"GENERAL", L"Topmost", 0) != 0);
	btnTopmostClick(NULL);
	PageControl1->TabIndex = ini->ReadInteger(L"GENERAL", L"Mode", 0);
	DateTimePicker1->Time = StrToTime(ini->ReadString(L"GENERAL", L"Time", L"00:00:05"));
	delete ini;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	FormatSettings.LongTimeFormat = L"HH:mm:ss";
	DateTimePicker1->Format = FormatSettings.LongTimeFormat;

	Caption = gTitle;
	Label1->Caption = gInactive;
	Hint = Edit1->Hint;
	Label1->Hint = Hint;

	Load();

	if (PageControl1->TabIndex == 0)
		Edit1->SetFocus();
	else
		DateTimePicker1->SetFocus();

	// load font if necessary
	if (Screen->Fonts->IndexOf(gFontName) == -1) {
		if (!FileExists(gFontFile))
			SaveResourceRawBin(MAKEINTRESOURCE(1234), gFontFile);
		AddFontResourceEx(gFontFile, FR_PRIVATE, NULL);
		PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	Save();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::btnTopmostClick(TObject *Sender)
{
	SetWindowPos(Handle, btnTopmost->Down?HWND_TOPMOST:HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Label1Click(TObject *Sender)
{
	if (Timer2->Enabled) {
		StopAlarm();
	} else {
		StartStop();
	}
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	Handled = true;
	WindowState = wsMinimized;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::DateTimePicker1Enter(TObject *Sender)
{
	if (!DateTimePicker1->Tag)
		Timer3->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
	Timer3->Enabled = false;
	DateTimePicker1->Tag = 1;
	DateTimePicker1->Perform(WM_KEYDOWN, VK_LEFT, 0);
	DateTimePicker1->Perform(WM_KEYUP, VK_LEFT, 0);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::tmrOverdueTimer(TObject *Sender)
{
	Caption = String(L"ALARM - ") + gOverdueTime.TimeString();
	lblOverdue->Caption = gOverdueTime.TimeString();
	gOverdueTime = IncSecond(gOverdueTime, 1);
}

//---------------------------------------------------------------------------


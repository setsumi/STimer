//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>
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

int gCountdown = 0;

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
		Label1->Caption = L"ALARM";
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
}

//---------------------------------------------------------------------------
void TForm1::StartStop()
{
	Label1->Caption = Edit1->Text;
	if (Timer1->Enabled) {
		Timer1->Enabled = false;
		Caption = gTitle;
		Label1->Caption = gInactive;
	} else {
		gCountdown = UpDown1->Position;
		Label1->Caption = Edit1->Text;
		Caption = Label1->Caption;
		Timer1->Enabled = true;
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
		case VK_RETURN:
			Key = 0;
			if (Timer2->Enabled) {
				StopAlarm();
			} else {
				StartStop();
			}
			break;
		case VK_ESCAPE:
			Key = 0;
			if (Timer2->Enabled) {
				StopAlarm();
			} else {
				WindowState = wsMinimized;
      }
			break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;
	gCountdown--;
	Label1->Caption = IntToStr(gCountdown);
	Caption = Label1->Caption;
	if (gCountdown <= 0) {
		SoundAlarm();
	} else {
		Timer1->Enabled = true;
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
	delete ini;
}

//---------------------------------------------------------------------------
void TForm1::Load()
{
	TIniFile *ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	UpDown1->Position = ini->ReadInteger(L"GENERAL", L"Interval", 5);
	delete ini;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Load();

	Caption = gTitle;
	Label1->Caption = gInactive;
	Hint = Edit1->Hint;
	Label1->Hint = Hint;

	// load font if necessary
	if (Screen->Fonts->IndexOf(gFontName) == -1) {
		if (!FileExists(gFontFile))
			SaveResourceRawBin(MAKEINTRESOURCE(1234), gFontFile);
		AddFontResourceEx(gFontFile, FR_PRIVATE, NULL);
		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	Save();
}



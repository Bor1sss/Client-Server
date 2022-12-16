#pragma once
#include<windows.h>
#include "Leaderboard.h"
#include <fstream>
#include <tchar.h>
#include <windowsX.h>
#include <time.h>
#include "resource.h"
#include <conio.h>
#include <string>
#include <commctrl.h>
#pragma comment(lib,"comctl32")
using namespace std;

class Keyboard
{
public:
	Keyboard(void);
public:
	~Keyboard(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Keyboard* ptr;

	int Max_WPM;
	int WPM;
	bool Limit = false;
	struct Time
	{
		int Minutes = 1;
		int Seconds = 0;
	}Time;

	void Cls_OnClose(HWND hwnd);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
	void Cls_OnCheck(WPARAM wp, LPARAM lp);
	void Max();
	void Words(WPARAM wp, LPARAM lp);

	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void OnTrayIcon(WPARAM wp, LPARAM lp);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hButton[26];
	HWND hCPS;
	HWND hTime;
	HWND hProgress;

	HMENU hMenu;
	PNOTIFYICONDATA pNID;
	HWND hDialog;


	HICON hIcon;
	HWND hEdit;
	HANDLE hMutex;


	LOGFONT lf;
	HFONT hFont;
	COLORREF color;
	TCHAR key[27] = { L"QWERTYUIOPASDFGHJKLZXCVBNM" };


	int Level = 0;
	int word = 0;


	static const int Max_Levels = 3;
	TCHAR words[Max_Levels][1024] = { L"Here you can find activities has i understand you ",L"Hello world",L"World Hello"};
	
	TCHAR Infinite_words[2048] = { L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed consequat sapien eu blandit aliquet. Cras at dictum eros. Vivamus venenatis molestie varius. Aenean non mollis enim. Proin maximus leo fermentum, pretium magna eu, tincidunt sem. Aenean varius orci eu fermentum aliquet. Vivamus tincidunt ante ante, sit amet euismod orci vehicula quis. Nunc dictum placerat est. Suspendisse quis turpis mi. Nam a accumsan nisi. Phasellus dignissim, felis non consectetur commodo, lectus nibh fringilla elit, et semper turpis libero eu neque. Proin a tincidunt quam. Aliquam erat volutpat. Aliquam erat volutpat. Nunc scelerisque arcu ac pretium venenatis.Nunc arcu lectus, aliquet quis faucibus vel, efficitur et massa.Proin tristique orci id dolor molestie, id eleifend enim rhoncus.Nulla interdum rhoncus pulvinar.Aenean pretium diam id enim rhoncus fringilla.In hac habitasse platea dictumst.Vivamus mi elit, suscipit a lacus et, iaculis malesuada felis.Fusce eu quam nec elit hendrerit hendrerit a non ex.Maecenas ac felis tortor.Nullam et lacus accumsan, ullamcorper dui vestibulum, tincidunt felis.Donec blandit auctor molestie.Cras ut aliquet lorem.Nam vehicula risus eget odio convallis, blandit condimentum odio sollicitudin.Mauris ut ipsum augue.Mauris laoreet, urna vel volutpat rutrum, nibh leo fringilla erat, at feugiat eros quam." };
	wstring a;//erase(0,1)
	wstring Infinite;
	bool Pause = false;
	bool Game = false;
	int MaxWords = 0;




	HANDLE h;
};

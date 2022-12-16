#pragma once
#include<windows.h>
#include <fstream>
#include <tchar.h>
#include <windowsX.h>
#include <time.h>
#include "resource.h"
#include <conio.h>
#include <string>
#include <commctrl.h>
using namespace std;
class Leaderboard
{
public:
	Leaderboard(void);
	Leaderboard(LPCTSTR lpStr);
	Leaderboard(int, int ,int);
public:
	~Leaderboard(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Leaderboard* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void WriteScore();
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	TCHAR text[200];
	HWND hLeaderboard, hEditScore;
	
	struct Time
	{
		int Minutes = 1;
		int Seconds = 0;
	}Max_Time;
	int Max_WPM;
	
};

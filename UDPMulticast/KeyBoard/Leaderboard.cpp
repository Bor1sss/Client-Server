#include "Leaderboard.h"

Leaderboard* Leaderboard::ptr = NULL;

Leaderboard::Leaderboard(void)
{
	ptr = this;
}

Leaderboard::Leaderboard(LPCTSTR lpStr)
{
	ptr = this;
	
}
Leaderboard::Leaderboard(int WPM,int Minute,int Second)
{
	ptr = this;
	Max_WPM = WPM;
	Max_Time.Minutes = Minute;
	Max_Time.Seconds = Second;


}
Leaderboard::~Leaderboard(void)
{

}

void Leaderboard::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, IDCANCEL);
}

BOOL Leaderboard::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hLeaderboard = GetDlgItem(hwnd, IDC_LEADERBOARD);
	hEditScore = GetDlgItem(hwnd, IDC_SCORE); 
	
	SetWindowText(hwnd, TEXT("Leaderboard"));
	WriteScore();

	return TRUE;
}

void Leaderboard::WriteScore()
{
	

	struct Score {
		wstring name;
		int wpm;
		int min;
		int sec;
	};
	wfstream in("Leaderboard.txt", ios::in | ios::out);
	
	
	wstring buff1;
	wstring name;
	int wpm;
	
	int min;
	int sec;
	TCHAR buff_int[256];
	int kol = 0;

	Score* Player;

	if (in.is_open())
	{
		in.seekg(0, ios::beg);
		buff1 += L"\t\tLeaderboard\r\n";

		while (!in.eof())
		{

			in >> name >> wpm >> min >> sec;
			kol++;

		}
		in.close();

	}
	in.open("Leaderboard.txt",ios::in|ios::out);
	if(in.is_open()) {
		if (kol > 0) {
			Player = new Score[kol];


			in.seekg(0, ios::beg);
			int i = 0;



			while (!in.eof()) {
				in >> name >> wpm >> min >> sec;
				Player[i].name = name;
				Player[i].wpm = wpm;
				Player[i].min = min;
				Player[i].sec = sec;
				i++;
			}


			Score temp;
			for (int i = 0; i < (kol - 1); i++)
			{
				for (int j = 0; j < (kol - i - 1); j++)
				{

					if (Player[j].min == Player[j + 1].min)
					{
						if (Player[j].sec > Player[j + 1].sec) {
							temp.min = Player[j].min;
							temp.sec = Player[j].sec;
							temp.name = Player[j].name;
							temp.wpm = Player[j].wpm;




							Player[j].min = Player[j + 1].min;
							Player[j].name = Player[j + 1].name;
							Player[j].sec = Player[j + 1].sec;
							Player[j].wpm = Player[j + 1].wpm;


							Player[j + 1].min = temp.min;
							Player[j + 1].sec = temp.sec;
							Player[j + 1].name = temp.name;
							Player[j + 1].wpm = temp.wpm;
						}

					}
					else if (Player[j].min > Player[j + 1].min)
					{
						temp.min = Player[j].min;
						temp.sec = Player[j].sec;
						temp.name = Player[j].name;
						temp.wpm = Player[j].wpm;




						Player[j].min = Player[j + 1].min;
						Player[j].name = Player[j + 1].name;
						Player[j].sec = Player[j + 1].sec;
						Player[j].wpm = Player[j + 1].wpm;


						Player[j + 1].min = temp.min;
						Player[j + 1].sec = temp.sec;
						Player[j + 1].name = temp.name;
						Player[j + 1].wpm = temp.wpm;
					}
				}
			}


			for (int i = 0; i < kol; i++)
			{

				buff1 += Player[i].name;
				buff1 += L"\t\t\t WPM_MAX:";

				wsprintf(buff_int, TEXT("%d"), Player[i].wpm);
				buff1 += buff_int;

				wsprintf(buff_int, TEXT("\t%d:%d"), Player[i].min, Player[i].sec);
				buff1 += buff_int;

				buff1 += '\r';
				buff1 += '\n';
			}
			in.close();
			SetWindowText(hLeaderboard, buff1.c_str());
		}
	}

	
}

void Leaderboard::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	
	if (id == IDC_BUTTON1)
	{
		wstring buff1;

		TCHAR a[256];
		GetWindowText(hEditScore, a, 256);

		buff1 = a;
		for (int i = 0; i < buff1.length(); i++)
		{
			if (buff1[i] == ' ')
			{
				buff1[i] = '_';
			}
		}
		TCHAR buff_int[256];
		
		wfstream out(TEXT("Leaderboard.txt"),ios::app);
		int pos;
		
		if (!out.is_open())
		{
				
			
		}
		else {
			
				out << '\n';
		
				buff1 += '\t';
				
				wsprintf(buff_int, TEXT("%d"), Max_WPM);
				buff1 += buff_int;


				buff1 += '\t';
				wsprintf(buff_int, TEXT("%d  %d"), Max_Time.Minutes,Max_Time.Seconds);
				buff1 += buff_int;


				out << buff1;
			
		}
		
		out.close();
		WriteScore();
	}
	
}

BOOL CALLBACK Leaderboard::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}

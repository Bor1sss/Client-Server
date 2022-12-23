#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;
string Normal;

string codes[] = { "840","978", };

string buff[] = { "\"txt\"","\"rate\"","\"cc\"","\"exchangedate\"" };
string Currency[3];

void ReadFile()
{

    ifstream out("file.txt");
   string buff2;
    string buff3;

    while (!out.eof())
    {
        out >> buff2;
        buff3 += buff2;
    }



    int Index;




    for (int i = 0; i < 2; i++)
    {



        Index = buff3.find(codes[i]);
        if (Index == -1) {
            Currency[i] += buff[i];
            Currency[i] += ": No info found";
        }
        else
        {
           
            int z = Index;
            while (buff3[z] != '}')
            {
                if (buff3[z] == ',')
                {
                    Currency[i] += "\r\n";
                }
                else {
                    Currency[i] += buff3[z];
                }
                z++;
            }







            Currency[i] += "\r\n";


        }
        cout<<Currency[i]<<endl;
    }
    
    

}
int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleCP(CP_UTF8);

    // the URL to download from 
    string srcURL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?json";
  
    string destFile = "file.txt";
    //// URLDownloadToFile returns S_OK on success 
 /*   if (S_OK == URLDownloadToFileA(NULL, srcURL.c_str(), destFile.c_str(), 0, NULL))
    {
        cout << "Saved to " << destFile << "\n";
    }
    else cout << "Fail!\n";*/
    
    ReadFile();
    

}


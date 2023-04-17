//-----------------------------------------------------------------------
// Project name     : Latin assistant
//-----------------------------------------------------------------------
// Source           : main.cpp
// Last modified    : 27.03.2023
// Version          : 9
// Author           : Nikita Vakhrushev
//-----------------------------------------------------------------------

#include "latin.h"

using namespace std;

const string    USEROUTPUT_PATH = "UserOutput.txt",
                DICTIONARYLR_PATH = "dictionaryLR.txt",
                DICTIONARYRL_PATH = "dictionaryRL.txt",
                LOG_PATH = "log.txt";

WordInfo currentRequest;
Log mainLog(LOG_PATH);
OutputStream mainOutput(USEROUTPUT_PATH);

int main() {
    mainLog.print("= File: main.cpp");
    mainLog.print("= Start: int main()");
    currentRequest.setCode(menuCode::DEFAULT);
    system("chcp 1251");
    setlocale(LC_ALL, "Russian");
    while (currentRequest.code != menuCode::QUIT) {
        system("cls");
        if (currentRequest.code == 0) {
            currentRequest.clear();
            ConsoleOutputBlock startMenuChoice("Íàæìèòå ...", 5,
                { "ïåðåâîä ñëîâà","îïðåäåëåíèå ôîðìû ñëîâà","ðåäàêòîð ñîõðàí¸ííûõ ñëîâ", "Esc - âûõîä èç ïðîãðàììû" },
                { 1,2,3,-1 });
            startMenuChoice.print();
            currentRequest.setCode(startMenuChoice.inputChoice());
        }
        switch (currentRequest.code) {
        case menuCode::DICTIONARYSEARCH: {
            dictionarySearch(DICTIONARYLR_PATH, DICTIONARYRL_PATH);
            break;
        }
        case menuCode::OUT_DICTIONARYSEARCH: {
            dictionarySearch(DICTIONARYLR_PATH, DICTIONARYRL_PATH);
            break;
        }
        case menuCode::FORMANALYSIS: {
            formAnalysis();
            break;
        }
        case menuCode::OUT_FORMANALYSIS: {
            formAnalysis();
            break;
        }
        case menuCode::OUTPUTACTION: {
            currentRequest.clear();
            mainOutput.action();
            break;
        }
        case menuCode::QUIT: {
            mainLog.print("= User decided to exit program");
            system("cls");
            cout << "Ñîõðàíèòü ñîäåðæàíèå ñëîâàðÿ?\n     0 - íåò\n     Ëþáàÿ äðóãàÿ êëàâèøà - äà\n";
            char c = _getch();
            if (c == '0') {
                mainLog.print("= User decided to clear dictionary");
                mainOutput.clear();
            }
            else {
                mainLog.print("= User decided to save dictionary");
            }
            break;
        }
        default: {
            currentRequest.setCode(menuCode::DEFAULT);
            break;
        }
        }
    }

    mainLog.print("= End: int main()");
    mainLog.noteExit(100);
}

//-----------------------------------------------------------------------
// Project name     : Latin assistant
//-----------------------------------------------------------------------
// Source           : main.cpp
// Last modified    : 01.04.2023
// Version          : 10
// Author           : Nikita Vakhrushev
//-----------------------------------------------------------------------

#include "latin.h"

using namespace std;

const string    USER_OUTPUT_PATH = "UserOutput.txt",
                DICTIONARY_LR_PATH = "dictionaryLR.txt",
                DICTIONARY_RL_PATH = "dictionaryRL.txt",
                LOG_PATH = "log.txt";

WordInfo currentRequest;
Log mainLog(LOG_PATH);
OutputStream mainOutput(USER_OUTPUT_PATH);

int main() {
    system("chcp 1251");
    bool local_IsActNotepad = false;
    if (local_IsActNotepad == true) {
        system("taskkill /im notepad.exe");
        mainLog.setIsStartNotepadLog(true);
    }
    mainLog.print("Start: int main()");
    currentRequest.setCode(menuCode::DEFAULT);
    while (currentRequest.code != menuCode::QUIT) {
        system("cls");
        if (currentRequest.code == menuCode::DEFAULT) {
            currentRequest.clear();
            ConsoleOutputBlock startMenuChoice("Нажмите ...", 5,
                { "перевод слова","определение формы слова","редактор сохранённых слов", "Esc - выход из программы" },
                { 1,2,3,-1 });
            startMenuChoice.print();
            currentRequest.setCode(startMenuChoice.inputChoice());
        }
        switch (currentRequest.code) {
        case menuCode::DICTIONARYSEARCH: {
            dictionarySearch(DICTIONARY_LR_PATH, DICTIONARY_RL_PATH);
            break;
        }
        case menuCode::OUT_DICTIONARYSEARCH: {
            dictionarySearch(DICTIONARY_LR_PATH, DICTIONARY_RL_PATH);
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
            cout << "Сохранить содержание словаря?\n     0 - нет\n     Любая другая клавиша - да\n";
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

    mainLog.print("End: int main()");
    mainLog.noteExit(100);
}

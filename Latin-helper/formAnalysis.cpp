//-----------------------------------------------------------------------
// Source           : formAnalysis.cpp
// Last modified    : 27.03.2023
// Version          : 9
// Description      : Determine form of user's word
//-----------------------------------------------------------------------

#include "latin.h"
#include "formAnalysis.h"

using namespace std;

void formAnalysis() {
    mainLog.print("== File: determineForm.cpp");
    mainLog.print("== Start of: determineForm");

    system("cls");
    if (currentRequest.code == menuCode::FORMANALYSIS) {
        cout << "Введите слово: ";
        while (currentRequest.inputWord() == false) {
            cout << "Слово написано не на латыни, повторите ввод\n";
        }
    }
    if (currentRequest.code == menuCode::OUT_FORMANALYSIS) {
        cout << currentRequest.word << endl;
        if(currentRequest.definition.empty() == false)
            cout << currentRequest.definition << endl;
    }
    
    while (true) {
        ConsoleOutputBlock selectPartOfSpeech("Выберите часть речи:", 5,
            { "существительное","прилагательное","глагол" }, { 1,2,3 });
        selectPartOfSpeech.print();
        switch (selectPartOfSpeech.inputChoice()) {
        case 1: {
            Noun* mainNoun = new Noun;
            delete mainNoun;
            break;
        }
        case 2: {
            Adjective* mainAdjective = new Adjective;
            delete mainAdjective;
            break;
        }
        case 3: {
            Verb* mainVerb = new Verb;
            delete mainVerb;
            break;
        }
        default: {
            cout << "Выбран некорректный вариант, нажмите 1, чтобы изменить выбор," <<
                " или любую другую клавишу, чтобы вернуться в меню\n";
            char c = _getch();
            if (c == '1') {
                cout << endl;
                continue;
            }
            else {
                mainLog.print("-- End of: determineForm");
                currentRequest.setCode(menuCode::DEFAULT);
                return void();
            }
        }
        }
        break;
    }

    cout << endl;
    currentRequest.printForms();

    cout << "\nНажмите 1, если хотите сохранить полученные данные, и любую другую клавишу, если нет\n";
    char c = _getch();
    if (c == '1') {
        mainOutput.updateWordInfo(-1, currentRequest);
    }

    cout << "\nДля нового запроса нажмите 2\n";
    if (currentRequest.definition.empty() == true)
        cout << "Для поиска определения данного слова нажмите 4\n";
    cout << "Для возвращения в меню нажмите любую клавишу\n";
    c = _getch();
    switch (c) {
    case '2': {
        currentRequest.setCode(menuCode::FORMANALYSIS);
        break;
    }
    case '4': {
        currentRequest.setCode(menuCode::OUT_DICTIONARYSEARCH);
        break;
    }
    default: {
        currentRequest.setCode(menuCode::DEFAULT);
        break;
    }
    }

    mainLog.print("== End of: determineForm");
}

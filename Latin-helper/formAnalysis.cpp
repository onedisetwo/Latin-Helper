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
        cout << "������� �����: ";
        while (currentRequest.inputWord() == false) {
            cout << "����� �������� �� �� ������, ��������� ����\n";
        }
    }
    if (currentRequest.code == menuCode::OUT_FORMANALYSIS) {
        cout << currentRequest.word << endl;
        if(currentRequest.definition.empty() == false)
            cout << currentRequest.definition << endl;
    }
    
    while (true) {
        ConsoleOutputBlock selectPartOfSpeech("�������� ����� ����:", 5,
            { "���������������","��������������","������" }, { 1,2,3 });
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
            cout << "������ ������������ �������, ������� 1, ����� �������� �����," <<
                " ��� ����� ������ �������, ����� ��������� � ����\n";
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

    cout << "\n������� 1, ���� ������ ��������� ���������� ������, � ����� ������ �������, ���� ���\n";
    char c = _getch();
    if (c == '1') {
        mainOutput.updateWordInfo(-1, currentRequest);
    }

    cout << "\n��� ������ ������� ������� 2\n";
    if (currentRequest.definition.empty() == true)
        cout << "��� ������ ����������� ������� ����� ������� 4\n";
    cout << "��� ����������� � ���� ������� ����� �������\n";
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

//-----------------------------------------------------------------------
// Source           : dictionarySearch.cpp
// Last modified    : 27.03.2023
// Version          : 9
// Description      : Search word in dictionary (Rus-Lat or Lat-Rus)
//-----------------------------------------------------------------------

#include "latin.h"

using namespace std;

//    if(lettersCutted < 2){
//        word.pop_back();
//        searchWord(word,lettersCutted + 1);
//    }

bool searchWord(uint lettersCutted, vector<string>& definitions, fstream& dictionaryFile, bool isLatRus) {
    mainLog.print({ "=== Start: bool searchWord(...) with lettersCutted = ", intToStr(lettersCutted, 0) });
    string  word = currentRequest.word.substr(0, currentRequest.word.size() - lettersCutted),
            inputLine;
    while (inputLine != "DICTIONARY_END") {
        getline(dictionaryFile, inputLine);
        if (inputLine.find(word) != string::npos) {
            definitions.push_back(inputLine);
            if (isLatRus == false && inputLine[0] >= 58) {
                do {
                    int pos = dictionaryFile.tellg();
                    getline(dictionaryFile, inputLine);
                    if (inputLine[0] < 58)
                        definitions.push_back(inputLine);
                    else
                        dictionaryFile.seekg(pos);
                } while (inputLine[0] < 58);
            }
        }
    }

    if (definitions.size() != 0) {
        mainLog.print("=== End: bool searchWord(...) returned true");
        return true;
    }
    else {
        mainLog.print("=== End: bool searchWord(...) returned false");
        return false;
    }
}

void dictionarySearch(const string DICTIONARYLR_PATH, const string DICTIONARYRL_PATH) {
    mainLog.print("== File: searchInDictionary.cpp");
    mainLog.print("== Start: void searchInDictionary()");

    vector<string> definitions;
    fstream dictionaryFile;
    const int maxLettersCutted = 3;

    setlocale(LC_ALL, "Russian");
    system("cls");
    
    bool isLatRus;
    if (currentRequest.code == menuCode::DICTIONARYSEARCH) {
        currentRequest.clear();
        cout << "�������� ������� � ���������� ����� �� ������� � � �������� �� ���������,\n" <<
            "����� ����� �������� ������������� �� ��������� �����.\n" << 
            "������� ������ (���������� ���� � ����� �����, �������� ����� ����������) �� ��������� ���������� 3\n\n" <<
            "������� ����� ��� �������� ��������� �������: ";
        isLatRus = currentRequest.inputWord();
        if(isLatRus == true)
            openFile(dictionaryFile, DICTIONARYLR_PATH, ios::in, true);
        else
            openFile(dictionaryFile, DICTIONARYRL_PATH, ios::in, true);
    }
    if (currentRequest.code == menuCode::OUT_DICTIONARYSEARCH) {
        cout << "����� ��� ��������: " << currentRequest.word << endl;
        if (currentRequest.word[0] > 0) {
            isLatRus = true;
            openFile(dictionaryFile, DICTIONARYLR_PATH, ios::in, true);
        }
        else {
            isLatRus = false;
            openFile(dictionaryFile, DICTIONARYRL_PATH, ios::in, true);
        }
    }

    cout << "\n����� �� �������: " << currentRequest.word << "...\n";

    uint lettersCutted = 0;
    while(lettersCutted <= maxLettersCutted) {
        dictionaryFile.seekg(0, ios::beg);
        if (searchWord(lettersCutted, definitions, dictionaryFile, isLatRus) == true) {
            break;
        }
        lettersCutted++;
    }

    if (definitions.size() == 0) {
        cout << "���������� �� ������� " << currentRequest.word << " �� �������";
        mainLog.print("-- Search failed");
    }
    else {
        mainLog.print({ "-- Found ", intToStr(definitions.size(), 0), " results" });
        if (lettersCutted == 0)
            cout << "���������� �� ������� " << currentRequest.word << ":\n\n";
        if (lettersCutted > 0)
            cout << "�����, �� ����� � ���� " <<
            currentRequest.word.substr(0, currentRequest.word.size() - lettersCutted) << "?\n\n";
        for (int i = 0; i < (int)(definitions.size()); i++)
            cout << i + 1 << ": " << definitions[i] << endl;
        cout << "\n���� ������ ��������� ������� ������, ������� 1, ����� - ����� ������ �������\n";
        char isContinue = _getch();
        while (isContinue == '1') {
            lettersCutted++;
            dictionaryFile.seekg(0, ios::beg);
            definitions.clear();
            cout << "\n����� �� �������: " << 
                currentRequest.word.substr(0, currentRequest.word.size() - lettersCutted) << "...\n";
            searchWord(lettersCutted, definitions, dictionaryFile, isLatRus);
            cout << "���������� �� ������� " <<
                currentRequest.word.substr(0, currentRequest.word.size() - lettersCutted) << ":\n\n";
            for (int i = 0; i < (int)(definitions.size()); i++)
                cout << i + 1 << ": " << definitions[i] << endl;
            cout << "\n���� ������ ��������� ������� ������, ������� 1, ����� - ����� ������ �������\n";
            isContinue = _getch();
        }
        cout << "\n������� ����� �������� ����������� �����������, ����� ��������� ���,\n" <<
            "��� ������ ��� ���������� - 0\n";
        int tempInt = inputInt();
        while (tempInt < 0 || tempInt > definitions.size()) {
            cout << "���� �����������, ��������� �����\n";
            tempInt = inputInt();
        }
        if(tempInt == 0)
            mainLog.print("-- User chose not to save definition");
        else {
            mainLog.print("-- User chose to save definition");
            currentRequest.setDef(definitions[tempInt - 1]);
            mainOutput.updateWordInfo(-1, currentRequest);
        }   
    }
    dictionaryFile.close();
    mainLog.print("F= File dictionary??.txt closed");

    ConsoleOutputBlock selectFinal("\n�������:", 5,
        { "����� ������ �� �������" }, { 1 });
    if (currentRequest.forms.empty() == true)
        selectFinal.addMessage("������ ������� �����", 5);
    selectFinal.addMessage("����� ������ ������� - ����� � ����", -1);
    selectFinal.print();
    switch (selectFinal.inputChoice()) {
    case menuCode::DEFAULT: {
        currentRequest.setCode(menuCode::DEFAULT);
        break;
    }
    case menuCode::DICTIONARYSEARCH: {
        currentRequest.setCode(menuCode::DICTIONARYSEARCH);
        break;
    }
    case menuCode::OUT_FORMANALYSIS: {
        currentRequest.setCode(menuCode::OUT_FORMANALYSIS);
        break;
    }
    default: {
        currentRequest.setCode(menuCode::DEFAULT);
        break;
    }
    }
    mainLog.print("== End: void searchInDictionary()");
}

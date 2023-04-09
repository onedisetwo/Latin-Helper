//-----------------------------------------------------------------------
// Header           : latin.h
// Last modified    : 01.04.2023
// Version          : 10
// Description      : Contains main project classes, reference to
//                    functions in "latin.cpp" and global variables
//-----------------------------------------------------------------------
#ifndef LATIN_H_INCLUDED
#define LATIN_H_INCLUDED
//-----------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <conio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <map>
//-----------------------------------------------------------------------
typedef uint16_t uint;
namespace menuCode {
    const int       DEFAULT = 0,  // Return to menu
                    DICTIONARYSEARCH = 1,  // Start dictionarySearch with empty currentRequest
                    FORMANALYSIS = 2,  // Start formanalysis with empty currentRequest
                    OUTPUTACTION = 3,  // Start mainOutput.action
                    OUT_DICTIONARYSEARCH = 4,  // Start dictionarySearch with not empty currentRequest
                    OUT_FORMANALYSIS = 5,  // Start formAnalysis with not empty currentRequest
                    QUIT = -21;  // End program
}
using namespace std;
//-----------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------
void dictionarySearch(string DICTIONARYLR_PATH, string DICTIONARYRL_PATH);
void formAnalysis();

void openFile(fstream& file, string path, ios::openmode mode, bool isLogNote);
int charToInt(char c);
string intToStr(int a, int len);
int inputInt();

// Put several chars/strings into one string
template <class T> string add(vector<T> vec) {
    string result;
    for (auto i : vec)
        result += i;
    return result;
}

//-----------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------

// Logging class
class Log    {
private:
    fstream file_;
    string PATH_;
    bool isStartNotepadLog_ = false;
public:
    // Constructor opens file and starts logging
    Log(string PATH) {
        PATH_ = PATH;
        file_.open(PATH_, ios::ate | ios::app);
        if (file_.is_open() == false) {
            file_.open(PATH_, ios::out);
        }
        auto start = std::chrono::system_clock::now();
        auto legacyStart = std::chrono::system_clock::to_time_t(start);
        char str[26];
        ctime_s(str, sizeof str, &legacyStart);
        file_ << "\n-#- Start  processing -#-\n" << str << endl;
    }
    // Destructor ends logging and closes file
    ~Log() {
        auto start = std::chrono::system_clock::now();
        auto legacyStart = std::chrono::system_clock::to_time_t(start);
        char str[26];
        ctime_s(str, sizeof str, &legacyStart);
        file_ << endl << "#-# Finish processing #-#\n" << str << endl;
        file_.close();
        if(isStartNotepadLog_ == true)
            system("start notepad.exe log.txt");
    }
    // Prints single line to a log
    void print(string message) {
        file_ << message << endl;
    }
    // Set new isStartNotepadLog
    // Default isStartNotepadLog = false
    void setIsStartNotepadLog(bool b) {
        isStartNotepadLog_ = b;
    }
    // Prints single line(a string given by its constituents) to a log
    void print(vector<string> vMessage) {
        file_ << add<string>(vMessage) << endl;
    }
    // Terminates the program with a message with exit code
    void noteExit(int code) {
        //mainOutput.writeData();
        print({ "!!! Program finished with exit code: ", intToStr(code, 4) });
        exit(code);
    }
};

extern Log mainLog;

// Class for simpler menu console output
// Example:
// ConsoleOutputBlock("Select:", 5, {"First option", "Second option", "Esc - Third option"}, {1, 2, -1});
//    | Select:
//    |      1 - First option
//    |      2 - Second option
//    |      Esc - Third option
class ConsoleOutputBlock {
private:
    string headerMessage_;
    vector<string> vMessages_;
    vector<int> vNumbers_;
    uint space_;
    char filler_ = ' ';
public:
    // vMessage and vNumbers sizes must be equal!
    // Set number = -1 if you want to add atypical output
    ConsoleOutputBlock(string headerMessage, uint space, vector<string> vMessages, vector<int> vNumbers) {
        headerMessage_ = headerMessage;
        space_ = space;
        vMessages_ = vMessages;
        vNumbers_ = vNumbers;
        if (vNumbers.size() != vMessages.size()) {
            vMessages_.resize(max(vNumbers.size(), vMessages.size()));
            vNumbers_.resize(max(vNumbers.size(), vMessages.size()));
        }
    }
    // Add new message and its number
    void addMessage(string addedMesage, int addedNumber) {
        vMessages_.push_back(addedMesage);
        vNumbers_.push_back(addedNumber);
    }
    // Prints ConsoleOutputBlock (see example)
    void print() {
        if (headerMessage_.empty() == 0) {
            cout << headerMessage_ << endl;
        }
        for (int i = 0; i < vMessages_.size(); i++) {
            string spaceString;
            spaceString.assign(space_, filler_);
            if (vNumbers_[i] != -1)
                cout << spaceString << vNumbers_[i] << " - " << vMessages_[i] << endl;
            else
                cout << spaceString << vMessages_[i] << endl;
        }
    }
    // Sets new filler_
    void setFiller(char c) {
        filler_ = c;
    }
    // User input related to block
    int inputChoice() {
        char c = _getch();
        return (int)(c)-48;
    }
};

// Сlass for user requests
class WordInfo {
public:
    string  word,
            definition;
    vector<string> forms;
    int code;

    // Compare two WordInfo, if word && (definitions || forms) are equal returns true,
    // else returns false
    friend bool operator==(const WordInfo wi1, const WordInfo wi2) {
        if (wi1.word == wi2.word &&
            (wi1.definition == wi2.definition || wi1.forms == wi2.forms))
            return true;
        else
            return false;
    }

    // Clears WordInfo
    void clear() {
        mainLog.print("=G= currentRequest.word cleared");
        word.clear();
        definition.clear();
        forms.clear();
    }
    // Checks if a WordInfo is empty
    bool empty() {
        if (word.empty() == true &&
            definition.empty() == true &&
            forms.empty() == true)
            return true;
        else
            return false;
    }

    // Input new word by the user, if word is Latin - returns true,
    // else - returns false
    bool inputWord() {
        cin >> word;
        while (word[0] == ' ')
            word.erase(word.begin());
        bool isLat = ((int)(word[0]) >= 97 && (int)(word[0]) <= 122);
        bool isRus = ((int)(word[0]) >= -32 && (int)(word[0]) <= -1);
        if (isLat == isRus) {
            cout << "Слово введено некорректно, повторите ввод: ";
            return inputWord();
        }
        else {
            for (int i = 0; i < word.size(); i++) {
                if (isLat == true) {
                    if ((int)(word[i]) < 97 || (int)(word[i]) > 122 == true) {
                        cout << "Слово введено некорректно, повторите ввод: ";
                        return inputWord();
                        break;
                    }
                }
                if (isRus == true) {
                    if ((int)(word[i]) < -32 || (int)(word[i]) > -1 == true) {
                        cout << "Слово введено некорректно, повторите ввод: ";
                        return inputWord();
                        break;
                    }
                }
            }
        }
        mainLog.print({ "=G= Input new currentRequest.word: ", word });
        return isLat;
    }
    // Set .word members with logging
    void setWord(string s) {
        word = s;
        mainLog.print({ "=G= Set new currentRequest.word: ", word });
    }
    // Set .definition with logging
    void setDef(string s) {
        definition = s;
        mainLog.print({ "=G= Set new currentRequest.definition: ", definition });
    }
    // Set .forms with logging
    void setForms(vector<string>& vStrings) {
        forms = vStrings;
        mainLog.print("=G= Set new currentRequest.forms");
    }
    // Set .code with logging
    void setCode(int n) {
        code = n;
        mainLog.print({ "=G= Set new currentRequest.code: ", intToStr(code,0) });
    }

    // Add one line to forms
    void addForm(string s) {
        forms.push_back(s);
    }
    // Print forms
    void printForms() {
        if (forms.size() != 0) {
            cout << "Info about forms:\n";
            for (auto i : forms)
                cout << i << endl;
        }
    }
    // Print full WordInfo except code
    void print() {
        cout << endl;
        if(word.empty() == false)
            cout << "Word: " << word << endl;
        if (definition.empty() == false)
            cout << "Definition: " << definition << endl;
        printForms();
    }
};

extern WordInfo currentRequest;

// Class for actions with user dictionary
class OutputStream {
private:
    vector<WordInfo> vWords_;
    string PATH_;
    vector<string> userNotes_;
    // Reads data from a file
    void readData() {
        fstream file;
        openFile(file, PATH_, ios::in, true);
        string inputLine;
        while (inputLine != "FILE_END" && file.eof() == false) {
            getline(file, inputLine);
            if (inputLine[0] == '#') {
                WordInfo currWordInfo;
                while (inputLine != "###") {
                    getline(file, inputLine);
                    if (inputLine.substr(0, 5) == "Word:") {
                        inputLine.erase(0, 5);
                        while (inputLine[0] == ' ')
                            inputLine.erase(inputLine.begin());
                        currWordInfo.word = inputLine;
                        continue;
                    }
                    if (inputLine.substr(0, 11) == "Definition:") {
                        inputLine.erase(0, 11);
                        while (inputLine[0] == ' ')
                            inputLine.erase(inputLine.begin());
                        currWordInfo.definition = inputLine;
                        continue;
                    }
                    if (inputLine.substr(0, 6) == "Forms:") {
                        inputLine.erase(0, 5);
                        while (inputLine != "###") {
                            getline(file, inputLine);
                            if (inputLine != "###")
                                currWordInfo.forms.push_back(inputLine);
                        }
                        continue;
                    }
                }
                vWords_.push_back(currWordInfo);
            }
        }
        while (file.eof() == false) {
            getline(file, inputLine);
            if (inputLine != "Место для заметок:")
                userNotes_.push_back(inputLine);
        }
        file.close();
        mainLog.print("= Data from user dictionary was read");
    }
    // Writes data to a file
    void writeData() {
        fstream file;
        openFile(file, PATH_, ios::out, true);
        for (int i = 0; i < vWords_.size(); i++) {
            file << "\n# " << i + 1 << endl;
            file << "Word: " << vWords_[i].word << endl;
            if (vWords_[i].definition.empty() == false)
                file << "Definition: " << vWords_[i].definition << endl;
            if (vWords_[i].forms.empty() == false) {
                file << "Forms:\n";
                for (auto j : vWords_[i].forms) {
                    file << j << endl;
                }
            }
            file << "###\n";
        }
        file << "\nFILE_END\nМесто для заметок:\n";
        for (auto i : userNotes_) {
            file << i << endl;
        }
        file.close();
        mainLog.print("= User dictionary file was rewrited");
    }
    // Сhecks the validity of the file and creates it if it does not exist
    void makeFile() {
        fstream file;
        file.open(PATH_, ios::in);
        if (file.is_open() == false)
            file.open(PATH_, ios::out);
        file.close();
        mainLog.print("= User dictionary file was made");
    }
    // Deletes the user-selected member of WordInfo
    void deleteWordInfoElement(WordInfo& currWordInfo) {
        mainLog.print("== Start of: OutputStream::deleteWordInfoElement");
        const int   WORD = 1,
            DEFINITION = 2,
            FORMS = 3;
        currWordInfo.print();
        ConsoleOutputBlock deleteMod("Выберите элемент, который хотели бы удалить:", 5,
            { }, { });
        if (currWordInfo.word.empty() == false)
            deleteMod.addMessage("Слово", 1);
        if (currWordInfo.definition.empty() == false)
            deleteMod.addMessage("Определение", 2);
        if (currWordInfo.forms.empty() == false)
            deleteMod.addMessage("Информация о формах", 3);
        deleteMod.addMessage("Любая другая клавиша - выход", -1);
        deleteMod.print();
        switch (deleteMod.inputChoice()) {
        case WORD: {
            if (currWordInfo.word.empty() == false) {
                currWordInfo.word.clear();
                cout << "Слово удалено\n";
            }
            break;
        }
        case DEFINITION: {
            if (currWordInfo.definition.empty() == false) {
                currWordInfo.definition.clear();
                cout << "Определение удалено\n";
            }
            break;
        }
        case FORMS: {
            if (currWordInfo.forms.empty() == false) {
                currWordInfo.forms.clear();
                cout << "Информация о формах удалена\n";
            }
            break;
        }
        }
        mainLog.print("== End of: OutputStream::deleteWordInfoElement");
    }
    // Changes the user-selected member of WordInfo
    void changeWordInfo(WordInfo& currWordInfo) {
        mainLog.print("== Start of: OutputStream::changeWordInfo");
        const int   WORD = 1,
            DEFINITION = 2,
            FORMS = 3;
        system("cls");
        currWordInfo.print();
        ConsoleOutputBlock changeMod("Выберите элемент, который хотели бы изменить или добавить:", 5,
            { "Слово","Определение слова","Формы слова","Любая другая клавиша - выход" }, { 1,2,3,-1 });
        changeMod.print();
        switch (changeMod.inputChoice()) {
        case WORD: {
            if (currWordInfo.word.empty() == false)
                cout << "Текущее слово: " << currWordInfo.word
                << "\nВведите новое слово:\n";
            else
                cout << "Слово отсутствует, введите новое: ";
            cin >> currWordInfo.word;
            cout << "Новое слово установлено, нажмите любую клавишу, чтобы вернуться в редактор словаря\n";
            _getch();
            break;
        }
        case DEFINITION: {
            if (currWordInfo.definition.empty() == false)
                cout << "Текущее определение: " << currWordInfo.definition
                << "\nВведите новое определение:\n";
            else
                cout << "Определение отсутствует, введите новое: ";
            cin >> currWordInfo.definition;
            cout << "Новое определение установлено, нажмите любую клавишу, чтобы вернуться в редактор словаря\n";
            _getch();
            break;
        }
        case FORMS: {
            int strNumber = -1;
            const int ENDWHILE = 0;
            while (strNumber != ENDWHILE) {
                system("cls");
                if (currWordInfo.forms.empty() == false) {
                    cout << "Текущая информация о форме слова:\n";
                    for (int i = 0; i < currWordInfo.forms.size(); i++) {
                        cout << i + 1 << ": " << currWordInfo.forms[i] << endl;
                    }
                }
                ConsoleOutputBlock selectAction("Нажмите:", 5, { "добавить строку" }, { 1 });
                if (currWordInfo.forms.empty() == false)
                    selectAction.addMessage("изменить существующую строку", 2);
                selectAction.addMessage("Любая другая клавиша - выйти", -1);
                selectAction.print();
                char c = _getch();
                if (c == '1') {
                    if (currWordInfo.forms.empty() == false) {
                        cout << "Введите номер строки, после которой нужно добавить строку, или 0 для выхода: ";
                        cin >> strNumber;
                        if (strNumber == ENDWHILE) { // Cycle breaker
                            break;
                        }
                        else if (strNumber < 1 || strNumber > currWordInfo.forms.size()) {
                            system("cls");
                            cout << "Номер " << strNumber << " не является допустимым значеним, повторите ввод...\n";
                            continue;
                        }
                    }
                    else
                        strNumber = 0;
                    cout << "Введите новую строку: ";
                    currWordInfo.forms.resize(currWordInfo.forms.size() + 1);
                    if(currWordInfo.forms.size() > 1)
                        for (int i = currWordInfo.forms.size() - 1; i >= strNumber; i--) {
                            currWordInfo.forms[i] = currWordInfo.forms[i - 1];
                        }
                    cin >> currWordInfo.forms[strNumber];
                    cout << "Добавлена новая строка с номером " << strNumber <<
                        ", нажмите 1 для продолжения работы с формами или любую другую клавишу для возвращения в редактор словаря\n";
                    strNumber = -1;
                    switch (_getch()) {
                    case '1':
                        continue;
                    default:
                        strNumber = ENDWHILE;
                        break;
                    }
                }
                if (c == '2') {
                    cout << "Введите номер строки для изменения или 0 для выхода: ";
                    cin >> strNumber;
                    if (strNumber == ENDWHILE) { // Cycle breaker
                        break;
                    }
                    else if (strNumber < 1 || strNumber > currWordInfo.forms.size()) {
                        system("cls");
                        cout << "Номер " << strNumber << " не является допустимым значеним, повторите ввод...\n";
                        continue;
                    }
                    else {
                        cout << "Введите новое значение строки " << strNumber << " (для удаления введите '000'): ";
                        cin >> currWordInfo.forms[strNumber - 1];
                        if (currWordInfo.forms[strNumber - 1] == "000")
                            currWordInfo.forms.erase(currWordInfo.forms.begin() + strNumber - 1);
                        cout << "Строка под номером " << strNumber <<
                            " изменена(удалена), нажмите 1 для продолжения работы с формами или любую другую клавишу для возвращения в редактор словаря\n";
                        switch (_getch()) {
                        case '1':
                            continue;
                        default:
                            strNumber = ENDWHILE;
                            break;
                        }
                    }
                }
                break;
            }
            break;
        }
        default:
            break;
        }
        mainLog.print("== End of: OutputStream::changeWordInfo");
    }
    // User makes new WordInfo and add it to vWords_
    void makeNewWordInfo() {
        mainLog.print("== Start of: OutputStream::makeNewWordInfo");
        WordInfo currWordInfo;
        cout << "Введите слово: ";
        currWordInfo.inputWord();
        string s;
        cout << "Введите определение ('-' в случае отсутствия): ";
        cin >> s;
        if (s != "-")
            currWordInfo.definition = s;
        cout << "Введите информацию о формах ('-' в случае отсутствия):\n";
        cin >> s;
        while (s != "-") {
            currWordInfo.addForm(s);
            cin >> s;
        }
        vWords_.push_back(currWordInfo);
        mainLog.print("== End of: OutputStream::makeNewWordInfo");
    }
public:
    // Set path to output file and read info form it
    OutputStream(string PATH) {
        PATH_ = PATH;
        makeFile();
        readData();
        mainLog.print("=C OutputStream");
    }
    // Write finished vWords_ to file
    ~OutputStream() {
        writeData();
        mainLog.print("=D OutputStream");
    }
    // Clears file
    void clear() {
        vWords_.clear();
        writeData();
        mainLog.print("=G= OutputStream was cleared");
    }
    // Updates WordInfo in vWords_ using its number
    // numWordInfo is a number of updated WordInfo
    // numWordInfo = -1 if add new WordInfo at the end of vWords_ with replacement
    // numWordInfo = -2 if add new WordInfo without relacement
    void updateWordInfo(int numWordInfo, WordInfo &currWordInfo) {
        if (numWordInfo == -1) {
            if (vWords_.empty() == false && currWordInfo == vWords_[vWords_.size() - 1])
                vWords_[vWords_.size() - 1] = currWordInfo;
            else
                vWords_.push_back(currWordInfo);
        }
        else if (numWordInfo == -2)
            vWords_.push_back(currWordInfo);
        else
            vWords_[numWordInfo] = currWordInfo;
        mainLog.print("== Happened: updateWordInfo");
    }
    // Function for user actions with a user dictionary
    void action() {
        mainLog.print("= Start of: mainOutput.action");
        system("cls");
        cout << "Открыт редактор словаря\n\n";
        if (vWords_.empty() == true) {
            cout << "В данный момент словарь пуст, нажмите 1 - для добавления нового слова\n" <<
                "     или любую другую клавишу для возвращения в меню\n";
            currentRequest.setCode(0);
            char c = _getch();
            if (c == '1')
                makeNewWordInfo();
            else {
                currentRequest.setCode(menuCode::DEFAULT);
                return void();
            }
        }
        for (int i = 0; i < vWords_.size(); i++) {
            cout << "# " << i + 1;
            vWords_[i].print();
        }
        const int   DELETEWORDINFO = 1,
                    DELETEWORDINFOELEMENT = 2,
                    CHANGEWORDINFO = 3,
                    MAKENEWWORDINFO = 6,
                    CLEAROUTPUT = 7;
        ConsoleOutputBlock editMod("\nВыберите действие:", 5,
            { "Удалить всю информацию о слове","Удалить отдельный элемент информации о слове",
            "Изменить информацию о слове","Найти перевод выбранного слова", 
            "Определить форму выбранного слова", "Добавить новое слово", "Очистить словарь",
            "Любая другая клавиша - Выйти из режима редактирования" },
            { 1,2,3,4,5,6,7,-1 });
        editMod.print();
        switch (editMod.inputChoice()) {
        case DELETEWORDINFO: {
            cout << "Выберите номер слова для удаления\n";
            int numWordInfo = inputInt();
            if (numWordInfo > 0 && numWordInfo <= vWords_.size())
                vWords_.erase(vWords_.begin() + numWordInfo - 1);
            else
                cout << "Введён некорректный номер\n";
            break;
        }
        case DELETEWORDINFOELEMENT: {
            cout << "Выберите номер слова для удаления элемента\n";
            int numWordInfo = inputInt();
            if (numWordInfo > 0 && numWordInfo <= vWords_.size()) {
                deleteWordInfoElement(vWords_[numWordInfo - 1]);
                if (vWords_[numWordInfo - 1].empty() == true)
                    vWords_.erase(vWords_.begin() + numWordInfo - 1);
            }
            else
                cout << "Введён некорректный номер\n";
            break;
        }
        case CHANGEWORDINFO: {
            cout << "Выберите номер слова для изменения\n";
            int numWordInfo = inputInt();
            if (numWordInfo > 0 && numWordInfo <= vWords_.size())
                changeWordInfo(vWords_[numWordInfo - 1]);
            else
                cout << "Введён некорректный номер\n";
            break;
        }
        case MAKENEWWORDINFO: {
            makeNewWordInfo();
            cout << "Добавлено новое слово:\n";
            vWords_[vWords_.size() - 1].print();
            break;
        }
        case menuCode::OUT_DICTIONARYSEARCH: {
            while (true) {
                cout << "Введите номер слова для поиска определения\n";
                int numWordInfo = inputInt();
                if (numWordInfo == -1) // Cycle breaker
                    break;
                if (numWordInfo > 0 && numWordInfo <= vWords_.size()) {
                    currentRequest = vWords_[numWordInfo - 1];
                    break;
                }
                else {
                    cout << "Введён некорректный номер, повторите ввод или введите -1 для возвращения в редактор\n";
                    continue;
                }
                currentRequest.setCode(menuCode::OUT_DICTIONARYSEARCH);
                return void();
            }
            break;
        }
        case menuCode::OUT_FORMANALYSIS: {
            while (true) {
                cout << "Введите номер слова для анализа формы\n";
                int numWordInfo = inputInt();
                if (numWordInfo == -1) // Cycle breaker
                    break;
                if (numWordInfo > 0 && numWordInfo <= vWords_.size())
                    currentRequest = vWords_[numWordInfo - 1];
                else {
                    cout << "Введён некорректный номер, повторите ввод или введите -1 для возвращения в редактор\n";
                    continue;
                }
                if (currentRequest.word[0] < 0) {
                    cout << "Недопустимое для анализа формы слово: выберите слово, " << 
                        "состоящее только из букв латиснкого алфавита или -1 для возвращения в редактор\n";
                    continue;
                }
                currentRequest.setCode(menuCode::OUT_FORMANALYSIS);
                return void();
            }
            break;
        }
        case CLEAROUTPUT: {
            clear();
            cout << "Словарь очищен, нажмите любую клавишу, чтобы вернуться в меню\n";
            _getch();
            return void();
        }
        default: {
            currentRequest.setCode(0);
            writeData();
            return void();
        }
        }
        cout << "Нажмите 1 для нового запроса или любую другую клавишу для выхода в меню\n";
        char c = _getch();
        if (c == '1')
            currentRequest.setCode(menuCode::OUTPUTACTION);
        else {
            currentRequest.setCode(menuCode::DEFAULT);
        }
        writeData();
        mainLog.print("= End of: mainOutput.action");
    }  
};

extern OutputStream mainOutput;

//-----------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------

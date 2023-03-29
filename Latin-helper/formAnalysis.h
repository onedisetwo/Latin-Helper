//-----------------------------------------------------------------------
// Header           : formAnalysis.h
// Last modified    : 27.03.2023
// Version          : 9
// Description      : Library for "formsAnalysis.cpp".
//                    Must be included once, because contains function
//-----------------------------------------------------------------------
#ifndef FORMANALYSIS_H_INCLUDED
#define FORMANALYSIS_H_INCLUDED
//-----------------------------------------------------------------------
#include "latin.h"
#include "formAnalysisData.h"
//-----------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------

// Checks if a string contains another string
bool isContain(string container, string desired) {
    if (container.find(desired) != string::npos)
        return true;
    else
        return false;
}

// Class with word form information
class CForm {
private:
    char    genus_,    // Род
            numerus_,  // Число
            casus_;    // Падеж

public:
    // Constructors
    CForm() {};
    CForm(char numerus, char casus) {
        numerus_ = numerus;
        casus_ = casus;
    }
    CForm(char genus, char numerus, char casus) {
        genus_ = genus;
        numerus_ = numerus;
        casus_ = casus;
    }

    void setGenus(char genus) {
        genus_ = genus;
    }
    void setNumerus(char numerus) {
        numerus_ = numerus;
    }
    void setCasus(char casus) {
        casus_ = casus;
    }

    char getGenus() {
        return genus_;
    }
    char getNumerus() {
        return numerus_;
    }
    char getCasus() {
        return casus_;
    }
};

class Noun {
private:
    // Info about data
    const string  KEYWORD_ = "INFO_NOUN";       // Keyword to access path to file
    NounInfo INFO;

    // Word characteristics
    string  word_,
            standartEnding_;
    char    declinatio_,
            genus_;

    vector<string> formsString_;
    vector<CForm> formsCForm_;

    // A function that returns true if there was error and determines it's type
    bool isError(int code) {
        if (code == 100)
            return false;
        string message;
        if (code % 10 == 101) {
            message = "Окончание введено неверно";
        }
        if (code == 1020) {
            message = "Род и склонение несовместимы";
        }
        if (code == 1030) {
            message = "Данной формы не найдено";
        }
        cout << "\nОшибка: \n" << message
            << " (" << code << ")\n";
        mainLog.print({ "===C! ", intToStr(code,4) });
        return true;
    }

    // Declination function
    int determineDeclinatio() {
        mainLog.print("===C= Start: int determineDeclinatio()");
        string infoLine = INFO.declinatio_;
        if (standartEnding_.size() == 1 && isContain(infoLine, standartEnding_) == 0) {
            return 1010;
        }
        else if (standartEnding_.size() == 2) {
            if (isContain(infoLine, standartEnding_) == 0)
                standartEnding_ = standartEnding_.substr(1, 1);
            if (isContain(infoLine, standartEnding_) == 0)
                return 1011;
        }
        else {
            if (isContain(infoLine, standartEnding_) == 0)
                standartEnding_ = standartEnding_.substr(standartEnding_.size() - 2, 2);
            if (isContain(infoLine, standartEnding_) == 0)
                standartEnding_ = standartEnding_.substr(1, 1);
            if (isContain(infoLine, standartEnding_) == 0)
                return 1012;
        }
        declinatio_ = infoLine[infoLine.find(standartEnding_) - 1];

        mainLog.print("===C= End: int determineDeclinatio() returned 100");
        return 100;
    }

    // Form function
    int determineForm() {
        mainLog.print("===C= Start: int determineForm()");
        vector<string>::iterator infoIter = INFO.form_.begin();
        while (true) {
            if ((*infoIter).find(add<char>({ declinatio_,genus_ })) != string::npos) {
                break;
            }
            if (infoIter == INFO.form_.end())
                return 1020;
            infoIter++;
        }
        infoIter++;
        // Get info about endings from file
        CForm tempForm;
        int beg = -1;
        for (size_t currPos = 0; currPos < (*infoIter).size(); currPos++) {
            if (((*infoIter)[currPos] == ',' || (*infoIter)[currPos] == ';') && beg != -1) {
                size_t tempPos = word_.rfind((*infoIter).substr(beg, currPos - beg));
                //cout << (*infoIter).substr(beg, currPos - beg) << " " << tempPos << endl;
                if (tempPos != string::npos && (tempPos + currPos - beg) == word_.size()) {
                    formsCForm_.push_back(tempForm);
                }
            }
            if ((*infoIter)[currPos] == ',')
                beg = currPos + 1;
            if ((*infoIter)[currPos] == ';' && currPos != (*infoIter).size() - 1) {
                tempForm.setNumerus((*infoIter)[currPos + 1]);
                tempForm.setCasus((*infoIter)[currPos + 2]);
                beg = currPos + 4;
                currPos += 4;
            }
        }
        // Null ending exception
        if (formsCForm_.size() == 0) {
            if (declinatio_ == '3') {
                formsCForm_.push_back(CForm('0', '0'));
                if (genus_ == 'n') {
                    formsCForm_.push_back(CForm('0', '3'));
                    formsCForm_.push_back(CForm('0', '4'));
                }
            }
            else
                return 1030;
        }
        mainLog.print("===C= End: int determineForm() returned 100");
        return 100;
    }

    // Convert info from CForm to strings
    void fillFormsString() {
        mainLog.print("===C= Start: void fillFormsString()");
        map<char, string>::iterator iter;

        iter = INFO.output_[0].find(declinatio_);
        formsString_.push_back(add<string>({ "Declinatio ", (*iter).second }));

        if (genus_ != '-') {
            iter = INFO.output_[1].find(genus_);
            formsString_[formsString_.size()-1] += add<string>({"; Genus ", (*iter).second});
        }

        for (int i = 0; i < formsCForm_.size(); i++) {
            string infoToAdd;

            iter = INFO.output_[2].find(formsCForm_[i].getNumerus());
            infoToAdd += "Numerus ";
            infoToAdd += (*iter).second;
            infoToAdd += ";";

            iter = INFO.output_[3].find(formsCForm_[i].getCasus());
            infoToAdd += " Casus ";
            infoToAdd += (*iter).second;

            formsString_.push_back(infoToAdd);
        }
        mainLog.print("===C= End: void fillFormsString()");
    }

public:

    Noun() {
        mainLog.print("===C+ Noun");
        word_ = currentRequest.word;
        cout << "\n         Введите окончание в gen. sing.: ";
        cin >> standartEnding_;
        cout << "\n  Введите род (m,f,n,- если неизвестен): ";
        cin >> genus_;
        if (isError(determineDeclinatio()) == false)
            if (isError(determineForm()) == false) {
                fillFormsString();
                currentRequest.setForms(formsString_);
            }
    }

    ~Noun() {
        mainLog.print("===C- Noun");
    }
};

class Adjective {
private:
    const string  KEYWORD_ = "INFO_ADJ"; // Keyword to access path to file
    AdjectiveInfo INFO;

    string  word_,
            coreForms_[3],
            root_;
    char    declinatio_;

    vector<string> formsString_;
    vector<CForm> formsCForm_;

    // Input adjective root
    void inputRoot() {
        cout << "\n        Введите основу прилагательного: ";
        cin >> root_;
    }

    // A function that returns true if there was error and determines it's type
    bool isError(int code) {
        if (code == 100)
            return false;
        string message;
        if (code == 1010) {
            message = "Словарная форма введена неверно";
        }
        if (code == 1020) {
            message = "Данной формы не найдено";
        }
        cout << "Ошибка: \n" << message
            << " (" << code << ")\n";
        mainLog.print({ "===C! ",intToStr(code,4) });
        return true;
    }

public:
    // Declination function
    int determineDeclinatio() {
        mainLog.print("===C= Start: int determineDeclinatio()");
        vector<string>::iterator infoIter = INFO.declinatio_.begin();

        string  endingMasculine,
            coreEndings[4];

        endingMasculine = coreForms_[0].substr(coreForms_[0].size() - 2, 2);
        coreForms_[2] += " ";
        declinatio_ = '1';
        while (true) {
            declinatio_++;
            if ((*infoIter)[0] == '.') {
                infoIter++;
                coreEndings[0] = (*infoIter).substr(0, 2);
                coreEndings[1] = (*infoIter).substr(2, 2);
                coreEndings[2] = (*infoIter).substr(4, 2);
                coreEndings[3] = (*infoIter).substr(6, 2);
                if (endingMasculine == coreEndings[0] ||
                    endingMasculine == coreEndings[1])
                    if (coreForms_[2].find(coreEndings[3]) != string::npos) {
                        break;
                    }
            }
            infoIter++;
            if (infoIter == INFO.declinatio_.end()) {
                return 1010;
            }
        }
        coreForms_[2].pop_back();
        mainLog.print("===C= End: int determineDeclinatio() returned 100");
        return 100;
    }

    // Form function
    int determineForm() {
        mainLog.print("===C= Start: int determineForm()");
        vector<string>::iterator infoIter = INFO.form_.begin();

        while (infoIter != INFO.form_.end()) {
            if ((*infoIter)[0] == 'd' && (*infoIter)[1] == declinatio_) {
                do {
                    CForm currentForm;
                    infoIter++;
                    if ((*infoIter)[0] == 'g') {
                        currentForm.setGenus((*infoIter)[1]);
                        infoIter++;
                    }
                    int begOfInfo = -1;
                    for (size_t currPos = 0; currPos < (*infoIter).size(); currPos++) {
                        if (((*infoIter)[currPos] == ',' || (*infoIter)[currPos] == ';') && begOfInfo != -1) {
                            size_t tempPos = word_.rfind((*infoIter).substr(begOfInfo, currPos - begOfInfo));
                            if (tempPos != string::npos && (tempPos + currPos - begOfInfo) == word_.size())
                                formsCForm_.push_back(currentForm);
                        }
                        if ((*infoIter)[currPos] == ',')
                            begOfInfo = currPos + 1;
                        if ((*infoIter)[currPos] == ';' && currPos != (*infoIter).size() - 1) {
                            currentForm.setNumerus((*infoIter)[currPos + 1]);
                            currentForm.setCasus((*infoIter)[currPos + 2]);
                            begOfInfo = currPos + 4;
                            currPos += 4;
                        }
                    }
                } while ((*infoIter)[0] != 'd' && (*infoIter)[0] != '_');
                break;
            }
            infoIter++;
        }

        // Null ending exception
        if (formsCForm_.size() == 0) {
            inputRoot();
            if (word_ == root_ && declinatio_ == '5')
                formsCForm_ = { CForm('b','0','0'),
                               CForm('n','0','0'),
                               CForm('b','0','3') };
            else {
                return 1020;
            }
        }
        mainLog.print("===C= End: int determineForm() returned 100");
        return 100;
    }

    // Convert info from CForm to strings
    void fillFormsString() {
        mainLog.print("===C= Start: void fillFormsString()");
        map<char, string>::iterator iter;

        iter = INFO.output_[0].find(declinatio_);
        formsString_.push_back(add<string>({ "Declinatio ", (*iter).second }));

        for (int i = 0; i < formsCForm_.size(); i++) {
            string infoToAdd;

            iter = INFO.output_[1].find(formsCForm_[i].getGenus());
            infoToAdd += "Genus ";
            infoToAdd += (*iter).second;
            infoToAdd += ";";

            iter = INFO.output_[2].find(formsCForm_[i].getNumerus());
            infoToAdd += "Numerus ";
            infoToAdd += (*iter).second;
            infoToAdd += ";";

            iter = INFO.output_[3].find(formsCForm_[i].getCasus());
            infoToAdd += "Casus ";
            infoToAdd += (*iter).second;

            formsString_.push_back(infoToAdd);
        }

        mainLog.print("===C= End: void fillFormsString()");
    }

    Adjective() {
        mainLog.print("===C+ Adjective");
        word_ = currentRequest.word;
        cout << "\n           Введите словарную форму слова: ";
        cin >> coreForms_[0] >> coreForms_[1] >> coreForms_[2];
        if (isError(determineDeclinatio()) == false)
            if (isError(determineForm()) == false) {
                fillFormsString();
                currentRequest.setForms(formsString_);
            }
    }

    ~Adjective() {
        mainLog.print("===C- Adjective");
    }
};

class Verb {
private:
    const string  KEYWORD_ = "INFO_VERB";
    VerbInfo INFO;

    string  word_,
            coreForms_[4];
    char    coniugatio_;

    vector<string> formsString_;
    vector<CForm> formsCForm_;

    bool isError(int code) {
        if (code == 100)
            return false;
        string message;

        cout << "\nОшибка: \n" << message
            << " (" << code << ")\n";
        mainLog.print({ "===C! ", intToStr(code,4) });
        return true;
    }

public:

    void determineConiugatio() {
        string infectRoot;
        if (coreForms_[3].substr(coreForms_[3].size() - 3, 3) == "are") {
            infectRoot = coreForms_[3].substr(0, coreForms_[3].size() - 2);
        }
        else {
            infectRoot = coreForms_[0].substr(0, coreForms_[0].size() - 1);
        }
        char last = *infectRoot.end();
        switch (last) {
        case 'a': {
            coniugatio_ = '1';
            break;
        }
        case 'e': {
            coniugatio_ = '2';
            break;
        }
        case 'i': {
            coniugatio_ = '4';
            break;
        }
        default: {
            coniugatio_ = '3';
            break;
        }
        }
    }

    Verb() {
        mainLog.print("===C+ Verb");
        cout << "Введите 4 словарные формы:\n";
        for (int i = 0; i < 4; i++) {
            cin >> coreForms_[i];
        }
        determineConiugatio();
        cout << "Coniugatio " << coniugatio_ << endl;
    }

    ~Verb() {
        mainLog.print("===C- Verb");
    }
};

//-----------------------------------------------------------------------
#endif // FORMANALYSIS_INCLUDED
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Source           : latin.cpp
// Last modified    : 31.03.2023
// Version          : 10
// Description      : Contains functions from "libs/latin.h"
//-----------------------------------------------------------------------

#include "latin.h"

using namespace std;

// Ensures safe opening of the file
void openFile(fstream& file, string path, ios::openmode mode, bool isLogNote) {
    file.open(path, mode);
    if (!file.is_open()) {
        string output = "Не удалось открыть файл ";
        output += path;
        cout << output;
        _getch();
        mainLog.print({ "=F= !!! Unable to open ", path });
        mainLog.noteExit(9909);
    }
    else if (isLogNote == true) {
        mainLog.print({ "=F= File ", path, " opened successfully" });
    }
}

// Converts char to int
int charToInt(char c) {
    return (int)(c - 48);
}

// Converts int to string
// minLength - minimum length of string convertation
// # intToStr(43, 4) = "0043"
// When you don't need a specific length recommended to minLength = 0
string intToStr(int num, int minLength) {
    string result;
    bool isNegative = false;
    if (num == 0)
        return "0";
    if (num < 0) {
        isNegative = true;
        num = -num;
    }
    while (num > 0) {
        result += (char)(num % 10 + 48);
        num /= 10;
    }
    while (result.size() < minLength) {
        result += '0';
    }
    if (isNegative == true) {
        result += '-';
    }
    reverse(result.begin(), result.end());
    return result;
}

// Returns integer entered by the user
int inputInt() {
    int tempInt;
    cin >> tempInt;
    return tempInt;
}

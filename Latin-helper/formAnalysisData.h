//-----------------------------------------------------------------------
// Header           : formAnalysisData.h
// Last modified    : 24.03.2023
// Version          : 9
// Description      : Contains necessary data for partsAnalysis.h
//-----------------------------------------------------------------------
#ifndef FORMANALYSISDATA_H_INCLUDED
#define FORMANALYSISDATA_H_INCLUDED
//-----------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <map>
//-----------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------
struct NounInfo {
    string declinatio_;
    vector<string> form_;
    map<char, string> output_[4];
    void initDeclinatio() {
        declinatio_ = "1ae2i3is4us5ei";
    }
    void initForm() {
        form_.resize(16);
        form_[0] = "1f 1-";
        form_[1] = ";00-a;01-ae;02-ae;03-am;04-a;10-ae;11-arum;12-is;13-as;14-is;";
        form_[2] = "2m 2-";
        form_[3] = ";00-us,er,um;01-i;02-o;03-um;04-o;10-i;11-orum;12-is;13-os;14-is;";
        form_[4] = "2n 2-";
        form_[5] = ";00-us,er,um;01-i;02-o;03-um;04-o;10-a;11-orum;12-is;13-os;14-is;";
        form_[6] = "3m 3f 3-";
        form_[7] = ";00- ;01-is;02-i;03-em;04-e;10-es;11-um,ium;12-ibus;13-es;14-es;";
        form_[8] = "3n 3-";
        form_[9] = ";00- ;01-is;02-i;03- ;04- ,i;10-a,ia;11-um,ium;12-ibus;13-a,ia;14-a,ia;";
        form_[10] = "4m 4-";
        form_[11] = ";00-us;01-us;02-ui;03-um;04-u;10-us;11-uum;12-ibus;13-us;14-us;";
        form_[12] = "4n 4-";
        form_[13] = ";00-u;01-us;02-u;03-u;04-u;10-ua;11-uum;12-ibus;13-ua;14-ua;";
        form_[14] = "5f 5-";
        form_[15] = ";00-es;01-ei;02-ei;03-em;04-e;10-es;11-erum;12-ebus;13-es;14-es;";
    }
    void initOutput() {
        output_[0] = { {'1', "prima"}, {'2', "secunda"}, {'3', "tertia"},
                        {'4', "quarta"}, {'5', "quinta"} };
        output_[1] = { {'m', "masculinum"}, {'f', "femininum"}, {'n', "neutrum"} };
        output_[2] = { {'0', "singularis"}, {'1', "pluralis"} };
        output_[3] = { {'0', "Nominativus/Vocativus"}, {'1', "Genitivus"},
                        {'2', "Dativus"}, {'3', "Accusativus"}, {'4', "Ablativus"} };
    }
    void getOutput(map<char, string> output[4]) {
        for (int i = 0; i < 4; i++) {
            output[i] = output_[i];
        }
    }
    NounInfo() {
        initDeclinatio();
        initForm();
        initOutput();
    }
};

struct AdjectiveInfo {
    vector<string> declinatio_;
    vector<string> form_;
    map<char, string> output_[4];
    void initDeclinatio() {
        declinatio_.resize(8);
        declinatio_[0] = ".12";
        declinatio_[1] = "useraum ";
        declinatio_[2] = ".31";
        declinatio_[3] = "ererise ";
        declinatio_[4] = ".32";
        declinatio_[5] = "isisise ";
        declinatio_[6] = ".33";
        declinatio_[7] = "isisisis";
    }
    void initForm() {
        form_.resize(24);
        form_[0] = "d2";
        form_[1] = "gm";
        form_[2] = ";00-us,er;01-i;02-o;03-um;04-o;10-i;11-orum;12-is;13-os;14-is;";
        form_[3] = "gf";
        form_[4] = ";00-a;01-ae;02-ae;03-am;04-a;10-ae;11-arum;12-is;13-as;14-is;";
        form_[5] = "gn";
        form_[6] = ";00-um;01-i;02-o;03-um;04-o;10-a;11-orum;12-is;13-os;14-is;";
        form_[7] = "d3";
        form_[8] = "gm";
        form_[9] = ";00-er;01-is;02-i;03-em;04-i;10-es;11-ium;12-ibus;13-es;14-ibus;";
        form_[10] = "gf";
        form_[11] = ";00-is;01-is;02-i;03-em;04-i;10-es;11-ium;12-ibus;13-es;14-ibus;";
        form_[12] = "gn";
        form_[13] = ";00-e;01-is;02-i;03-e;04-i;10-ia;11-ium;12-ibus;13-ia;14-ibus;";
        form_[14] = "d4";
        form_[15] = "gb";
        form_[16] = ";00-is;01-is;02-i;03-em;04-i;10-es;11-ium;12-ibus;13-es;14-ibus;";
        form_[17] = "gn";
        form_[18] = ";00-e;01-is;02-i;03-e;04-i;10-ia;11-ium;12-ibus;13-ia;14-ibus;";
        form_[19] = "d5";
        form_[20] = "gb";
        form_[21] = ";00- ;01-is;02-i;03-em;04-i;10-es;11-ium;12-ibus;13-es;14-ibus;";
        form_[22] = "gn";
        form_[23] = ";00- ;01-is;02-i;03- ;04-i;10-es;11-ium;12-ibus;13-ia;14-ibus;";
    }
    void initOutput() {
        output_[0] = { {'2', "I-II"}, {'3', "III (type 1)"}, {'4', "III (type 2)"},
                        {'5', "III (type 3)"} };
        output_[1] = { {'m', "masculinum"}, {'f', "femininum"}, {'n', "neutrum"} };
        output_[2] = { {'0', "singularis"}, {'1', "pluralis"} };
        output_[3] = { {'0', "Nominativus"}, {'1', "Genitivus"},
                        {'2', "Dativus"}, {'3', "Accusativus"}, {'4', "Ablativus"} };
    }
    AdjectiveInfo() {
        initDeclinatio();
        initForm();
        initOutput();
    }
};

struct VerbInfo {
    VerbInfo() {

    }
};
//-----------------------------------------------------------------------
#endif // FORMANALYSISDATA_H_INCLUDED
//-----------------------------------------------------------------------
//
//  lexical_analyzer.cpp
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lexical_analyzer.hpp"
#include "token.hpp"

using namespace std;

Tokens LexicalAnalyzer::analyzeString (string str) {
    if (!(str[0] >= '0' && str[0] <= '9')){

        // 새로운 Identifier가 들어오면 symbol table에 등록해준다.
        // Identifier에 대한 init 값은 null 이다.
        LexicalAnalyzer::_symbolTable.push_back(make_tuple(str, '\0'));
        return IDENT;
    }

    // 모든 문자가 숫자인지 확인    
    string::iterator iter = str.begin();
    for (; iter != str.end(); iter++) {
        if (!('0' <= *iter && *iter <= '9')) {
            // error
        }
    } 

    return CONST;
}


void LexicalAnalyzer::analyzeInputFile(ifstream& inputFile) {
    // 파일 내용 읽기
    vector<tuple<Tokens, string>> lexicalResult;
    string line;
    char c;
    
    while (true)
    {
        inputFile.get(c);
        if (inputFile.fail()) {
            if (!line.empty()) {
                lexicalResult.push_back(make_tuple(analyzeString(line), line));
                line.clear();
                lexicalResult.push_back(make_tuple(END_OF_FILE, ""));
            }
            break;
        }
        
        // white space에 대한 처리
        if (c <= 32) {
            if (!line.empty()) {
                lexicalResult.push_back(make_tuple(analyzeString(line), line));
                line.clear();
            }
        }
        // alphabet, number에 대한 처리
        else if ((48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122)) {
            line += c;
        } 
        // 기호에 대한 처리
        else {
            if (!line.empty()) {
                if (line != ":") {
                    lexicalResult.push_back(make_tuple(analyzeString(line), line));
                    line.clear();
                }
            }
            
            switch (c) {
                case '(':
                    lexicalResult.push_back(make_tuple(LEFT_PAREN, "("));
                    break;
                case ')':
                    lexicalResult.push_back(make_tuple(RIGHT_PAREN, ")"));
                    break;
                case '*':
                    lexicalResult.push_back(make_tuple(MULT_OP, "*"));
                    break;
                case '/':
                    lexicalResult.push_back(make_tuple(MULT_OP, "/"));
                    break;
                case '+':
                    lexicalResult.push_back(make_tuple(ADD_OP, "+"));
                    break;
                case '-':
                    lexicalResult.push_back(make_tuple(ADD_OP, "-"));
                    break;
                case ';':
                    lexicalResult.push_back(make_tuple(SEMI_COLON, ";"));
                    break;
                case ':':
                    line += c;
                    break;
                case '=':
                    line += c;
                    lexicalResult.push_back(make_tuple(ASSIGNMENT_OP, ":="));
                    line.clear();
                    break;
            }
        }
    }
    
    LexicalAnalyzer::_lexResult = lexicalResult;
}

vector<tuple<Tokens, string>> LexicalAnalyzer::getAnalyzedResult() {
    return _lexResult;
}
vector<tuple<string, int>> LexicalAnalyzer::getSymbolTable() {
    return _symbolTable;
}
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
#include <tuple>

#include "lexical_analyzer.hpp"
#include "token.hpp"

using namespace std;

Tokens LexicalAnalyzer::analyzeString (string str) {
    // 숫자로 시작하지 않는 경우
    if (!(str[0] >= '0' && str[0] <= '9')){

        // 새로운 Identifier가 들어오면 symbol table에 등록해준다.
        // Identifier에 대한 init 값은 null 이다.
        LexicalAnalyzer::_symbolTable.push_back(str);
        return IDENT;
    }

    // 숫자로 시작하는 경우, 모든 문자가 숫자인지 확인
    string::iterator iter = str.begin();
    for (; iter != str.end(); iter++) {
        if (!('0' <= *iter && *iter <= '9')) {
            // 문자가 포함되어 있다면, UNKNOWN으로 처리한다.
            return UNKNOWN;
        }
    } 

    return CONST;
}


void LexicalAnalyzer::analyzeInputFile(ifstream& inputFile) {
    // 파일 내용 읽기
    vector<tuple<Tokens, string>> lexicalResult;
    string line;
    char c;
    
    while (inputFile.get(c)){
        // white space에 대한 처리
        if (c <= 32) {
            if (!line.empty()) {
                lexicalResult.push_back(make_tuple(analyzeString(line), line));
                line.clear();
            }
        }

        // alphabet, number, underscore에 대한 처리
        else if (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_') {
            line += c;
        } 

        // 기호에 대한 처리
        else {
            if (!line.empty()) {
                lexicalResult.push_back(make_tuple(analyzeString(line), line));
            }

            line.clear();
            
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
                    lexicalResult.push_back(make_tuple(COLON, ":"));
                    break;
                case '=':
                    lexicalResult.push_back(make_tuple(EQUAL, "="));
                    break;
                default:
                    // cout << "\nDEBUG - UNKNOWN SYMBOL IS ENTERED\n" ;
                    lexicalResult.push_back(make_tuple(UNKNOWN, string(1, c)));
                    break;
            }
        }
    }
    if (!line.empty()) {
        lexicalResult.push_back(make_tuple(analyzeString(line), line));
        line.clear();
    }
    lexicalResult.push_back(make_tuple(END_OF_FILE, ""));
    // DEBUG : Lexical Result 출력하기
    // for (vector<tuple<Tokens, string>>::iterator it = lexicalResult.begin() ; it != lexicalResult.end(); ++it) {
	// 			cout << get<0>(*it) << " " <<get<1>(*it) << '\n';
	// 	}
    
    LexicalAnalyzer::_lexResult = lexicalResult;
}

vector<tuple<Tokens, string>> LexicalAnalyzer::getAnalyzedResult() {
    return _lexResult;
}
vector<string> LexicalAnalyzer::getSymbolTable() {
    return _symbolTable;
}
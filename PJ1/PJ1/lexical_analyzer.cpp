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

// 주어진 문자열에 대해 토큰을 분석하는 함수
Tokens LexicalAnalyzer::analyzeString (string str) {
    
    // 숫자로 시작하지 않는 경우
    if (!(str[0] >= '0' && str[0] <= '9')){

        // 새로운 Identifier가 들어오면 symbol table에 등록
        // Identifier에 대한 init 값은 null 이다.
        if(find(_symbolTable.begin() , _symbolTable.end(), str) == _symbolTable.end()) {
            LexicalAnalyzer::_symbolTable.push_back(str);
        }
        
        // IDENT로 토큰을 분류
        return IDENT;
    }

    // 숫자로 시작하는 경우, 모든 문자가 숫자인지 확인
    // 만약에 문자가 하나라도 포함되어 있다면 UNKNOWN 토큰으로 처리
    for (string::iterator iter = str.begin(); iter != str.end(); iter++) {
        if (!('0' <= *iter && *iter <= '9')) {
            return UNKNOWN;
        }
    }
    // 모든 문자가 숫자로 들어온 경우에는, CONST로 토큰을 분류
    return CONST;
}

void LexicalAnalyzer::analyzeInputFile(ifstream& inputFile) {
    vector<tuple<Tokens, string>> lexicalResult;
    string line;
    char c;

    // 모든 문자를 처리할 때 까지 한 글자씩 아래 로직을 처리함
    while (inputFile.get(c)) {
        // 공백 문자가 들어온 경우에 대한 처리
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

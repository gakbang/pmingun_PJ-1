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
#include "token.h"

using namespace std;

Tokens analyzeString (string str) {
    
    return PROGRAM;
}


vector<tuple<Tokens, string>> analyzeInputFile(ifstream& inputFile) {
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
    return lexicalResult;
}

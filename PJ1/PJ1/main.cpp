//
//  main.cpp
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "parser.hpp"
#include "token.hpp"
#include "lexical_analyzer.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    
    vector<tuple<int, string>> symbolTable;

    // 파일명은 argv[1]에 있습니다.
    const char *filename = argv[1];
    
    // 입력 파일 스트림 생성
    ifstream inputFile(filename);
    
    if (!inputFile) {
        cout << "파일을 열 수 없습니다: " << filename << endl;
        return 0;
    }
    
    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer();

    // Lexical Analyzer 통해 구문 분석 진행
    lexicalAnalyzer.analyzeInputFile(inputFile);
    
    // DEBUG PRINT
    // for (int index = 0; index < lexResult.size(); index++) {
    //     cout << get<0>(lexResult.at(index)) << ' ' << get<1>(lexResult.at(index)) << '\n';
    // }

    Parser parser(lexicalAnalyzer.getAnalyzedResult(), lexicalAnalyzer.getSymbolTable());
    parser.Parse();


    // 파일 닫기 (자동으로 닫히지만 명시적으로 닫아주는 것이 좋습니다)
    inputFile.close();

    return 0;
}

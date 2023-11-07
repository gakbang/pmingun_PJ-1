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
    
    // 파일명은 argv[1]을 통해 전달됨
    const char *filename = argv[1];
    
    // 입력 파일 스트림 생성
    ifstream inputFile(filename);
    if (!inputFile) {
        cout << "[" << filename << "] 파일을 열 수 없습니다" << endl;
        return 0;
    }
    
    // Lexical Analyzer 객체를 생성
    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer();

    // Lexical Analyzer를 통해 토큰 분석 진행
    lexicalAnalyzer.analyzeInputFile(inputFile);
    
   

    // Lexical Analyzer의 구문 분석 결과 -토큰 리스트, 심볼 테이블- 를 Parser에게 전달하며
    // Parser 객체를 생성
    Parser parser(lexicalAnalyzer.getAnalyzedResult(), lexicalAnalyzer.getSymbolTable());
   

    

    // Parser를 통해 구문 분석을 진행
    parser.Parse();

    // 모든 처리가 완료된 후 파일 닫기
    inputFile.close();
    
    return 0;
}

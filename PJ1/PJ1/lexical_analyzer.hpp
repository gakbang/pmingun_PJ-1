//
//  lexical_analyzer.hpp
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#ifndef lexical_analyzer_hpp
#define lexical_analyzer_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

#include "token.hpp"

using namespace std;

class LexicalAnalyzer {
  private:
  vector<tuple<Tokens, string>> _lexResult;
  vector<tuple<string, int>> _symbolTable;

  public: 
  Tokens analyzeString (string);
  void analyzeInputFile(ifstream&);
  vector<tuple<Tokens, string>> getAnalyzedResult();
  vector<tuple<string, int>> getSymbolTable();
};



#endif /* lexical_analyzer_hpp */

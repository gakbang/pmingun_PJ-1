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

class OptionalInt {
public:
	bool isNull;
	int data;
	OptionalInt() :isNull(false), data(0) {}

};

class LexicalAnalyzer {
  private:
  vector<tuple<Tokens, string>> _lexResult;
  vector<string> _symbolTable;

  public: 
  Tokens analyzeString (string);
  void analyzeInputFile(ifstream&);
  vector<tuple<Tokens, string>> getAnalyzedResult();
  vector<string> getSymbolTable();
};





#endif /* lexical_analyzer_hpp */

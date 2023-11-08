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
  vector<string> _symbolTable;
  Tokens analyzeString (string);

public: 
  void analyzeInputFile(ifstream&);
  vector<tuple<Tokens, string>> getAnalyzedResult();
  vector<string> getSymbolTable();
};

#endif /* lexical_analyzer_hpp */

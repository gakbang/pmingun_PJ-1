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

Tokens analyzeString (string);
vector<tuple<Tokens, string>> analyzeInputFile(ifstream&);

#endif /* lexical_analyzer_hpp */

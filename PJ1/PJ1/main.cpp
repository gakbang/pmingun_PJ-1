//
//  main.cpp
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#include <iostream>
#include "token.hpp"
#include "parser.hpp"
#include<vector>
#include <tuple>
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    std::tuple<Tokens, std::string> token;
    std::vector<std::tuple<Tokens, std::string>> tokenList;
    tokenList.push_back(std::make_tuple(IDENT, "1"));
    tokenList.push_back(std::make_tuple(ASSIGNMENT_OP, "2"));
    tokenList.push_back(std::make_tuple(CONST, "3"));
    tokenList.push_back(std::make_tuple(SEMI_COLON, "4"));
    tokenList.push_back(std::make_tuple(IDENT, "4"));
    tokenList.push_back(std::make_tuple(ASSIGNMENT_OP, "2"));
    tokenList.push_back(std::make_tuple(LEFT_PAREN, "4"));
    tokenList.push_back(std::make_tuple(CONST, "3"));
    tokenList.push_back(std::make_tuple(MULT_OP, "3"));
    tokenList.push_back(std::make_tuple(CONST, "3"));
    tokenList.push_back(std::make_tuple(RIGHT_PAREN, "4"));

    Parser parser(tokenList);
    parser.Parse();

    return 0;
}

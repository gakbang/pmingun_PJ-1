//
//  parser.hpp
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#ifndef parser_hpp
#define parser_hpp

#include <stdio.h>
#include<stack>
#include<vector>
#include<tuple>
#include<string>
#include<iostream>

#include "token.hpp"

#endif /* parser_hpp */

class Parser {
private:
	int _index;
	std::stack<Tokens> _parsingStack;
	std::vector<std::tuple<Tokens, std::string>> _tokenList;


	std::string getToken() { return std::get<1>(_tokenList[_index]); } //TOKEN의 string값 가져오기

	void nextToken() { _index++; return; }
	bool isEmpty() { return _index >= _tokenList.size(); }
	bool isToken(Tokens token) { return token == std::get<0>(_tokenList[_index]); }

	void program();
	void statements();
	void statement();

	int expression();
	int term();
	int term_tail();
	int factor();
	int factor_tail();

	int ident();
	int ident_val();
	int add_op();
	int mult_op();
	int const_val();



	void debug() {
		for (auto i : _tokenList) {
			std::cout << std::get<0>(i) << std::get<1>(i) << std::endl;
		}
	}
public:
	Parser(std::vector<std::tuple<Tokens, std::string>> tokenList) :_index(0), _tokenList(tokenList) {
		_parsingStack.push(PROGRAM);
		//debug();
	}
	void Parse();
};

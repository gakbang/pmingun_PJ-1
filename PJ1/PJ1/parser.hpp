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
#include<map>

#include "token.hpp"
#include "lexical_analyzer.hpp"

#endif /* parser_hpp */


template<typename T>
class OptionalData {
public:
	bool isNull;
	bool isUnknown;
	T data;
	OptionalData() :isNull(true), isUnknown(false), data(0) {} // NullValue 
	OptionalData(bool isUnknown) :isNull(false), isUnknown(isUnknown), data(0) {} // Unknown data
	OptionalData(T data) :isNull(false), isUnknown(false), data(data) {} //공 스트링 용도


};


class OptionalInt : OptionalData<int> {
public:
	OptionalInt():OptionalData(){} 
	OptionalInt(bool isUnknown) :OptionalData(isUnknown) {}
	OptionalInt(int data) :OptionalData(data) {}

	OptionalInt& operator=(const OptionalInt& o) {
		isUnknown = o.isUnknown;
		data = o.data;
		return *this;
	}
	OptionalInt& operator=(const int& data) {
		this->data = data;
		return *this;
	}
	
};

class OptionalDouble : OptionalData<double> {
public:
	OptionalDouble() :OptionalData() {}
	OptionalDouble(bool isUnknown) :OptionalData(isUnknown) {}
	OptionalDouble(double data) :OptionalData(data) {}

	OptionalDouble& operator=(const OptionalDouble& o) {
		isUnknown = o.isUnknown;
		data = o.data;
		return *this;
	}
};

class Parser {
private:
	int _index;
	std::stack<Tokens> _parsingStack;
	std::vector<std::tuple<Tokens, std::string>> _tokenList;
	std::map<std::string, OptionalData<int>> _symbolTable;

	std::string getToken() { return std::get<1>(_tokenList[_index]); } //TOKEN의 string값 가져오기
	void nextToken() { _index++; return; }
	bool isEmpty() { return std::get<0>(_tokenList[_index]) == END_OF_FILE; }
	bool isToken(Tokens token) { return token == std::get<0>(_tokenList[_index]); }

	bool isErrorOccurred = false;

	void program();
	void statements();
	void statement();

	OptionalData<int> expression();
	OptionalData<int> term();
	OptionalData<int> term_tail();
	OptionalData<int> factor();
	OptionalData<double> factor_tail();

	std::string ident();
	OptionalData<int> ident_val();
	OptionalData<int> add_op();
	OptionalData<int> mult_op();
	OptionalData<int> const_val();



	void debug() {
		for (auto i : _tokenList) {
			std::cout << std::get<0>(i) << std::get<1>(i) << std::endl;
		}
	}

	void checkError(Tokens token) { // 현재 있는 함수 정보 parameter로 전달 - WARNING 처리할 때 좋을 것 같음

	}



public:
	Parser(std::vector<std::tuple<Tokens, std::string>> tokenList,
		std::vector<std::string> symbolTable) :_index(0), _tokenList(tokenList) {
		_parsingStack.push(PROGRAM);
		for (auto symbol : symbolTable) {
			_symbolTable.insert(pair<std::string, OptionalData<int>>(symbol, OptionalData<int>()));
			
		}
	}
	void Parse();
};
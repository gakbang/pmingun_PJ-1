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

#include "error_warning.hpp"
#include "token.hpp"
#include "lexical_analyzer.hpp"

#endif /* parser_hpp */


template<typename T>
class OptionalData {
protected:
	OptionalData() :isNull(true), isUnknown(false), data(0) {}
	OptionalData(bool isUnknown) :isNull(false), isUnknown(isUnknown), data(0) {}
	OptionalData(T data) :isNull(false), isUnknown(false), data(data) {}
public:
	using data_type = T;
	bool isNull; // 선언 여부 확인하는 변수
	bool isUnknown; // Unknown Data 확인하는 변수
	T data;

	bool isValid() { return !(isNull | isUnknown); }
	
	std::string GetData() {
		if (isNull) return "NullData";
		if (isUnknown) return "Unknown";
		else return to_string(data);
	}
};

template <class T1, class T2>
T1 ConvertType(T2& t) {
	static_assert(std::is_base_of<OptionalData<typename T1::data_type>, T1>::value, "OptionalData");
	static_assert(std::is_base_of<OptionalData<typename T2::data_type>, T2>::value, "OptionalData");
	T1 value = T1();
	value.isNull = t.isNull;
	value.isUnknown = t.isUnknown;
	value.data = t.data;

	return value;
}


class OptionalDouble : public OptionalData<double> {
private:
	OptionalDouble(bool isUnknown) : OptionalData(isUnknown) {}

public:
	static OptionalDouble GetUnknown(){ return OptionalDouble(true); }
	OptionalDouble() :OptionalData() {}
	OptionalDouble(double data) : OptionalData(data) {}


	OptionalDouble& operator=(const OptionalDouble& o) {
		isUnknown = o.isUnknown;
		data = o.data;
		return *this;
	}

	OptionalDouble& operator+ (const OptionalDouble& o) {
		OptionalDouble value = OptionalDouble(0.0);
		value.isUnknown = isUnknown | o.isUnknown;
		value.data = data + o.data;
		return value;
	}

	OptionalDouble& operator* (const OptionalDouble& o) {
		OptionalDouble value = OptionalDouble(0.0);
		value.isUnknown = isUnknown | o.isUnknown;
		value.data = data * o.data;
		return value;
	}
};

class OptionalInt : public OptionalData<int> {
private:
	OptionalInt(bool isUnknown) : OptionalData(isUnknown) {} //OptionalInt(true) - UnknownData

public:
	static OptionalInt GetUnknown() { return OptionalInt(true); }

	OptionalInt() : OptionalData() {} //Null Data
	OptionalInt(int data) : OptionalData(data) {
	}
	

	OptionalInt& operator=(const OptionalInt& o) {
		isUnknown = o.isUnknown;
		data = o.data;
		return *this;
	}

	OptionalInt& operator=(const OptionalDouble& o) {
		isUnknown = o.isUnknown;
		data = o.data;
		return *this;
	}

	OptionalInt& operator=(const int& o) {
		data = o;
		return *this;
	}

	OptionalInt& operator+ (const OptionalInt& o) {
		OptionalInt value = OptionalInt(0);
		value.isUnknown = isUnknown|o.isUnknown;
		value.data = data + o.data;
		return value;
	}

	OptionalInt& operator* (const OptionalInt& o) {
		OptionalInt value = OptionalInt(0);
		value.isUnknown = isUnknown | o.isUnknown;
		value.data = data * o.data;
		return value;
	}
};


class Parser {
private:
	int _index;
	std::stack<Tokens> _parsingStack;
	std::vector<std::tuple<Tokens, std::string>> _tokenList;
	std::map<std::string, OptionalInt> _symbolTable;

	std::string getToken() { return std::get<1>(_tokenList[_index]); } //TOKEN의 string값 가져오기
	void nextToken() { _index++; return; }
    void moveNextAndCheckValid() {nextToken(); if(isToken(UNKNOWN)){logWarning(UNKNOWN_ID); nextToken();} }
	bool isEmpty() { return std::get<0>(_tokenList[_index]) == END_OF_FILE; }
	bool isToken(Tokens token) { 
		return token == std::get<0>(_tokenList[_index]); 
	}

	// Error Handling
	vector<Warnings> warningList = {};
	vector<Errors> errorList = {};

	bool hasError() { return errorList.size() != 0; }

	int idCountPerStatement = 0;
	int constCountPerStatement = 0;
	int opCountPerStatement = 0;


	void program();
	void statements();
	void statement();

	OptionalInt expression();
	OptionalInt term();
	OptionalInt term_tail();
	OptionalInt factor();
	OptionalDouble factor_tail();

	std::string ident();
	OptionalInt ident_val();
	int add_op();
	int mult_op();
	OptionalInt const_val();

	// Log Function
	void printStatementLog() { printCountPerStatement(); printWarningAndErrorList(); resetVariablesForNewStatement(); }
	void printToken();
	void printCountPerStatement();
	void printWarningAndErrorList();

	// Error Handling Function
	void resetVariablesForNewStatement();
	void logError(Errors);
	void logWarning(Warnings);

	//Error Manage Function
	void ManageInvalidInput();
	void ManageInvalidInput(Tokens); //현재 Token을 파라미터로 전달, 다음 토큰에 따라 오류 처리
	void ManageInvalidInput(Errors); //Error / Warning Log를 전달 오류 처리
	void ManageInvalidInput(Warnings); //Error / Warning Log를 전달 오류 처리



	void debug() {
		for (auto i : _tokenList) {
			std::cout << std::get<0>(i) << std::get<1>(i) << std::endl;
		}
	}

	void debug2() { // symbolTable의 변수명, 저장된 데이터값 출력
		for (auto i : _symbolTable) {
			std::cout << i.first << " : " << i.second.GetData() << std::endl;
		}
	}

	void checkError(Tokens token) { // 현재 있는 함수 정보 parameter로 전달 - WARNING 처리할 때 좋을 것 같음

	}



public:
	Parser(std::vector<std::tuple<Tokens, std::string>> tokenList,
		std::vector<std::string> symbolTable) :_index(0), _tokenList(tokenList) {
		_parsingStack.push(PROGRAM);
		for (auto symbol : symbolTable) {
			_symbolTable.insert(pair<std::string, OptionalInt>(symbol, OptionalInt()));
			
		}
	}
	void Parse();
};

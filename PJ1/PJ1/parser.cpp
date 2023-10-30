//
//  parser.cpp
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//
#include<stack>
#include<vector>
#include<tuple>
#include<string>
#include<iostream>
#include<sstream>
#include "parser.hpp"
#include "token.hpp"





void Parser::Parse() { program(); }
void Parser::program() { statements(); return ; }



void Parser::statements() {
	statement();
	while (!isEmpty()&&isToken(SEMI_COLON)){
		std::cout << "SEMI_COLON" << " ";
		nextToken();
		statement();
		
	}
	if (!isEmpty()) {
		//에러 : Token이 남아 있음
	}
	return;
}
void Parser::statement() {
	if (!isEmpty() && isToken(IDENT)) {
		int identVal = ident();
	}
	else {
		//에러 : STATEMENT가 IDENT로 시작하지 않음
	}
	if (!isEmpty() && isToken(ASSIGNMENT_OP)) {
		std::cout << "ASSIGN" << " ";
		nextToken();
	}
	else {
		//에러 : STATEMENT에 ASSIGNMENT_OP없음
	}
	int value = expression();
	// 대입문
	// symbolTablep[identVal] = value;
	return;
}

int Parser::expression() {
	int value1 = term();
	int value2 = term_tail();
	return value1 + value2;
}

int Parser::term() {
	int value1 = factor();
	double value2 = factor_tail();
	return (int)(value1 * value2);
}

int Parser::term_tail() {
	if (!isEmpty() && isToken(ADD_OP)) {
		int opType = add_op();
		int value1 = term();
		double value2 = term_tail();
		int value = (int)(value1 * value2);
		if (opType) { // - 연산인 경우
			value = 0 - value;
		}
		return value;
	}
	else return 0; // 공 스트링 (연산 없음)
}

int Parser::factor() {
	if (!isEmpty() && isToken(IDENT)) {
		
		return ident_val();
	}
	else if (!isEmpty() && isToken(CONST)) {
		return const_val();
	}
	else if (!isEmpty() && isToken(LEFT_PAREN)) {
		std::cout << "LEFT" << " ";
		nextToken();
		int value = expression();
		if (!isEmpty() && isToken(RIGHT_PAREN)) {
			std::cout << "RIGHT" << " ";
			nextToken();
			return value;
		}
		else {
			//오류
		}
	}
	return 0;
}

double Parser::factor_tail() {
	if (!isEmpty() && isToken(MULT_OP)) {
		int opType = mult_op();
		int value1 = factor();
		double value2 = factor_tail();
		double value = value1 * value2;
		if (opType) { // 나누기 연산인 경우
			value = 1.0 / value;
		}
		return value;
	}
	else return 1; // 공 스트링 (연산 없음)
}


int Parser::ident(){ // STATEMENT의 가장 앞에 나오는 identifier
	if (!isEmpty() && isToken(IDENT)) {
		std::string value = getToken();
		std::cout << "IDENT" << " ";
		// symbol table 에서 ident 확인 작업

		nextToken();
	}
	return 0;
	//symbol table index 리턴할 듯 ?
}

int Parser::ident_val() { // ident value 읽어오기
	if (!isEmpty() && isToken(IDENT)) {
		std::cout << "IDENT" << " ";
		std::string id = getToken();

		int index = ident();
		
		int value;
		// value = symbol table[ident]
		return 0; //변수의 value
	}
	
}



int Parser::add_op(){
	if (!isEmpty() && isToken(ADD_OP)) {
		std::cout << "ADD_OP"<<" ";
		int value;
		value = getToken() == "-";
		//  +, - 구분가능한 리턴값
		nextToken();
		return value;
	}
}

int Parser::mult_op() {
	if (!isEmpty() && isToken(MULT_OP)) {
		std::cout << "MULT_OP" << " ";
		int value;
		value = getToken() == "/";
		nextToken();
		return value;
	}
}

int Parser::const_val() {
	if (!isEmpty() && isToken(CONST)) {
		std::stringstream ss(getToken());
		std::cout << "CONST"<<" ";
		int value;
		ss >> value;
		nextToken();
		return value;
	}
}





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
		nextToken();
		statement();
	}
	if (!isEmpty()) {
		//에러 : Token이 남아 있음
	}
	return;
}
void Parser::statement() {
	if (isToken(IDENT)) {
		int identVal = ident();
	}
	else {
		//에러 : STATEMENT가 IDENT로 시작하지 않음
	}
	if (isToken(ASSIGNMENT_OP)) {
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
	int value =	term();
	term_tail();
	return 0;
}

int Parser::term() {
	factor();
	factor_tail();
	return 0;
}

int Parser::term_tail() {
	if (isToken(ADD_OP)) {
		int opType = add_op();
		term();
		term_tail();
	}
	else return 0; // 공 스트링 (연산 없음)
}

int Parser::factor() {
	if (isToken(IDENT)) {
		return ident_val();
	}
	else if (isToken(CONST)) {
		return const_val();
	}
	else if (isToken(LEFT_PAREN)) {
		nextToken();
		int value = expression();
		if (isToken(RIGHT_PAREN)) {
			nextToken();
			return value;
		}
		else {
			//오류
		}
	}
	return 0;
}

int Parser::factor_tail() {
	if (isToken(MULT_OP)) {
		mult_op();
		factor();
		factor_tail();
	}
	else return 1; // 공 스트링 (연산 없음)
}


int Parser::ident(){ // STATEMENT의 가장 앞에 나오는 identifier
	if (isToken(IDENT)) {
		std::string value = getToken();
		// symbol table 에서 ident 확인 작업

		nextToken();
	}
	return 0;
	//symbol table index 리턴할 듯 ?
}

int Parser::ident_val() { // ident value 읽어오기
	if (isToken(IDENT)) {
		std::string id = getToken();

		int index = ident();
		
		int value;
		// value = symbol table[ident]
		return 0; //변수의 value
	}
	
}



int Parser::add_op(){
	if (isToken(ADD_OP)) {
		int value;
		//  +, - 구분가능한 리턴값
		nextToken();
		return value;
	}
}

int Parser::mult_op() {
	if (isToken(MULT_OP)) {
		int value;
		nextToken();
		return value;
	}
}

int Parser::const_val() {
	if (isToken(CONST)) {
		std::stringstream ss(getToken());
		int value;
		ss >> value;
		return value;
	}
}





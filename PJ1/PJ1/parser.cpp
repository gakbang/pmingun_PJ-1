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
		//���� : Token�� ���� ����
	}
	return;
}
void Parser::statement() {
	if (isToken(IDENT)) {
		int identVal = ident();
	}
	else {
		//���� : STATEMENT�� IDENT�� �������� ����
	}
	if (isToken(ASSIGNMENT_OP)) {
		nextToken();
	}
	else {
		//���� : STATEMENT�� ASSIGNMENT_OP����
	}
	int value = expression();
	// ���Թ�
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
	if (isToken(ADD_OP)) {
		int opType = add_op();
		int value1 = term();
		double value2 = term_tail();
		int value = (int)(value1 * value2);
		if (opType) { // - ������ ���
			value = 0 - value;
		}
		return value;
	}
	else return 0; // �� ��Ʈ�� (���� ����)
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
			//����
		}
	}
	return 0;
}

double Parser::factor_tail() {
	if (isToken(MULT_OP)) {
		int opType = mult_op();
		int value1 = factor();
		double value2 = factor_tail();
		double value = value1 * value2;
		if (opType) { // ������ ������ ���
			value = 1.0 / value;
		}
		return value;
	}
	else return 1; // �� ��Ʈ�� (���� ����)
}


int Parser::ident(){ // STATEMENT�� ���� �տ� ������ identifier
	if (isToken(IDENT)) {
		std::string value = getToken();
		// symbol table ���� ident Ȯ�� �۾�

		nextToken();
	}
	return 0;
	//symbol table index ������ �� ?
}

int Parser::ident_val() { // ident value �о����
	if (isToken(IDENT)) {
		std::string id = getToken();

		int index = ident();
		
		int value;
		// value = symbol table[ident]
		return 0; //������ value
	}
	
}



int Parser::add_op(){
	if (isToken(ADD_OP)) {
		int value;
		value = getToken() == "-";
		//  +, - ���а����� ���ϰ�
		nextToken();
		return value;
	}
}

int Parser::mult_op() {
	if (isToken(MULT_OP)) {
		int value;
		value = getToken() == "/";
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





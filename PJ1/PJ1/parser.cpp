#include <stack>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>

#include "lexical_analyzer.hpp"	
#include "parser.hpp"
#include "token.hpp"

void Parser::Parse() { program(); }
void Parser::program() { statements(); return ; }

void Parser::statements() {
	statement();
	while (isToken(SEMI_COLON)){
		//std::cout << "SEMI_COLON" << " ";
		nextToken();
		statement();
		
	}
	if (!isEmpty()) {
		//에러 : Token이 남아 있음
		std::cout << "\nDEBUG : ERROR - TOKEN IS STILL LEFT\n";
		
	}
	return;
}
void Parser::statement() {
	isErrorOccurred = false;
	std::string id;
	if ( isToken(IDENT)) {
		 id = ident();
	}
	else {
		//에러 처리 함수
		//에러 : STATEMENT가 IDENT로 시작하지 않음
		std::cout << "\nDEBUG : ERROR - STATEMENT NOT START WITH IDENT \n";
	}

	if (isToken(COLON)) {
		//std::cout << "ASSIGN" << " ";
		nextToken();
	}
	else {
		//에러 : STATEMENT에 ASSIGNMENT_OP의 :가 없음
		std::cout << "\nDEBUG : ERROR - ASSIGNMENT OP : IS MISSING\n";		
		isErrorOccurred = true;
	}
	
	if (!isEmpty() && isToken(EQUAL)) {
		nextToken();
	}
	else {
		//에러 : STATEMENT에 ASSIGNMENT_OP의 =가 없음
		std::cout << "\nDEBUG : ERROR - ASSIGNMENT OP = IS MISSING\n";		
		isErrorOccurred = true;
	}
	
	OptionalInt value = expression();

	if (isErrorOccurred) {
		//Error 발생한 Statement, Error 출력 후 넘어가기
	}
	
	_symbolTable.find(id)->second = value;
	
	// 대입문
	// symbolTablep[identVal] = value;
	return;
}

OptionalInt Parser::expression() {
	if (!isErrorOccurred) {
		OptionalInt value1 = term();
		OptionalInt value2 = term_tail();
		return value1 + value2;
	}
	return OptionalInt();
}

OptionalInt Parser::term() {
	if (!isErrorOccurred) {
		int value1 = factor();
		double value2 = factor_tail();
		return (int)(value1 * value2);
	}
		return OptionalInt();

}

OptionalInt Parser::term_tail() {
	if (!isErrorOccurred) {
		if (isToken(ADD_OP)) {
			int opType = add_op();
			int value1 = term();
			double value2 = term_tail();
			int value = (int)(value1 * value2);
			if (opType) { // - 연산인 경우
				value = 0 - value;
			}
			return value;
		}
		else{
			return OptionalInt(0); // 공 스트링 (연산 없음)
		}
	}
	return OptionalInt();
}

OptionalInt Parser::factor() {
	if (!isErrorOccurred) {
		if (!isEmpty() && isToken(IDENT)) {
			
			return ident_val();
		}
		else if (!isEmpty() && isToken(CONST)) {
			return const_val();
		}
		else if (!isEmpty() && isToken(LEFT_PAREN)) {
			//std::cout << "LEFT" << " ";
			nextToken();
			OptionalInt value = expression();
			if (!isEmpty() && isToken(RIGHT_PAREN)) {
				std::cout << "RIGHT" << " ";
				nextToken();
				return value;
			}
			else {
				//오류
				std::cout << "\nDEBUG : ERROR - RIGHT PARRENT IS MISSING\n";		
				isErrorOccurred = true;
			}
		}
		else {
		//오류
			std::cout << "\nDEBUG : ERROR - FACTOR \n";		
			isErrorOccurred = true;

		}
	}
	return 0;
}

OptionalDouble Parser::factor_tail() {
	if (!isErrorOccurred) {
		if (isToken(MULT_OP)) {
			int opType = mult_op();
			int value1 = factor();
			OptionalDouble value2 = factor_tail();
			OptionalDouble value = value1 * value2;
			if (opType) { // 나누기 연산인 경우
				value = 1.0 / value;
			}
			return value;
		}
		return OptionalDouble(1.0); // 공 스트링 (연산 없음)
	}
	else {
		return OptionalDouble(false); // UnknownData
	}
}


std::string Parser::ident(){ // STATEMENT의 가장 앞에 나오는 identifier
	if (!isErrorOccurred) {
		if (!isEmpty() && isToken(IDENT)) {
			std::string value = getToken();
			_symbolTable.find(getToken())->second.isNull = false; //Identifier 선언
			_symbolTable.find(getToken())->second.data = 0; //Identifier 선언
			//std::cout << "IDENT" << " ";
			nextToken();
			return value;
		}
	}
	//symbol table index 리턴할 듯 ?
}

OptionalData<int> Parser::ident_val() { // ident value 읽어오기
	OptionalInt value;
	if (!isErrorOccurred) {
		if (!isEmpty() && isToken(IDENT)) {
			std::cout << "IDENT" << " ";
			auto iter = _symbolTable.find(getToken());
			if (iter->second.isNull) {
				// Error : 아직 선언되지 않은 변수 참조

				_symbolTable.find(getToken())->second.isNull = false; //Identifier 선언
				_symbolTable.find(getToken())->second.isUnknown = true; //Identifier 선언
				

			}
			return iter->second.data;
			// value = symbol table[ident]
			//return value; //변수의 value
		}
	}
	return 0;
}

OptionalInt Parser::add_op(){
	OptionalInt value;
	if (!isErrorOccurred) {
		if (!isEmpty() && isToken(ADD_OP)) {
			std::cout << "ADD_OP"<<" ";
			int value = getToken() == "-"; //  +, - 구분가능한 리턴값
			nextToken();
			return value;
		}
	}
	return 0;
}

OptionalInt Parser::mult_op() {
	OptionalInt value;
	if (!isErrorOccurred) {
		if (!isEmpty() && isToken(MULT_OP)) {
			std::cout << "MULT_OP" << " ";
			int value = getToken() == "/";
			nextToken();
			return value;
		}
	}
	return 0;
}

OptionalInt Parser::const_val() {
	OptionalInt value;
	if (!isErrorOccurred) {
		if (isToken(CONST)) {
			std::stringstream ss(getToken());
			std::cout << "CONST"<<" ";
			int data;
			ss >> data;
			value = data;
			nextToken();
			
		}
	}
	return value;
}
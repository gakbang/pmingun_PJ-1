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
void Parser::program() { statements(); cout << "\n\n"; debug2(); return ; }

void Parser::statements() {
	statement();
	while (isToken(SEMI_COLON)){
		std::cout << getToken() << "\n";
		printCountPerStatement();
		idCountPerStatement = 0;
		constCountPerStatement = 0;
		opCountPerStatement = 0;


		nextToken();
		statement();
		
	}
	if (!isEmpty()) {
		//에러 : Token이 남아 있음
		std::cout << "\nDEBUG : ERROR - TOKEN IS STILL LEFT\n";
		
	}
	cout << '\n';
			printCountPerStatement();
		idCountPerStatement = 0;
		constCountPerStatement = 0;
		opCountPerStatement = 0;
	
	return;
}
void Parser::statement() {
	isErrorOccurred = false;
	std::string id;

	if (isToken(IDENT)) {
		 id = ident();
	}
	else {
		//에러 처리 함수
		//에러 : STATEMENT가 IDENT로 시작하지 않음
		std::cout << "\nDEBUG : ERROR - STATEMENT NOT START WITH IDENT \n";
	}

	if (isToken(COLON)) {
		printToken();
		nextToken();
	}
	else {
		//에러 : STATEMENT에 ASSIGNMENT_OP의 :가 없음
		std::cout << "\nDEBUG : ERROR - COLON : IS MISSING\n";		
		isErrorOccurred = true;
	}
	
	if (!isEmpty() && isToken(EQUAL)) {
		printToken();
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
		_symbolTable.find(id)->second = OptionalInt::GetUnknown();
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
		return (value1 + value2);
	}
	return OptionalInt::GetUnknown();
}

OptionalInt Parser::term() {
	if (!isErrorOccurred) {
		OptionalDouble value1 = ConvertType<OptionalDouble,OptionalInt>(factor());
		OptionalDouble value2 = factor_tail();
		return ConvertType<OptionalInt,OptionalDouble>(value1 * value2);
	}
		return OptionalInt::GetUnknown();

}

OptionalInt Parser::term_tail() {
	if (!isErrorOccurred) {
		if (isToken(ADD_OP)) {
			int opType = add_op();
			OptionalInt value1 = term();
			OptionalInt value2 = term_tail();
			OptionalInt value = value1 + value2;
			if (opType) { // - 연산인 경우
				value.data = 0 - value.data;
			}
			return value;
		}
		else{
			return OptionalInt(0); // 공 스트링 (연산 없음)
		}
	}
	return OptionalInt::GetUnknown();
}

OptionalInt Parser::factor() {
	if (!isErrorOccurred) {
		if (isToken(IDENT)) {
			
			return ident_val();
		}
		else if (!isEmpty() && isToken(CONST)) {
			return const_val();
		}
		else if (!isEmpty() && isToken(LEFT_PAREN)) {
			printToken();
			nextToken();
			OptionalInt value = expression();
			if (!isEmpty() && isToken(RIGHT_PAREN)) {
				printToken();
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
			OptionalInt value1 = factor();
			OptionalDouble value2 = factor_tail();
			OptionalDouble value = value1.data * value2.data;
			if (opType) { // 나누기 연산인 경우
				value = 1.0 / value.data;
			}
			return value;
		}
		return OptionalDouble(1.0); // 공 스트링 (연산 없음)
	}
	else {
		return OptionalDouble::GetUnknown();
	}
}


std::string Parser::ident(){ // STATEMENT의 가장 앞에 나오는 identifier
	if (!isErrorOccurred) {
		if (isToken(IDENT)) {
			std::string value = getToken();
			_symbolTable.find(getToken())->second.isNull = false; //Identifier 선언
			//_symbolTable.find(getToken())->second.data = 0; //Identifier 선언
			printToken();
			idCountPerStatement += 1;
			nextToken();
			return value;
		}
	}
	//symbol table index 리턴할 듯 ?
}

OptionalInt Parser::ident_val() { // ident value 읽어오기
	OptionalInt value;
	if (!isErrorOccurred) {
		if (isToken(IDENT)) {
			printToken();
						idCountPerStatement += 1;

			auto iter = _symbolTable.find(getToken());
			if (iter->second.isNull) {
				// Error : 아직 선언되지 않은 변수 참조

				iter->second.isNull = false; //Identifier 선언
				iter->second.isUnknown = true; //Identifier 선언
				
				

			}
			nextToken();
			return iter->second;
			// value = symbol table[ident]
			//return value; //변수의 value
		}
	}
	return OptionalInt::GetUnknown();
}

int Parser::add_op(){
	OptionalInt value;
	if (!isErrorOccurred) {
		if (!isEmpty() && isToken(ADD_OP)) {
			printToken();
			opCountPerStatement += 1;

			int value = getToken() == "-"; //  +, - 구분가능한 리턴값
			nextToken();
			return value;
		}
	}
	return 0;//Null
}

int Parser::mult_op() {
	OptionalInt value;
	if (!isErrorOccurred) {
		if (isToken(MULT_OP)) {
			printToken();
			opCountPerStatement += 1;
			int value = getToken() == "/";
			nextToken();
			return value;
		}
	}
	return 0;//Null
}

OptionalInt Parser::const_val() {
	if (!isErrorOccurred) {
		if (isToken(CONST)) {
			std::stringstream ss(getToken());
			printToken();
			constCountPerStatement += 1;
			int data;
			ss >> data;
			nextToken();
			return OptionalInt(data);
		}
	}
	return OptionalInt::GetUnknown();
}

void Parser::printToken() {
	std::cout << getToken();
}

void Parser::printCountPerStatement() {
	std::cout << "ID: " << idCountPerStatement << "; CONST:" << constCountPerStatement << "; OP: " << opCountPerStatement << "\n\n";
}
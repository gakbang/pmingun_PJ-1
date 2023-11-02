#include <stack>
#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>

#include "error_warning.hpp"
#include "lexical_analyzer.hpp"	
#include "parser.hpp"
#include "token.hpp"


void Parser::Parse() { program(); }

//TOKEN  function 

void Parser::program() { resetVariablesForNewStatement(); statements(); cout << "\n"; debug2(); return; }

void Parser::statements() {

	statement();//     STATEMENT

	while (isToken(SEMI_COLON)) {
		std::cout << getToken() << endl; // SEMICOLON    
		printStatementLog();// ERROR, WARNING, COUNT message     
		nextToken(); // SEMICOLON     
		statement();//     STATEMENT
	}        


	if (!isToken(END_OF_FILE)) {
		//     : Token            
		std::cout << "\nDEBUG : ERROR - TOKEN IS STILL LEFT\n";
	}

	cout << endl;

	printStatementLog();// ERROR, WARNING, COUNT message

	return;
}

void Parser::statement() {

	std::string id;

	if (isToken(IDENT)) {
		id = ident();
	}
	else {
		printToken();
		logError(BEGIN_IDENT_MISSING);
		nextToken();
	}

	if (isToken(COLON)) {
		printToken();
		nextToken();
	}
	else {
		//     : STATEMENT   ASSIGNMENT_OP   :       
		// std::cout << "\nDEBUG : ERROR - COLON : IS MISSING\n";		
		logWarning(COLON_MISSING);
		cout << ":";
	}

	if (isToken(EQUAL)) {
		printToken();
		nextToken();
	}
	else {
		//     : STATEMENT   ASSIGNMENT_OP   =       
		// std::cout << "\nDEBUG : ERROR - ASSIGNMENT OP = IS MISSING\n";		
		logWarning(EQUAL_MISSING);
		cout << "=";
		// nextToken();
	}

	OptionalInt value = expression();  

	if (id.empty()) return; // check for begin identifier error
	if (hasError()) { _symbolTable.find(id)->second = OptionalInt::GetUnknown(); }
	else { _symbolTable.find(id)->second = value; }
	// assignment executed

	return;
}

OptionalInt Parser::expression() {
	OptionalInt value1 = term();
	OptionalInt value2 = term_tail();
	return (value1 + value2);
}

OptionalInt Parser::term() {
	OptionalInt temp = factor();
	cout << "\nBEFORE CONVERT : " << temp.data << endl;
	cout << "BEFORE CONVERT : " << temp.isNull << endl;
	cout << "BEFORE CONVERT : " << temp.isUnknown << endl;

	OptionalDouble value1 = ConvertType<OptionalDouble, OptionalInt>(temp);

	cout << "OUT CONVERT : " << value1.data << endl;
	cout << "OUT CONVERT : " << value1.isNull << endl;
	cout << "OUT CONVERT : " << value1.isUnknown << endl;
	OptionalDouble value2 = factor_tail();
	return ConvertType<OptionalInt, OptionalDouble>(value1 * value2);
}

OptionalInt Parser::term_tail() {
	if (isToken(ADD_OP)) {
		int opType = add_op();
		OptionalInt value1 = term();
		OptionalInt value2 = term_tail();
		OptionalInt value = value1 + value2;
		if (opType) { value.data = 0 - value.data; }
		return value;
	}
	else { return OptionalInt(0); } // return default value

}

OptionalInt Parser::factor() {
	if (isToken(IDENT)) {
		return ident_val();
	}
	else if (isToken(CONST)) {
		OptionalInt v = const_val();
		return v;
	}
	else if (isToken(LEFT_PAREN)) {
		printToken();
		nextToken();

		OptionalInt value = expression();
		if (isToken(RIGHT_PAREN)) {
			printToken();
			nextToken();
			return value;
		}
		//    
		// RIGHT PARENT WARNING
		else {
			logWarning(NON_PAIR_LEFT_PAREN);
			cout << ")";
			return value;
		}
	}
	else {
		//    
		// MULTIPLE OPERATION WARNING
		if (isToken(MULT_OP) || isToken(ADD_OP)) {
			logWarning(INVALID_OP);
			nextToken();
			return factor();
		}
		else {
			logError(UNKNOWN_ERROR);
			nextToken();
			return factor();
		}
	}
}

OptionalDouble Parser::factor_tail() {
	if (isToken(MULT_OP)) {
		int opType = mult_op();
		OptionalInt value1 = factor();
		OptionalDouble value2 = factor_tail();
		OptionalDouble value = value1.data * value2.data;
		if (opType) { //                  
			if (value.isUnknown) {
				// Unknown       
				return OptionalDouble::GetUnknown();
			}
			else if (value.data == 0) {
				// division by 0  - logic error
				// NEED CODE FOR LOGIC ERROR
				return OptionalDouble();
			}
			else {
				value = 1.0 / value.data;
			}
		}
		return value;
	}
	else {
		return OptionalDouble(1.0); // return default value
	}
}


std::string Parser::ident() { // STATEMENT begin identifier - check declarations
	if (!isToken(IDENT)) { throw std::exception(); } //CODE ERROR : >>CHECK CODE<<
	std::string value = getToken();
	_symbolTable.find(getToken())->second.isNull = false; //Identifier     
	//_symbolTable.find(getToken())->second.data = 0; //Identifier     
	printToken();
	idCountPerStatement += 1;
	nextToken();
	return value;
}

OptionalInt Parser::ident_val() { // ident value  о    
	OptionalInt value;
	if (!isToken(IDENT)) { throw std::exception(); } //CODE ERROR : >>CHECK CODE<<
	{
		printToken();
		idCountPerStatement += 1;

		auto iter = _symbolTable.find(getToken());
		if (iter->second.isNull) {
			// Error :                            
			iter->second.isNull = false; //Identifier     
			iter->second.isUnknown = true; //Identifier     

		}
		nextToken();
		return iter->second;
		// value = symbol table[ident]
		//return value; 
	}

	if (hasError()) {

		return OptionalInt::GetUnknown();
	}

}

int Parser::add_op() {
	if (!isToken(ADD_OP)) { throw std::exception(); }//CODE ERROR : >>CHECK CODE<<

	printToken();
	opCountPerStatement += 1;
	int value = getToken() == "-"; // return true only if op is -
	nextToken();
	if (hasError()) {
		return 0;// return null value - no logic for unknown value
	}
	return value;
}

int Parser::mult_op() {
	if (!isToken(MULT_OP)) { throw std::exception(); } //CODE ERROR : >>CHECK CODE<<

	printToken();
	opCountPerStatement += 1;
	int value = (getToken() == "/");
	nextToken();

	if (hasError()) return 0; //Null 

	return value;
}

OptionalInt Parser::const_val() {
	if (!isToken(CONST)) { throw std::exception(); } //CODE ERROR : >>CHECK CODE<<

	int data;
	std::stringstream ss(getToken());
	printToken();
	constCountPerStatement += 1;
	ss >> data;
	cout << "\nDEBUGHERE** " << data << '\n';
	nextToken();

	if (hasError()) { //ERROR -> return Unknown value
		return OptionalInt::GetUnknown();
	}

	return OptionalInt(data);
}

//TOKEN  FUCTION END




//Error Manage Function



























void Parser::printToken() {
	std::cout << getToken();
}

void Parser::printCountPerStatement() {
	std::cout << "ID: " << idCountPerStatement << "; CONST:" << constCountPerStatement << "; OP: " << opCountPerStatement << "\n";
}

void Parser::printWarningAndErrorList() {
	if (warningList.size() == 0 && errorList.size() == 0) {
		std::cout << "(OK)" << std::endl;
	}
	else {
		for (auto it : warningList) {
			cout << "[WARNING] : ";
			switch (it) {
			case INVALID_OP:
				std::cout << "Romove given invalid operation (additional op / invalid position)" << std::endl;
				break;

			case NON_PAIR_LEFT_PAREN:
				std::cout << "() PAIR DOES NOT MATCH" << std::endl;
				break;

			case EQUAL_MISSING:
				std::cout << "Can't Find = in statement" << std::endl;
				break;

			case COLON_MISSING:
				std::cout << "Can't Find : in statement" << std::endl;
				break;
			}
		}

		for (vector<Errors>::iterator it = errorList.begin(); it != errorList.end(); ++it) {
			cout << "<ERROR> : ";
			switch (*it) {
			case UNKNOWN_ERROR:
				std::cout << "UNKNOWN ERROR IS DETECTED." << std::endl;
				break;
			case UNKNOWN_ID:
				std::cout << "INVALID LEXEME IS DETECTED. " << std::endl;
				break;
			case BEGIN_IDENT_MISSING:
				std::cout << "THE STATEMENT DOES NOT BEGIN WITH IDENTIFIER.";
				break;
			}
		}
	}
	cout << '\n';
}


void Parser::resetVariablesForNewStatement() {
	idCountPerStatement = 0;
	constCountPerStatement = 0;
	opCountPerStatement = 0;
	warningList.clear();
	errorList.clear();
}

void Parser::logError(Errors error) {
	errorList.push_back(error);
}

void Parser::logWarning(Warnings warning) {
	warningList.push_back(warning);
}

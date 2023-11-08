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

void Parser::program() { resetVariablesForNewStatement(); statements(); cout << "\n"; SymbolOutput(); return; }

void Parser::statements() { //STATEMENTS
    
    statement();//     STATEMENT
        
    while (isToken(SEMI_COLON)) {
        std::cout << getToken() << endl; // SEMICOLON
        printStatementLog();// ERROR, WARNING, COUNT message
        moveNextAndCheckValid();        // SEMICOLON
        while (isToken(SEMI_COLON)) {

        }
        statement();        //     STATEMENT
    }

    if (!isToken(END_OF_FILE)) { // check token error
        cout << getToken() << endl;
        logError(TOKEN_LEFT);
    }
    
    
    cout << endl;

    printStatementLog();// ERROR, WARNING, COUNT message
    
    return;
}

void Parser::statement() { // STATEMENT
    
    std::string id; //identifier

    if (isToken(END_OF_FILE)) {
        logWarning(EOF_SEMI_COLON); // last statement semicolon check
        return;
    }    
    if (isToken(IDENT)) { id = ident(); }
    else { //warning error check
        if (isToken(COLON) || isToken(EQUAL)) {
            logError(BEGIN_IDENT_MISSING);  
        }
        else {
            printToken();
            logError(BEGIN_IDENT_MISSING);
            moveNextAndCheckValid();
        }
        
    }
    if (isToken(COLON)) {
        printToken();
        moveNextAndCheckValid();
    }
    else {
        // : STATEMENT   ASSIGNMENT_OP   :
        logWarning(COLON_MISSING);
        cout << ":";
    }
    
    if (isToken(EQUAL)) {
        printToken();
        moveNextAndCheckValid();
    }
    else {
        // : STATEMENT   ASSIGNMENT_OP   =
        logWarning(EQUAL_MISSING);
        cout << "=";
    }
    
    OptionalInt value = expression();

    while (!isToken(END_OF_FILE) && !isToken(SEMI_COLON)) { // check for wrong structure
        expression();
        logError(WRONG_STATEMENT);
        value = OptionalInt::GetUnknown();
    }

    while (parenCountPerStatement > 0) { // parenthesis pair matching
        logWarning(NON_PAIR_LEFT_PAREN);
        cout << ")";
        parenCountPerStatement--;
    }

    if (id.empty()) return; // check for begin identifier error
    _symbolTable.find(id)->second.isNull = false; // identifier declaration
    if (hasError()) { _symbolTable.find(id)->second = OptionalInt::GetUnknown(); }
    else { _symbolTable.find(id)->second = value; }
    // assignment executed

    return;
}

OptionalInt Parser::expression() { //EXPRESSION
    
    OptionalInt value1 = term();
    OptionalInt value2 = term_tail();
    return (value1 + value2);
}

OptionalInt Parser::term() { // TERM
    OptionalInt factorResult = factor();
    OptionalDouble value1 = ConvertType<OptionalDouble, OptionalInt>(factorResult); //casting value
    OptionalDouble value2 = factor_tail();
    OptionalDouble multResult = value1 * value2; 
    return ConvertType<OptionalInt, OptionalDouble>(multResult); //casting value
}

OptionalInt Parser::term_tail() {
    if (isToken(ADD_OP)) {
        int opType = add_op();
        OptionalInt value1 = term();
        OptionalInt value2 = term_tail();
        OptionalInt value = value1 + value2;
        if (opType) { value.data = 0 - value.data; } // return negative is operation is minus
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
        parenCountPerStatement++;
        printToken();
        moveNextAndCheckValid();
        OptionalInt value = expression();
        if (isToken(RIGHT_PAREN)) { // paren count matching
            if (parenCountPerStatement > 0) {
                parenCountPerStatement--;
                printToken();
            }
            else { // no left parethesis
                logError(PAREN_PAIR_MISSING);
                printToken();
            }
            moveNextAndCheckValid();
        }
        return value;
    }
    else {
        //check error and warnings
        if (isToken(SEMI_COLON)||isToken(END_OF_FILE)) { //statement end without argument
            logError(ARGUMENT_MISSING);
            return OptionalInt::GetUnknown();
        }
        else if (isToken(RIGHT_PAREN)) { 
            logError(ARGUMENT_MISSING);//operator needs argument
            if (parenCountPerStatement > 0) parenCountPerStatement--;// check for parenthesis
            else logError(PAREN_PAIR_MISSING); // check for parenthesis
            printToken();
            moveNextAndCheckValid();
            return OptionalInt::GetUnknown();
        }
        if (isToken(MULT_OP) || isToken(ADD_OP)) { // operator repetition
            logWarning(INVALID_OP);
            moveNextAndCheckValid();
            return factor();
        }
        else {
            logError(UNKNOWN_ERROR); //unknown
            moveNextAndCheckValid();
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
        if (opType) { // if operation is division
            if (value.isUnknown) { // cannot assure data
                return OptionalDouble::GetUnknown();
            }
            else if (value.data == 0) { // zero divisor error
                logError(ZERO_DIVISER);
                return OptionalDouble::GetUnknown();
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
    printToken();
    idCountPerStatement += 1;
    moveNextAndCheckValid();
    return value;
}

OptionalInt Parser::ident_val() { // ident value  о
    OptionalInt value;
    if (!isToken(IDENT)) { throw std::exception(); } //CODE ERROR : >>CHECK CODE<<
    printToken();
    idCountPerStatement += 1;
    auto iter = _symbolTable.find(getToken());
    if (iter->second.isNull) { //declaration check
        logError(NOT_DECLARED, getToken());
        iter->second.isNull = false; //Identifier
        iter->second.isUnknown = true; //Identifier
        moveNextAndCheckValid();
        return OptionalInt::GetUnknown();

    }
    moveNextAndCheckValid();
    return iter->second;
    // value = symbol table[ident]
    // return value;
    
    if (hasError()) { // error alreay happen - cannot assure data
        return OptionalInt::GetUnknown();
    }
    
}

int Parser::add_op() {
    if (!isToken(ADD_OP)) { throw std::exception(); }//CODE ERROR : >>CHECK CODE<<
    printToken();
    opCountPerStatement += 1;
    int value = getToken() == "-"; // return true only if op is -
    moveNextAndCheckValid();
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
    moveNextAndCheckValid();
    if (hasError()) return 0;// return null value - no logic for unknown value
    return value;
}

OptionalInt Parser::const_val() {
    if (!isToken(CONST)) { throw std::exception(); } //CODE ERROR : >>CHECK CODE<<
    int data;
    std::stringstream ss(getToken());
    printToken();
    constCountPerStatement += 1;
    ss >> data;
    moveNextAndCheckValid();
    if (hasError()) { //ERROR -> return Unknown value
        return OptionalInt::GetUnknown();
    }
    return OptionalInt(data);
}

//TOKEN  FUCTION END




//Error Manage Function

void Parser::printToken() { // print current stack value
    std::cout << getToken();
}

void Parser::printCountPerStatement() { // print count
    std::cout << "ID: " << idCountPerStatement << "; CONST:" << constCountPerStatement << "; OP: " << opCountPerStatement << "\n";
}

void Parser::printWarningAndErrorList() { // print Error and Warning
    if (warningList.size() == 0 && errorList.size() == 0) {
        std::cout << "(OK)" << std::endl;
    }
    else {
        for (auto it : warningList) {
            cout << "[WARNING] : ";
            switch (get<0>(it)) {
                case INVALID_OP:
                    std::cout << "Remove given invalid operation (additional op / invalid position)" << std::endl;
                    break;
                    
                case UNKNOWN_ID:
                    std::cout << "Invalid Lexeme is detected" << std::endl;
                    break;
                    
                case NON_PAIR_LEFT_PAREN:
                    std::cout << "() Pair does not matched" << std::endl;
                    break;
                    
                case EQUAL_MISSING:
                    std::cout << "Can't Find = in statement" << std::endl;
                    break;
                    
                case COLON_MISSING:
                    std::cout << "Can't Find : in statement" << std::endl;
                    break;

                case EOF_SEMI_COLON:
                    std::cout << "LAST STATEMENT DO NOT REQUIRE SEMI COLON" << std::endl;
                    break;
            }
        }
        
        for (auto it : errorList) {
            cout << "<ERROR> : ";
            switch (get<0>(it)) {
                
                case UNKNOWN_ERROR:
                    std::cout << "UNKNOWN ERROR IS DETECTED" << std::endl;
                    break;
                case BEGIN_IDENT_MISSING:
                    std::cout << "THE STATEMENT DOES NOT BEGIN WITH IDENTIFIER" << std::endl;
                    break;
                case WRONG_STATEMENT:
                    std::cout << "STATEMENT HAS WRONG STRUCTURE" << std::endl;
                    break;
                case NOT_DECLARED:
                    std::cout << "THE INDENTIFIER " << get<1>(it)<< " IS NOT DECLARED." << std::endl;
                    break;
                case TOKEN_LEFT:
                    std::cout << "TOKEN IS STILL LEFT IN STREAM" << std::endl;
                    break;
                case ZERO_DIVISER:
                    std::cout << "VALUE CAN'T BE DIVIDED BY ZERO" << std::endl;
                    break;
                case PAREN_PAIR_MISSING:
                    std::cout << "RIGHT PARENT DOES NOT HAVE PAIR." << std::endl;
                    break;
                case ARGUMENT_MISSING:
                    std::cout << "ARGUMENT IS MISSING IN OPRATION." << std::endl;
                    break;
            }
        }
    }
    cout << '\n';
}


void Parser::resetVariablesForNewStatement() { // initialize count per statement
    idCountPerStatement = 0;
    constCountPerStatement = 0;
    opCountPerStatement = 0;
    parenCountPerStatement = 0;
    warningList.clear();
    errorList.clear();
}

// log Error and Warning

void Parser::logError(Errors error) {
    errorList.push_back(std::make_tuple(error, std::string()));
}

void Parser::logError(Errors error, string message) {
    errorList.push_back(std::make_tuple(error, message));
}

void Parser::logWarning(Warnings warning) {
    warningList.push_back(make_tuple(warning,std::string()));
}

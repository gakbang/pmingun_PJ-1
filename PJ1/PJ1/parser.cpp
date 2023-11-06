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
        moveNextAndCheckValid();        // SEMICOLON
        
        statement();        //     STATEMENT
        
    }

    if (!isToken(END_OF_FILE)) {
        cout << getToken() << endl;
        logError(TOKEN_LEFT);
       
    }
    
    
    cout << endl;
    printStatementLog();// ERROR, WARNING, COUNT message
    
    return;
}

void Parser::statement() {
    
    std::string id;

    if (isToken(END_OF_FILE)) {
        logWarning(EOF_SEMI_COLON);
        return;
    }
    
    if (isToken(IDENT)) { id = ident(); }
    else {
        
        
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
        // moveNextAndCheckValid();
    }
    
    OptionalInt value = expression();
    

    while (!isToken(END_OF_FILE) && !isToken(SEMI_COLON)) {

        expression();
        value = OptionalInt::GetUnknown();
    }

    while (parenCountPerStatement > 0) {
        logWarning(NON_PAIR_LEFT_PAREN);
        cout << ")";
        parenCountPerStatement--;
    }

    if (id.empty()) return; // check for begin identifier error
    _symbolTable.find(id)->second.isNull = false;
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
    OptionalInt factorResult = factor();
    OptionalDouble value1 = ConvertType<OptionalDouble, OptionalInt>(factorResult);
    OptionalDouble value2 = factor_tail();
    OptionalDouble multResult = value1 * value2;
    return ConvertType<OptionalInt, OptionalDouble>(multResult);
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
        parenCountPerStatement++;
        printToken();
        moveNextAndCheckValid();
        
        OptionalInt value = expression();


        if (isToken(RIGHT_PAREN)) {

            if (parenCountPerStatement > 0) {
                parenCountPerStatement--;
                printToken();
            }
            else {
                logError(PAREN_PAIR_MISSING);
                printToken();
            }
            moveNextAndCheckValid();
        }

        return value;
        //
        // RIGHT PARENT WARNING
        
    }
    else {
        if (isToken(SEMI_COLON)) {
            logError(ARGUMENT_MISSING);
            return OptionalInt::GetUnknown();
        }
        else if (isToken(RIGHT_PAREN)) {
            logError(ARGUMENT_MISSING);
            if (parenCountPerStatement > 0) parenCountPerStatement--;
            else logError(PAREN_PAIR_MISSING);
            printToken();
            moveNextAndCheckValid();
            return OptionalInt::GetUnknown();
        }
        if (isToken(MULT_OP) || isToken(ADD_OP)) {
            logWarning(INVALID_OP);
            moveNextAndCheckValid();
            return factor();
        }
        else {
            logError(UNKNOWN_ERROR);
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
        if (opType) { //
            if (value.isUnknown) {
                // Unknown
                return OptionalDouble::GetUnknown();
            }
            else if (value.data == 0) {
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
    //_symbolTable.find(getToken())->second.isNull = false; //Identifier
    //_symbolTable.find(getToken())->second.data = 0; //Identifier
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
    
    if (iter->second.isNull) {
        logError(NOT_DECLARED, getToken());
        iter->second.isNull = false; //Identifier
        iter->second.isUnknown = true; //Identifier
        moveNextAndCheckValid();
        return OptionalInt::GetUnknown();

    }
    moveNextAndCheckValid();
    return iter->second;
    // value = symbol table[ident]
    //return value;
    
    if (hasError()) {
        
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
    moveNextAndCheckValid();
    
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
            switch (get<0>(it)) {
                case INVALID_OP:
                    std::cout << "Remove given invalid operation (additional op / invalid position)" << std::endl;
                    break;
                    
                case UNKNOWN_ID:
                    std::cout << "Invalid Lexem is detected" << std::endl;
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
                    std::cout << "LAST STATEMENT DO NOT REQUIRE EOF" << std::endl;
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


void Parser::resetVariablesForNewStatement() {
    idCountPerStatement = 0;
    constCountPerStatement = 0;
    opCountPerStatement = 0;
    parenCountPerStatement = 0;
    warningList.clear();
    errorList.clear();
}

void Parser::logError(Errors error) { 
    errorList.push_back(std::make_tuple(error, std::string()));
}

void Parser::logError(Errors error, string message) {
    errorList.push_back(std::make_tuple(error, message));
}

void Parser::logWarning(Warnings warning) {
    warningList.push_back(make_tuple(warning,std::string()));
}

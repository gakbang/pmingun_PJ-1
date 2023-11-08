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

// **** TOKEN FUNCTION ****
// Program
void Parser::program() {
    resetVariablesForNewStatement(); 
    statements();
    cout << "\n";
    SymbolOutput();
    return;
}

// STATEMENTS
void Parser::statements() {
    
    // call STATEMENT
    statement();
        
    while (isToken(SEMI_COLON)) {
        // SEMICOLON에 대한 처리
        std::cout << getToken() << endl;
        
        // SEMICOLON
        moveNextAndCheckValid();
        
        while (isToken(SEMI_COLON)) {
            logWarning(SEMI_COLON_REPITITION);
            moveNextAndCheckValid();
        }
        
        // ERROR, WARNING, COUNT message
        printStatementLog();
        // STATEMENT
        statement();
    }

    // 만약 모든 처리가 완료되었음에도 토큰이 남아있다면
    // 에러로 기록
    if (!isToken(END_OF_FILE)) {
        cout << getToken() << endl;
        logError(TOKEN_LEFT);
    }
    
    // 남은 ERROR, WARNING, COUNT 메시지 출력하며 statement 파싱 종료
    cout << endl;
    printStatementLog();
    
    return;
}

// STATEMENT
void Parser::statement() {
    
    //identifier
    std::string id;

    // 만약 입력된 토큰이 모두 소진되었는데 세미콜론이 있는 경우에는
    // EOF 위치에 세미콜론이 있음을 경고로 출력
    if (isToken(END_OF_FILE)) {
        logWarning(EOF_SEMI_COLON);
        return;
    }    
    
    // IDENT 처리
    if (isToken(IDENT)) {
        id = ident();
    }
    // IDENT 처리를 못한 경우 오류, 경고 처리
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
    
    // COLON 처리
    if (isToken(COLON)) {
        printToken();
        moveNextAndCheckValid();
    }
    // COLON 처리를 못한 경우 오류, 경고 처리
    else {
        // COLON이 없는 경우, 콜론을 삽입하며 경고 추가 후 진행
        logWarning(COLON_MISSING);
        cout << ":";
    }
    
    // EQUAL 처리
    if (isToken(EQUAL)) {
        printToken();
        moveNextAndCheckValid();
    }
    // EQUAL 처리를 못한 경우 오류, 경고 처리
    else {
        // EQUAL이 없는 경우, 콜론을 삽입하며 경고 추가 후 진행
        logWarning(EQUAL_MISSING);
        cout << "=";
    }
    
    // EXPRESSION 처리
    OptionalInt value = expression();

    // EXPRESSION 내 잘못된 구조에 대한 처리
    while (!isToken(END_OF_FILE) && !isToken(SEMI_COLON)) {
        expression();
        logError(WRONG_STATEMENT);
        value = OptionalInt::GetUnknown();
    }

    // EXPRESSION 내 쌍이 맞지 않는 좌우 괄호에 대한 처리
    while (parenCountPerStatement > 0) {
        logWarning(NON_PAIR_LEFT_PAREN);
        cout << ")";
        parenCountPerStatement--;
    }

    // IDENT로 시작하지 않는 경우에 대한 처리
    if (id.empty()) {
        return;
    }
    
    // IDENT가 유효한 값임을 처리
    _symbolTable.find(id)->second.isNull = false;
    
    // 문제가 없는 경우, IDENT에 연산된 값 대입을 수정
    if (hasError()) {
        _symbolTable.find(id)->second = OptionalInt::GetUnknown();
    }
    else {
        _symbolTable.find(id)->second = value;
    }

    return;
}

// EXPRESSION
OptionalInt Parser::expression() {
    OptionalInt value1 = term();
    OptionalInt value2 = term_tail();
    return (value1 + value2);
}

// TERM
OptionalInt Parser::term() {
    OptionalInt factorResult = factor();
    // casting value
    OptionalDouble value1 = ConvertType<OptionalDouble, OptionalInt>(factorResult);
    OptionalDouble value2 = factor_tail();
    OptionalDouble multResult = value1 * value2; 
    // casting value
    return ConvertType<OptionalInt, OptionalDouble>(multResult);
}

OptionalInt Parser::term_tail() {
    if (isToken(ADD_OP)) {
        int opType = add_op();
        OptionalInt value1 = term();
        OptionalInt value2 = term_tail();
        OptionalInt value = value1 + value2;
        // return negative is operation is minus
        if (opType) {
            value.data = 0 - value.data;
        }
        return value;
    }
    // return default value
    else {
        return OptionalInt(0);
    }
}

OptionalInt Parser::factor() {
    if (isToken(IDENT)) {
        return ident_val();
    }
    else if (isToken(CONST)) {
        return const_val();
    }
    else if (isToken(LEFT_PAREN)) {
        parenCountPerStatement++;
        printToken();
        moveNextAndCheckValid();
        OptionalInt value = expression();
        
        // paren count matching
        if (isToken(RIGHT_PAREN)) {
            if (parenCountPerStatement > 0) {
                parenCountPerStatement--;
                printToken();
            }
            // no left parethesis
            else {
                logError(PAREN_PAIR_MISSING);
                printToken();
            }
            moveNextAndCheckValid();
        }
        return value;
    }
    else {
        // check error and warnings
        // statement end without argument
        if (isToken(SEMI_COLON)||isToken(END_OF_FILE)) {
            logError(ARGUMENT_MISSING);
            return OptionalInt::GetUnknown();
        }
        else if (isToken(RIGHT_PAREN)) {
            //operator needs argument
            logError(ARGUMENT_MISSING);
            
            // check for parenthesis
            if (parenCountPerStatement > 0)
                parenCountPerStatement--;
            // check for parenthesis
            else
                logError(PAREN_PAIR_MISSING);
            
            printToken();
            moveNextAndCheckValid();
            return OptionalInt::GetUnknown();
        }
        
        // operator repetition
        if (isToken(MULT_OP) || isToken(ADD_OP)) {
            logWarning(INVALID_OP);
            moveNextAndCheckValid();
            return factor();
        }
        // unknown
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
        
        // if operation is division
        if (opType) {
            // cannot assure data
            if (value.isUnknown) {
                return OptionalDouble::GetUnknown();
            }
            // zero divisor error
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
    
    // return default value
    else {
        return OptionalDouble(1.0);
    }
}

// STATEMENT begin identifier - check declarations
std::string Parser::ident() {
    // CODE ERROR : >>CHECK CODE<<
    if (!isToken(IDENT)) { throw std::exception(); }
    
    std::string value = getToken();
    printToken();
    idCountPerStatement += 1;
    moveNextAndCheckValid();
    return value;
}

// ident value
OptionalInt Parser::ident_val() {
    OptionalInt value;
    
    // CODE ERROR : >>CHECK CODE<<
    if (!isToken(IDENT)) { throw std::exception(); }
    printToken();
    idCountPerStatement += 1;
    auto iter = _symbolTable.find(getToken());
    
    // declaration check
    if (iter->second.isNull) {
        logError(NOT_DECLARED, getToken());
        
        // Identifier
        iter->second.isNull = false;
        iter->second.isUnknown = true;
        
        moveNextAndCheckValid();
        return OptionalInt::GetUnknown();
    }
    moveNextAndCheckValid();
    return iter->second;
}

int Parser::add_op() {
    // CODE ERROR : >>CHECK CODE<<
    if (!isToken(ADD_OP)) { throw std::exception(); }
    printToken();
    opCountPerStatement += 1;
    
    // return true only if op is -
    int value = getToken() == "-";
    moveNextAndCheckValid();
    
    // return null value - no logic for unknown value
    if (hasError()) {
        return 0;
    }
    return value;
}

int Parser::mult_op() {
    // CODE ERROR : >>CHECK CODE<<
    if (!isToken(MULT_OP)) { throw std::exception(); }
    printToken();
    opCountPerStatement += 1;
    int value = (getToken() == "/");
    moveNextAndCheckValid();
    
    // return null value - no logic for unknown value
    if (hasError()) return 0;
    return value;
}

OptionalInt Parser::const_val() {
    //CODE ERROR : >>CHECK CODE<<
    if (!isToken(CONST)) { throw std::exception(); }
    
    int data;
    std::stringstream ss(getToken());
    printToken();
    constCountPerStatement += 1;
    ss >> data;
    moveNextAndCheckValid();
    
    //ERROR -> return Unknown value
    if (hasError()) {
        return OptionalInt::GetUnknown();
    }
    return OptionalInt(data);
}

// **** TOKEN FUNCTION END ****




// Error Manage Function

// print current stack value
void Parser::printToken() {
    std::cout << getToken();
}

// print count
void Parser::printCountPerStatement() {
    std::cout << "ID: " << idCountPerStatement << "; CONST:" << constCountPerStatement << "; OP: " << opCountPerStatement << "\n";
}

// print Error and Warning
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

                case SEMI_COLON_REPITITION:
                    std::cout << "Semicolon is repeated." << std::endl;
                    break;

                case EOF_SEMI_COLON:
                    std::cout << "Last statement do not require semi colon" << std::endl;
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

// initialize count per statement
void Parser::resetVariablesForNewStatement() {
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

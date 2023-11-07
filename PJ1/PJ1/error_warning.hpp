#pragma once

#ifndef error_warning_h
#define error_warning_h

typedef enum {
    UNKNOWN_ERROR, // undefinable error
    BEGIN_IDENT_MISSING, // statement does not start with identifier
    WRONG_STATEMENT, // statement structure error
    NOT_DECLARED,    // call undeclared identifier
    TOKEN_LEFT,    // Token is still left in stack
    ZERO_DIVISER, // divide by 0
    PAREN_PAIR_MISSING, // right parenthesis does not have pair
    ARGUMENT_MISSING // operator does not have argument
} Errors;

typedef enum {
    INVALID_OP, // operation repitition
    UNKNOWN_ID, // Unknown token input
    NON_PAIR_LEFT_PAREN, //left parenthesis does not have pair
    EQUAL_MISSING, // = is missing
    COLON_MISSING, // : is missing
    EOF_SEMI_COLON // last statement has semicolon
} Warnings;

#endif /* error_warning_h */

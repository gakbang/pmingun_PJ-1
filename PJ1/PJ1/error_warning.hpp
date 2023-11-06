#pragma once

#ifndef error_warning_h
#define error_warning_h

typedef enum {
    UNKNOWN_ERROR,
    BEGIN_IDENT_MISSING,
    WRONG_STATEMENT,
    TOKEN_LEFT,   
    ZERO_DIVISER,
    PAREN_PAIR_MISSING,
    ARGUMENT_MISSING
} Errors;

typedef enum {
    INVALID_OP,
    UNKNOWN_ID,
    NON_PAIR_LEFT_PAREN,
    EQUAL_MISSING,
    COLON_MISSING,
    EOF_SEMI_COLON
} Warnings;

#endif /* error_warning_h */

#pragma once

#ifndef error_warning_h
#define error_warning_h

typedef enum {
    UNKNOWN_ERROR,
    BEGIN_IDENT_MISSING,
    TOKEN_LEFT,   
    ZERO_DIVISER,
    PAREN_PAIR_MISSING
} Errors;

typedef enum {
    INVALID_OP,
    UNKNOWN_ID,
    NON_PAIR_LEFT_PAREN,
    EQUAL_MISSING,
    COLON_MISSING,
} Warnings;

#endif /* error_warning_h */

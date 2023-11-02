#pragma once

#ifndef error_warning_h
#define error_warning_h

typedef enum {
    UNKNOWN_ERROR,
    UNKNOWN_ID,
    BEGIN_IDENT_MISSING,
    TOKEN_LEFT,    
} Errors;

typedef enum {
    INVALID_OP,
    NON_PAIR_LEFT_PAREN,
    EQUAL_MISSING,
    COLON_MISSING,
} Warnings;

#endif /* error_warning_h */
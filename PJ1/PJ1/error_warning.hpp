#pragma once

#ifndef error_warning_h
#define error_warning_h

typedef enum {
    UNKNOWN_ID,
} Errors;

typedef enum {
    MULTIPLE_OP,
    NON_PAIR_LEFT_PAREN,
} Warnings;

#endif /* error_warning_h */
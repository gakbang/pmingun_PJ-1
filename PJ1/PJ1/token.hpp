//
//  token.h
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#ifndef token_h
#define token_h

typedef enum {
    PROGRAM,
    STATEMENTS,
    STATEMENT,
    SEMI_COLON,
    IDENT,
    EQUAL,
    COLON,
    EXPRESSION,
    TERM,
    TERM_TAIL,
    ADD_OP,
    FACTOR,
    FACTOR_TAIL,
    MULT_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    CONST,
    END_OF_FILE,
    UNKNOWN,
} Tokens;

#endif /* token_h */
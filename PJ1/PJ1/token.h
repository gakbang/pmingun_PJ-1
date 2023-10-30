//
//  token.h
//  PJ1
//
//  Created by Park Sungmin on 10/30/23.
//

#ifndef token_h
#define token_h

typedef enum : NSUInteger {
    PROGRAM,
    STATEMENTS,
    STATEMENT,
    SEMI_COLON,
    IDENT,
    ASSIGNMENT_OP,
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
} Tokens;

#endif /* token_h */

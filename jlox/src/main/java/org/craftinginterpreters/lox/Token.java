package org.craftinginterpreters.lox;

import java.util.Objects;

class Token {
    final TokenType type;
    final int line;
    final Object literal;
    final String lexeme;

    Token(TokenType type, String lexeme, Object literal, int line) {
        this.type = type;
        this.line = line;
        this.literal = literal;
        this.lexeme = lexeme;
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Token token) {
            return Objects.equals(this.lexeme, token.lexeme)
                    && Objects.equals(this.literal, token.literal)
                    && this.type == token.type
                    && this.line == token.line;
        }
        return false;


    }

    public String toString() {
        return String.format("%s %s %s", type, lexeme, literal);
    }
}

package org.craftinginterpreters;

import org.junit.jupiter.api.Assertions;

class ScannerTest {

    @org.junit.jupiter.api.Test
    void scanTokens() {
        var scanner = new Scanner("if (foo == bar) {print \"Foo is bar\"} // I am a greasy comment");
        var tokens = scanner.scanTokens();
        var result = tokens.toArray();
        Token[] expect = {
                new Token(TokenType.IF, "if", null, 1),
                new Token(TokenType.LEFT_PAREN, "(", null, 1),
                new Token(TokenType.IDENTIFIER, "foo", null, 1),
                new Token(TokenType.EQUAL_EQUAL, "==", null, 1),
                new Token(TokenType.IDENTIFIER, "bar", null, 1),
                new Token(TokenType.RIGHT_PAREN, ")", null, 1),
                new Token(TokenType.LEFT_BRACE, "{", null, 1),
                new Token(TokenType.PRINT, "print", null, 1),
                new Token(TokenType.STRING, "\"Foo is bar\"", "Foo is bar", 1),
                new Token(TokenType.RIGHT_BRACE, "}", null, 1),
                new Token(TokenType.EOF, "", null, 1),
        };

        Assertions.assertArrayEquals(expect, result);
    }
    @org.junit.jupiter.api.Test
    void scanTokensWithBlockComments() {
        var scanner = new Scanner("if (foo == bar) /* Hi I'm /* a \n\nblock */ comment */ {print \"Foo is bar\"} /* I am a greasy comment*/");
        var tokens = scanner.scanTokens();
        var result = tokens.toArray();
        Token[] expect = {
                new Token(TokenType.IF, "if", null, 1),
                new Token(TokenType.LEFT_PAREN, "(", null, 1),
                new Token(TokenType.IDENTIFIER, "foo", null, 1),
                new Token(TokenType.EQUAL_EQUAL, "==", null, 1),
                new Token(TokenType.IDENTIFIER, "bar", null, 1),
                new Token(TokenType.RIGHT_PAREN, ")", null, 1),
                new Token(TokenType.LEFT_BRACE, "{", null, 1),
                new Token(TokenType.PRINT, "print", null, 1),
                new Token(TokenType.STRING, "\"Foo is bar\"", "Foo is bar", 1),
                new Token(TokenType.RIGHT_BRACE, "}", null, 1),
                new Token(TokenType.EOF, "", null, 1),
        };

        Assertions.assertArrayEquals(expect, result);
    }
}
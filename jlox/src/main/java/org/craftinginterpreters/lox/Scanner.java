package org.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.craftinginterpreters.lox.TokenType.*;

class Scanner {

	private static final Map<String, TokenType> keywords;

	static {
		keywords = new HashMap<>();
		// I don't want to figure out how to turn off formatting here lol.
		// @formatter:off
		// nvm, i figured it out, this little guy I made got smooshed with it on ):
		//                     
		//                    ^^-^^^-^ 
		//                   ////\\\\\\   < Hi i'm a little guy with a tie!
		keywords.put("and", /* o   o */ AND);
		keywords.put("class", /*   */ CLASS);
		keywords.put("else", /*     */ ELSE);
		keywords.put("false", /* \/*/ FALSE);
		keywords.put("for", /*   /\  */ FOR);
		keywords.put("fun", /*  | |  */ FUN);
		keywords.put("if", /*  \| |/  */ IF);
		keywords.put("nil", /* /\/ \ */ NIL);
		keywords.put("or", /*__\   /__*/ OR);
		keywords.put("print", /*   */ PRINT);
		keywords.put("return", /* */ RETURN);
		keywords.put("super", /*/ \*/ SUPER);
		keywords.put("this", /*|   \*/ THIS);
		keywords.put("true", /*|   |*/ TRUE);
		keywords.put("var", /* |   | */ VAR);
		keywords.put("while", /*\ /*/ WHILE);
		// @formatter:on
	}

	private final Lox lox;
	private final String source;
	private final List<Token> tokens = new ArrayList<Token>();
	private int start = 0;
	private int current = 0;
	private int line = 1;

	Scanner(Lox lox, String source) {
		this.lox = lox;
		this.source = source;
	}

	List<Token> scanTokens() {
		while (!isAtEnd()) {
			start = current;
			scanToken();
		}

		tokens.add(new Token(EOF, "", null, line));
		return tokens;

	}

	private void scanToken() {
		char c = advance();

		switch (c) {
			case '(':
				addToken(LEFT_PAREN);
				break;
			case ')':
				addToken(RIGHT_PAREN);
				break;
			case '{':
				addToken(LEFT_BRACE);
				break;
			case '}':
				addToken(RIGHT_BRACE);
				break;
			case ',':
				addToken(COMMA);
				break;
			case '.':
				addToken(DOT);
				break;
			case '-':
				addToken(MINUS);
				break;
			case '+':
				addToken(PLUS);
				break;
			case ';':
				addToken(SEMICOLON);
				break;
			case '*':
				addToken(STAR);
				break;
			case '!':
				addToken(match('=') ? BANG_EQUAL : BANG);
				break;
			case '=':
				addToken(match('=') ? EQUAL_EQUAL : EQUAL);
				break;
			case '<':
				addToken(match('=') ? LESS_EQUAL : LESS);
				break;
			case '>':
				addToken(match('=') ? GREATER_EQUAL : GREATER);
				break;
			case '/':
				if (match('/')) {
					while (peek() != '\n' && !isAtEnd()) {
						advance();
					}
				} else {
					addToken(SLASH);
				}
				break;
			case ' ':
			case '\r':
			case '\t':
				// Ignore whitespace.
				break;
			case '\n':
				line++;
				break;
			case '"':
				string();
				break;
			default:
				if (isDigit(c)) {
					number();
				} else if (isAlpha(c)) {
					identifier();
				} else {
					lox.error(line, "Unexpected character.");
				}
				break;
		}

	}

	private void string() {
		while (peek() != '"' && !isAtEnd()) {
			if (peek() == '\n') {
				line++;
			}
			advance();
		}
		// File ended before end of string.
		if (isAtEnd()) {
			lox.error(line, "Unterminated string.");
			return;
		}
		// String has been closed.
		advance();
		// Trim the surrounding quotes.
		String value = source.substring(start + 1, current - 1);
		addToken(STRING, value);
	}

	private void number() {
		while (isDigit(peek())) {
			advance();
		}
		// Is the number fractional?
		if (peek() == '.' && isDigit(peekNext())) {
			// Consume the '.'.
			advance();
			while (isDigit(peek())) {
				advance();
			}
		}

		addToken(NUMBER, Double.parseDouble(source.substring(start, current)));
	}

	private void identifier() {
		while (isAlphaNumeric(peek())) {
			advance();
		}
		String text = source.substring(start, current);
		TokenType type = keywords.get(text);
		if (type == null) {
			type = IDENTIFIER;
		}

		addToken(type);
	}

	private char peek() {
		if (isAtEnd())
			return '\0';
		return source.charAt(current);
	}

	private char peekNext() {
		if (current + 1 >= source.length()) {
			return '\0';
		}
		return source.charAt(current + 1);
	}

	private boolean match(char expected) {
		if (isAtEnd())
			return false;
		if (source.charAt(current) != expected)
			return false;

		current++;
		return true;

	}

	private void addToken(TokenType type) {
		addToken(type, null);
	}

	private void addToken(TokenType type, Object literal) {
		String text = source.substring(start, current);
		tokens.add(new Token(type, text, literal, line));
	}

	private char advance() {
		return source.charAt(current++);
	}

	private boolean isAlphaNumeric(char c) {
		return isAlpha(c) || isDigit(c);
	}

	private boolean isAlpha(char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}

	private boolean isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	private boolean isAtEnd() {
		return current >= source.length();
	}
}

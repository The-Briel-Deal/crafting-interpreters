package jlox;

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

	public String toString() {
		return String.format("%s %s %s", type, lexeme, literal);
	}
}

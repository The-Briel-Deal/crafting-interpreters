package org.craftinginterpreters.lox;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Lox {
	private final Interpreter interpreter = new Interpreter();
	boolean hadError = false;
	boolean hadRuntimeError = false;

	public static void main(String[] args) throws IOException {
		var lox = new Lox();
		if (args.length > 1) {
			System.out.println("Usage: jlox [script]");
			System.exit(64);
		} else if (args.length == 1) {
			var filepath = args[0];
			lox.runFile(filepath);
		} else {
			lox.runPrompt();

		}
	}

	private void runFile(String path) throws IOException {
		var bytes = Files.readAllBytes(Paths.get(path));
		run(new String(bytes, Charset.defaultCharset()));
		if (hadError)
			System.exit(65);
		if (hadRuntimeError)
			System.exit(70);
	}

	private void runPrompt() throws IOException {
		var input = new InputStreamReader(System.in);
		var reader = new BufferedReader(input);

		for (;;) {
			System.out.print("> ");
			var line = reader.readLine();
			if (line == null)
				break;
			run(line);
		}
	}

	void run(String source) {
		var scanner = new Scanner(this, source);
		var tokens = scanner.scanTokens();

		var parser = new Parser(this, tokens);
		List<Stmt> statements = parser.parse();

		if (hadError)
			return;

		interpreter.interpret(this, statements);
	}

	void error(int line, String message) {
		report(line, message);
	}

	void error(Token token, String message) {
		if (token.type == TokenType.EOF) {
			report(token.line, " at end", message);
		} else {
			report(token.line, "at '" + token.lexeme + "'", message);
		}
	}

	void runtimeError(RuntimeError error) {
		System.err.println(error.getMessage() + "\n[line " + error.token.line + "]");
		hadRuntimeError = true;
	}

	private void report(int line, String message) {
		System.err.printf("[line %d] Error: %s", line, message);
		hadError = true;
	}

	private void report(int line, String at, String message) {
		System.err.printf("[line %d] %s Error: %s", line, at, message);
		hadError = true;
	}
}

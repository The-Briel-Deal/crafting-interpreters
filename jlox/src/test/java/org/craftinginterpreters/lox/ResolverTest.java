
package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;

class ResolverTest {

	class MockInterpreter extends Interpreter {
		public record ResolvedVar(Token name, int depth) {
		}

		List<ResolvedVar> resolvedVars = new ArrayList<>();

		@Override
		void resolve(Expr expr, int depth) {
			assert expr instanceof Expr.Variable;

			resolvedVars.add(new ResolvedVar(((Expr.Variable) expr).name, depth));
			super.resolve(expr, depth);
		}
	}

	List<MockInterpreter.ResolvedVar> resolveVars(String script) {
		var lox = new Lox();

		var scanner = new Scanner(lox, script);
		var tokens = scanner.scanTokens();

		var parser = new Parser(lox, tokens);
		var statements = parser.parse();

		var interpreter = new MockInterpreter();

		var resolver = new Resolver(interpreter, lox);
		resolver.resolve(statements);

		return interpreter.resolvedVars;
	}

	@org.junit.jupiter.api.Test
	void printVar() {
		var script = """
				fun sillydog() {
				  var dog = "cute";
				  {
				    print dog;
				  }
				}

				sillydog();
				""";

		var expect = new ArrayList<>();
		expect
				.add(new MockInterpreter.ResolvedVar(new Token(TokenType.IDENTIFIER, "dog", null, 4), 1));
		var result = resolveVars(script);

		assertEquals(expect, result);
	}
}

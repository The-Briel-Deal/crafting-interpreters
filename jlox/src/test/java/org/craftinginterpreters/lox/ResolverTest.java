
package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;

class ResolverTest {

	class MockInterpreter extends Interpreter {
		public record ResolvedVar(Expr expr, int depth) {
		}

		List<ResolvedVar> resolvedVars = new ArrayList<>();

		@Override
		void resolve(Expr expr, int depth) {
			resolvedVars.add(new ResolvedVar(expr, depth));
			super.resolve(expr, depth);
		}
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

		var lox = new Lox();

		var scanner = new Scanner(lox, script);
		var tokens = scanner.scanTokens();

		var parser = new Parser(lox, tokens);
		var statements = parser.parse();

		var interpreter = new Interpreter();

		var resolver = new Resolver(interpreter, lox);
		resolver.resolve(statements);

	}
}

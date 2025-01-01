
package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

class ResolverTest {

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

		var expect = "cute\n";
		assertEquals(expect, "beans");
	}
}

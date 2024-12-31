package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

class ParserTest {
	@org.junit.jupiter.api.Test
	void simplePrintStmt() {
		var script = "print \"Im a dog\";";

		var lox = new Lox();

		var scanner = new Scanner(lox, script);
		var tokens = scanner.scanTokens();

		var parser = new Parser(lox, tokens);
		var statements = parser.parse();

		var astPrinter = new AstPrinter();
		var result = astPrinter.print(statements.toArray(new Stmt[0]));

		var expect = """
				(
				  print Im a dog
				)
				""";
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void declareVar() {
		var script = """
				var dog = "Im a dog";
				print dog;
				""";

		var lox = new Lox();

		var scanner = new Scanner(lox, script);
		var tokens = scanner.scanTokens();

		var parser = new Parser(lox, tokens);
		var statements = parser.parse();

		var astPrinter = new AstPrinter();
		var result = astPrinter.print(statements.toArray(new Stmt[0]));

		var expect = """
				(
				  var dog Im a dog
				)
				(
				  print Var 'dog'
				)
				""";
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void withScope() {
		var script = """
				var a = 3;
				var b = 5;
				{
				  var b = 10;
				  print a + b * a;
				  {
				    var a = 30;
				    print a + b * a;
				  }
				}
				print a + b * a;
				""";

		var lox = new Lox();

		var scanner = new Scanner(lox, script);
		var tokens = scanner.scanTokens();

		var parser = new Parser(lox, tokens);
		var statements = parser.parse();

		var astPrinter = new AstPrinter();
		var result = astPrinter.print(statements.toArray(new Stmt[0]));

		var expect = """
				(
				  var a 3.0
				)
				(
				  var b 5.0
				)
				(
				  blockStmt (
				    list (
				      var b 10.0
				    ) (
				      print (
				        + Var 'a' (
				          * Var 'b' Var 'a'
				        )
				      )
				    ) (
				      blockStmt (
				        list (
				          var a 30.0
				        ) (
				          print (
				            + Var 'a' (
				              * Var 'b' Var 'a'
				            )
				          )
				        )
				      )
				    )
				  )
				)
				(
				  print (
				    + Var 'a' (
				      * Var 'b' Var 'a'
				    )
				  )
				)
				""";
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void withGetter() {
		var script = """
				class Circle {
				  init(radius) {
				    this.radius = radius;
				  }

				  area {
				    return 3.141592653 * this.radius * this.radius;
				  }
				}

				var circle = Circle(4);
				print circle.area;
				""";

		var lox = new Lox();

		var scanner = new Scanner(lox, script);
		var tokens = scanner.scanTokens();

		var parser = new Parser(lox, tokens);
		var statements = parser.parse();

		var astPrinter = new AstPrinter();
		var result = astPrinter.print(statements.toArray(new Stmt[0]));

		var expect = """
				(
				  class Circle (
				    list (
				      function init (
				        list
				      ) (
				        list (
				          exprStmt (
				            setExpr radius this Var 'radius'
				          )
				        )
				      )
				    )
				  ) (
				    list (
				      getter area (
				        list (
				          return (
				            * (
				              * 3.141592653 (
				                getExpr radius this
				              )
				            ) (
				              getExpr radius this
				            )
				          )
				        )
				      )
				    )
				  )
				)
				(
				  var circle (
				    Circle 4.0
				  )
				)
				(
				  print (
				    getExpr area Var 'circle'
				  )
				)
				""";
		assertEquals(expect, result);
	}
}

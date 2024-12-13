package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import org.junit.jupiter.api.BeforeEach;

class LoxTest {

	@BeforeEach
	public void setUp() {
		// Lox.resetState();
	}

	@org.junit.jupiter.api.Test
	void print_str_literal() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("print \"Im a dog\";")
				.append("print \"Im a dog\";")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("Im a dog\n")
				.append("Im a dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void declare_var() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var dog = \"Im a dog\";")
				.append("print dog;")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("Im a dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void var_math() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = 3;")
				.append("var b = 5;")
				.append("print a + b * a;")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("18\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void lexical_scope() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = 3;")
				.append("var b = 5;")
				.append("{")
				.append("  var b = 10;")
				.append("  print a + b * a;")
				.append("  {")
				.append("    var a = 30;")
				.append("    print a + b * a;")
				.append("  }")
				.append("}")
				.append("print a + b * a;")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("33\n")
				.append("330\n")
				.append("18\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void str_cat() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = \"I'm a big greasy \";")
				.append("var b = \"dog\";")
				.append("print a + b;")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("I'm a big greasy dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void assignment() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = \"I'm a big greasy \";")
				.append("var b = \"dog\";")
				.append("b = \"silly dog\";")
				.append("print a + b;")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("I'm a big greasy silly dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void fibonacci() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = 0;")
				.append("var temp;")
				.append("")
				.append("for (var b = 1; a < 10000; b = temp + b) {")
				.append("  print a;")
				.append("  temp = a;")
				.append("  a = b;")
				.append("}")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("0\n")
				.append("1\n")
				.append("1\n")
				.append("2\n")
				.append("3\n")
				.append("5\n")
				.append("8\n")
				.append("13\n")
				.append("21\n")
				.append("34\n")
				.append("55\n")
				.append("89\n")
				.append("144\n")
				.append("233\n")
				.append("377\n")
				.append("610\n")
				.append("987\n")
				.append("1597\n")
				.append("2584\n")
				.append("4181\n")
				.append("6765\n")
				.toString();

		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void function() {
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("fun bark() {\n")
				.append("  print \"woof\";\n")
				.append("}\n")
				.append("bark();\n").toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("woof\n")
				.toString();

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void recursiveFunction() {
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("fun count(num, target) {")
				.append(" if (num <= target) {")
				.append(" print(num);")
				.append(" count(num + 1, target);")
				.append(" }")
				.append("}\n\n")
				.append("count(0, 3);")
				.toString();

		Lox.run(script);

		var expect = new StringBuilder()
				.append("0\n")
				.append("1\n")
				.append("2\n")
				.append("3\n")
				.toString();

		var result = output.get();
		System.out.println(result);

		assertEquals(expect, result);
	}

	class CapturedOutput {
		ByteArrayOutputStream output = new ByteArrayOutputStream();

		CapturedOutput() {
			System.setOut(new PrintStream(output));
		}

		public String get() {
			return output.toString();
		}
	}
}

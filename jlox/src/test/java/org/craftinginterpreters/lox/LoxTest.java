package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import org.junit.jupiter.api.BeforeEach;

class LoxTest {

	@BeforeEach
	public void setUp() {
		Lox.resetState();
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

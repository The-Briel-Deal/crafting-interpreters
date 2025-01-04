package org.craftinginterpreters.lox;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

class LoxTest {

	@org.junit.jupiter.api.Test
	void print_str_literal() {
		var lox = new Lox();
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("print \"Im a dog\";")
				.append("print \"Im a dog\";")
				.toString();

		lox.run(script);

		var expect = new StringBuilder()
				.append("Im a dog\n")
				.append("Im a dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void declare_var() {
		var lox = new Lox();
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var dog = \"Im a dog\";")
				.append("print dog;")
				.toString();

		lox.run(script);

		var expect = new StringBuilder()
				.append("Im a dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void var_math() {
		var lox = new Lox();
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = 3;")
				.append("var b = 5;")
				.append("print a + b * a;")
				.toString();

		lox.run(script);

		var expect = new StringBuilder()
				.append("18\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void lexical_scope() {
		var lox = new Lox();
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

		lox.run(script);

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
		var lox = new Lox();
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = \"I'm a big greasy \";")
				.append("var b = \"dog\";")
				.append("print a + b;")
				.toString();

		lox.run(script);

		var expect = new StringBuilder()
				.append("I'm a big greasy dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void assignment() {
		var lox = new Lox();
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("var a = \"I'm a big greasy \";")
				.append("var b = \"dog\";")
				.append("b = \"silly dog\";")
				.append("print a + b;")
				.toString();

		lox.run(script);

		var expect = new StringBuilder()
				.append("I'm a big greasy silly dog\n")
				.toString();
		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void fibonacci() {
		var lox = new Lox();
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

		lox.run(script);

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
		var lox = new Lox();
		var output = new CapturedOutput();

		var script = new StringBuilder()
				.append("fun bark() {\n")
				.append("  print \"woof\";\n")
				.append("}\n")
				.append("bark();\n").toString();

		lox.run(script);

		var expect = new StringBuilder()
				.append("woof\n")
				.toString();

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void recursiveFunction() {
		var lox = new Lox();
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

		lox.run(script);

		var expect = new StringBuilder()
				.append("0\n")
				.append("1\n")
				.append("2\n")
				.append("3\n")
				.toString();

		var result = output.get();
		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void recursiveFibonacci() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("fun fib(n) {\n")
				.append("  if (n <= 1) return n;\n")
				.append("  return fib(n - 2) + fib(n - 1);\n")
				.append("}\n")
				.append("\n")
				.append("for (var i = 0; i < 20; i = i + 1) {\n")
				.append("  print fib(i);\n")
				.append("}\n")
				.toString();

		lox.run(script);

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
				.toString();

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void parseClass() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class DevonshireCream {                             \n")
				.append("  serveOn() {                                       \n")
				.append("    return \"Scones\";                                \n")
				.append("  }                                                 \n")
				.append("}                                                   \n")
				.append("                                                    \n")
				.append("print DevonshireCream; // Prints \"DevonshireCream\". \n")
				.toString();

		lox.run(script);

		var expect = "DevonshireCream\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void printInstance() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class Bagel {}\n")
				.append("var bagel = Bagel();\n")
				.append("print bagel;\n")
				.toString();

		lox.run(script);

		var expect = "Bagel instance\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void setAndGetMember() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class Dog {}\n")
				.append("var dog = Dog();\n")
				.append("dog.color = \"black\";\n")
				.append("print dog.color;\n")
				.toString();

		lox.run(script);

		var expect = "black\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void simpleMethod() {
		// Methods should always use members from the declared variable.
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class Bacon {\n")
				.append("  eat() {\n")
				.append("    print \"Crunch crunch crunch!\";\n")
				.append("  }\n")
				.append("}\n")
				.append("\n")
				.append("Bacon().eat(); // Prints \"Crunch crunch crunch!\".\n")
				.toString();

		lox.run(script);

		var expect = "Crunch crunch crunch!\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void setBindMethod() {
		// Methods should always use members from the declared variable.
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class Person {\n")
				.append("  sayName() {\n")
				.append("    print this.name;\n")
				.append("  }\n")
				.append("}\n")
				.append("\n")
				.append("var jane = Person();\n")
				.append("jane.name = \"Jane\";\n")
				.append("\n")
				.append("var bill = Person();\n")
				.append("bill.name = \"Bill\";\n")
				.append("\n")
				.append("bill.sayName = jane.sayName;\n")
				.append("bill.sayName(); // ?\n")
				.toString();

		lox.run(script);

		var expect = "Jane\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void useThisOutsideMethod() {
		var lox = new Lox();
		var output = new CapturedErrOutput();
		var script = new StringBuilder().append("print this.test;").toString();

		lox.run(script);

		var expect = "[line 1] at 'this' Error: Can't use 'this' outside of a class.\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void useConstructor() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class FoodWithTopping {\n")
				.append("	init(food, topping) {\n")
				.append("	  this.food = food;\n")
				.append("		this.topping = topping;\n")
				.append("	} \n")
				.append("\n")
				.append("	describe() {\n")
				.append("	  print \"This delectable delicacy is \" + this.topping + \" on \" + this.food + \".\";\n")
				.append("	}\n")
				.append("}\n")
				.append("\n")
				.append("var food = FoodWithTopping(\"toast\", \"beans\");\n")
				.append("\n")
				.append("food.describe();\n")
				.toString();

		lox.run(script);

		var expect = "This delectable delicacy is beans on toast.\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void useConstructorWithReturn() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = new StringBuilder()
				.append("class FoodWithTopping {\n")
				.append("	init(food, topping) {\n")
				.append("		this.hasTopping = false;\n")
				.append("	  this.food = food;\n")
				.append("		if (topping == nil) {\n")
				.append("		  return;\n")
				.append("		}\n")
				.append("		this.hasTopping = true;\n")
				.append("		this.topping = topping;\n")
				.append("	} \n")
				.append("\n")
				.append("	describe() {\n")
				.append("		if (this.hasTopping) {\n")
				.append("	    print \"This delectable delicacy is \" + this.topping + \" on \" + this.food + \".\";\n")
				.append("			return;\n")
				.append("		}\n")
				.append("	  print \"This delectable delicacy is \" + this.food + \".\";\n")
				.append("	}\n")
				.append("}\n")
				.append("\n")
				.append("var food = FoodWithTopping(\"toast\", nil);\n")
				.append("\n")
				.append("food.describe();\n")
				.toString();

		lox.run(script);

		var expect = "This delectable delicacy is toast.\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void inheritMethodsFromSuperclass() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class Doughnut {
				  cook() {
				    print "Fry until golden brown.";
				  }
				}

				class BostonCream < Doughnut {}

				BostonCream().cook();
				""";

		lox.run(script);

		var expect = "Fry until golden brown.\n";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void inheritMethodsWithAdditional() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class Doughnut {
				  cook() {
				    print "Fry until golden brown.";
				  }
				}

				class BostonCream < Doughnut {
				  fill() {
				    print "With lots of creme.";
				  }
				}

				BostonCream().cook();
				BostonCream().fill();
				""";

		lox.run(script);

		var expect = """
				Fry until golden brown.
				With lots of creme.
				""";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void overrideInheritedMethod() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class Doughnut {
				  cook() {
				    print "Fry until golden brown.";
				  }
				}

				class BostonCream < Doughnut {
				  cook() {
						print "Fry until crispy.";
				  }
				  fill() {
				    print "With lots of creme.";
				  }
				}

				BostonCream().cook();
				BostonCream().fill();
				""";

		lox.run(script);

		var expect = """
				Fry until crispy.
				With lots of creme.
				""";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void callSuperMethod() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class Doughnut {
				  cook() {
				    print "Fry until golden brown.";
				  }
				}

				class BostonCream < Doughnut {
				  cook() {
				    super.cook();
				    print "Pipe full of custard and coat with chocolate.";
				  }
				}

				BostonCream().cook();
				""";

		lox.run(script);

		var expect = """
				Fry until golden brown.
				Pipe full of custard and coat with chocolate.
				""";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void callMultiInheritanceSuperMethod() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class FriedFood {
				  cook() {
				    print "Submerge in hot oil.";
				  }
				}

				class Doughnut < FriedFood {
				  cook() {
				    super.cook();
				    print "Fry until golden brown.";
				  }
				}

				class BostonCream < Doughnut {
				  cook() {
				    super.cook();
				    print "Pipe full of custard and coat with chocolate.";
				  }
				}

				BostonCream().cook();
				""";

		lox.run(script);

		var expect = """
				Submerge in hot oil.
				Fry until golden brown.
				Pipe full of custard and coat with chocolate.
				""";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void ShareParentState() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class FriedFood {
				  cook() {
				    this.instructions = this.instructions + "Submerge in hot oil.\n";
				  }
				}

				class Doughnut < FriedFood {
				  cook() {
				    super.cook();
				    this.instructions = this.instructions + "Fry until golden brown.\n";
				  }
				}

				class BostonCream < Doughnut {
				  cook() {
				    this.instructions = "Instructions to make Boston Cream doughnut.\n";
				    super.cook();
				    this.instructions = this.instructions + "Pipe full of custard and coat with chocolate.";
				  }
				}

				var bostonCream = BostonCream();
				bostonCream.cook();
				print bostonCream.instructions;
				""";

		lox.run(script);

		var expect = """
				Instructions to make Boston Cream doughnut.
				Submerge in hot oil.
				Fry until golden brown.
				Pipe full of custard and coat with chocolate.
				""";

		var result = output.get();

		assertEquals(expect, result);
	}

	@org.junit.jupiter.api.Test
	void MethodStubs() {
		var lox = new Lox();
		var output = new CapturedOutput();
		var script = """
				class Doughnut {
				  cook()
				}

				class BostonCream < Doughnut {
				  cook() {
						print "Pipe full of custard and coat with chocolate."
				  }
				}

				var bostonCream = BostonCream();
				bostonCream.cook();
				""";

		lox.run(script);

		var expect = """
				Pipe full of custard and coat with chocolate.
				""";

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

	class CapturedErrOutput {
		ByteArrayOutputStream output = new ByteArrayOutputStream();

		CapturedErrOutput() {
			System.setErr(new PrintStream(output));
		}

		public String get() {
			return output.toString();
		}
	}
}

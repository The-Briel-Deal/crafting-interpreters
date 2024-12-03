package org.craftinginterpreters.lox;

class AstPrinter implements Expr.Visitor<String> {
	public static void main(String[] args) {
		// 1 + 2 - 3 * 4
		var unary = new Expr.Unary(
				new Token(TokenType.MINUS, "-", null, 1),
				new Expr.Literal(123));
		var grouping = new Expr.Grouping(new Expr.Literal(45.67));

		var expression = new Expr.Binary(unary, new Token(TokenType.STAR, "*", null, 1), grouping);

		System.out.println(new AstPrinter().print(expression));
	}

	String print(Expr expr) {
		return expr.accept(this);
	}

	@Override
	public String visitBinaryExpr(Expr.Binary expr) {
		return parenthesize(expr.operator.lexeme, expr.left, expr.right);
	}

	@Override
	public String visitGroupingExpr(Expr.Grouping expr) {
		return parenthesize("group", expr.expression);
	}

	@Override
	public String visitLiteralExpr(Expr.Literal expr) {
		if (expr.value == null)
			return "nil";
		return expr.value.toString();
	}

	@Override
	public String visitUnaryExpr(Expr.Unary expr) {
		return parenthesize(expr.operator.lexeme, expr.right);
	}

	private String parenthesize(String name, Expr... exprs) {
		StringBuilder builder = new StringBuilder();

		builder.append("(").append(name);
		for (Expr expr : exprs) {
			builder.append(" ");
			builder.append(expr.accept(this));
		}
		builder.append(")");

		return builder.toString();
	}
}

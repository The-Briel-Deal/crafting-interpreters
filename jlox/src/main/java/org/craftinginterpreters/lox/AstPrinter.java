package org.craftinginterpreters.lox;

import org.craftinginterpreters.lox.Expr.Assign;
import org.craftinginterpreters.lox.Expr.Call;
import org.craftinginterpreters.lox.Expr.Get;
import org.craftinginterpreters.lox.Expr.Logical;
import org.craftinginterpreters.lox.Expr.Set;
import org.craftinginterpreters.lox.Expr.This;
import org.craftinginterpreters.lox.Stmt.Block;
import org.craftinginterpreters.lox.Stmt.Class;
import org.craftinginterpreters.lox.Stmt.Expression;
import org.craftinginterpreters.lox.Stmt.Function;
import org.craftinginterpreters.lox.Stmt.If;
import org.craftinginterpreters.lox.Stmt.Print;
import org.craftinginterpreters.lox.Stmt.Return;
import org.craftinginterpreters.lox.Stmt.Var;
import org.craftinginterpreters.lox.Stmt.While;

class AstPrinter implements Expr.Visitor<String>, Stmt.Visitor<String> {
	String print(Stmt... stmts) {
		var output = new StringBuilder();
		for (var stmt : stmts)
			output.append(stmt.accept(this) + "\n");
		return output.toString();
	}

	/* Expr */
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

	@Override
	public String visitVariableExpr(Expr.Variable expr) {
		return String.format("Var '%s'", expr.name);
	}

	@Override
	public String visitAssignExpr(Assign expr) {
		return String.format("%s = %s", expr.name, expr.value);
	}

	@Override
	public String visitLogicalExpr(Logical expr) {
		return parenthesize(expr.operator.lexeme, expr.left, expr.right);
	}

	@Override
	public String visitCallExpr(Call expr) {
		return parenthesize(expr.callee.toString(), (Object[]) expr.arguments.toArray(new Expr[0]));
	}

	@Override
	public String visitGetExpr(Get expr) {
		return parenthesize("getExpr " + expr.name.lexeme, expr.object);
	}

	@Override
	public String visitSetExpr(Set expr) {
		return parenthesize("setExpr " + expr.name.lexeme, expr.object, expr.value);
	}

	@Override
	public String visitThisExpr(This expr) {
		return expr.keyword.lexeme;
	}

	/* Stmt */
	@Override
	public String visitIfStmt(If stmt) {
		return parenthesize("ifStmt", stmt.condition, stmt.thenBranch, stmt.elseBranch);
	}

	@Override
	public String visitVarStmt(Var stmt) {
		return parenthesize("varStmt", stmt.initializer);
	}

	@Override
	public String visitBlockStmt(Block stmt) {
		return parenthesize("blockStmt", stmt.statements);
	}

	@Override
	public String visitClassStmt(Class stmt) {
		return parenthesize("class " + stmt.name.lexeme, stmt.methods);
	}

	@Override
	public String visitPrintStmt(Print stmt) {
		return parenthesize("print", stmt.expression);
	}

	@Override
	public String visitWhileStmt(While stmt) {
		return parenthesize("while", stmt.condition, stmt.body);
	}

	@Override
	public String visitReturnStmt(Return stmt) {
		return parenthesize("return", stmt.value);
	}

	@Override
	public String visitFunctionStmt(Function stmt) {
		return parenthesize("function " + stmt.name.lexeme, stmt.params, stmt.body);
	}

	@Override
	public String visitExpressionStmt(Expression stmt) {
		return parenthesize("exprStmt", stmt.expression);
	}

	private String parenthesize(String name, Object... visitors) {
		StringBuilder builder = new StringBuilder();

		builder.append("(").append(name);
		for (Object visitor : visitors) {
			builder.append(" ");
			if (visitor instanceof Expr expr)
				builder.append(expr.accept(this));
			if (visitor instanceof Stmt stmt)
				builder.append(stmt.accept(this));
		}
		builder.append(")");

		return builder.toString();
	}
}

package org.craftinginterpreters.lox;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import org.craftinginterpreters.lox.Expr.Assign;
import org.craftinginterpreters.lox.Expr.Binary;
import org.craftinginterpreters.lox.Expr.Call;
import org.craftinginterpreters.lox.Expr.Get;
import org.craftinginterpreters.lox.Expr.Grouping;
import org.craftinginterpreters.lox.Expr.Literal;
import org.craftinginterpreters.lox.Expr.Logical;
import org.craftinginterpreters.lox.Expr.This;
import org.craftinginterpreters.lox.Expr.Unary;
import org.craftinginterpreters.lox.Expr.Variable;
import org.craftinginterpreters.lox.Stmt.Block;
import org.craftinginterpreters.lox.Stmt.Class;
import org.craftinginterpreters.lox.Stmt.Expression;
import org.craftinginterpreters.lox.Stmt.Function;
import org.craftinginterpreters.lox.Stmt.Getter;
import org.craftinginterpreters.lox.Stmt.If;
import org.craftinginterpreters.lox.Stmt.Print;
import org.craftinginterpreters.lox.Stmt.Return;
import org.craftinginterpreters.lox.Stmt.Var;
import org.craftinginterpreters.lox.Stmt.While;

class Resolver implements Expr.Visitor<Void>, Stmt.Visitor<Void> {
	private final Lox lox;
	private final Interpreter interpreter;

	private final Stack<Map<String, Boolean>> scopes = new Stack<>();

	private enum FunctionType {
		NONE,
		FUNCTION,
		INITIALIZER,
		METHOD,
		GETTER
	}

	private FunctionType currentFunction = FunctionType.NONE;

	private enum ClassType {
		NONE,
		CLASS
	}

	private ClassType currentClass = ClassType.NONE;

	Resolver(Interpreter interpreter, Lox lox) {
		this.interpreter = interpreter;
		this.lox = lox;
	}

	@Override
	public Void visitBlockStmt(Block stmt) {
		beginScope();
		resolve(stmt.statements);
		endScope();
		return null;
	}

	@Override
	public Void visitClassStmt(Class stmt) {
		ClassType enclosingClass = currentClass;
		currentClass = ClassType.CLASS;

		declare(stmt.name);
		define(stmt.name);

		beginScope();
		scopes.peek().put("this", true);

		for (Stmt.Function method : stmt.methods) {
			FunctionType declaration = FunctionType.METHOD;
			if (method.name.lexeme.equals("init")) {
				declaration = FunctionType.INITIALIZER;
			}
			resolveFunction(method, declaration);
		}
		for (Stmt.Getter getter : stmt.getters) {
			resolveGetter(getter);
		}

		endScope();
		currentClass = enclosingClass;

		return null;
	}

	@Override
	public Void visitExpressionStmt(Expression stmt) {
		resolve(stmt.expression);
		return null;
	}

	@Override
	public Void visitIfStmt(If stmt) {
		resolve(stmt.condition);
		resolve(stmt.thenBranch);
		if (stmt.elseBranch != null)
			resolve(stmt.elseBranch);
		return null;
	}

	@Override
	public Void visitPrintStmt(Print stmt) {
		resolve(stmt.expression);
		return null;
	}

	@Override
	public Void visitReturnStmt(Return stmt) {
		if (currentFunction == FunctionType.NONE) {
			lox.error(stmt.keyword, "Can't return from top-level code.");
		}

		if (stmt.value != null) {
			if (currentFunction == FunctionType.INITIALIZER) {
				lox.error(stmt.keyword, "Can't return a value from an initializer.");
			}

			resolve(stmt.value);
		}

		return null;
	}

	@Override
	public Void visitFunctionStmt(Function stmt) {
		declare(stmt.name);
		define(stmt.name);

		resolveFunction(stmt, FunctionType.FUNCTION);
		return null;
	}

	@Override
	public Void visitGetterStmt(Getter stmt) {
		declare(stmt.name);
		define(stmt.name);

		resolveGetter(stmt);
		return null;
	}

	void resolve(List<Stmt> statements) {
		for (Stmt statement : statements) {
			resolve(statement);
		}
	}

	private void resolve(Stmt stmt) {
		stmt.accept(this);
	}

	private void resolve(Expr expr) {
		expr.accept(this);
	}

	private void resolveFunction(Stmt.Function function, FunctionType type) {
		FunctionType enclosingFunction = currentFunction;
		currentFunction = type;

		beginScope();
		for (Token param : function.params) {
			declare(param);
			define(param);
		}
		resolve(function.body);
		endScope();
		currentFunction = enclosingFunction;
	}
	private void resolveGetter(Stmt.Getter getter) {
		FunctionType enclosingFunction = currentFunction;
		currentFunction = FunctionType.GETTER;

		beginScope();
		resolve(getter.body);
		endScope();
		currentFunction = enclosingFunction;
	}

	private void beginScope() {
		scopes.push(new HashMap<String, Boolean>());
	}

	private void endScope() {
		scopes.pop();
	}

	private void declare(Token name) {
		if (scopes.isEmpty())
			return;

		Map<String, Boolean> scope = scopes.peek();
		if (scope.containsKey(name.lexeme)) {
			lox.error(name, "Already a variable with this name in this scope.");
		}

		scope.put(name.lexeme, false);
	}

	private void define(Token name) {
		if (scopes.isEmpty())
			return;
		scopes.peek().put(name.lexeme, true);
	}

	private void resolveLocal(Expr expr, Token name) {
		for (int i = scopes.size() - 1; i >= 0; i--) {
			if (scopes.get(i).containsKey(name.lexeme)) {
				interpreter.resolve(expr, scopes.size() - 1 - i);
			}
		}
	}

	@Override
	public Void visitVarStmt(Var stmt) {
		declare(stmt.name);
		if (stmt.initializer != null) {
			resolve(stmt.initializer);
		}
		define(stmt.name);
		return null;
	}

	@Override
	public Void visitWhileStmt(While stmt) {
		resolve(stmt.condition);
		resolve(stmt.body);
		return null;
	}

	@Override
	public Void visitAssignExpr(Assign expr) {
		resolve(expr.value);
		resolveLocal(expr, expr.name);
		return null;
	}

	@Override
	public Void visitBinaryExpr(Binary expr) {
		resolve(expr.left);
		resolve(expr.right);
		return null;
	}

	@Override
	public Void visitCallExpr(Call expr) {
		resolve(expr.callee);

		for (Expr argument : expr.arguments) {
			resolve(argument);
		}

		return null;
	}

	@Override
	public Void visitGetExpr(Get expr) {
		resolve(expr.object);
		return null;
	}

	@Override
	public Void visitGroupingExpr(Grouping expr) {
		resolve(expr.expression);
		return null;
	}

	@Override
	public Void visitLiteralExpr(Literal expr) {
		// Literal expressions don't mention variables so there is nothing to resolve.
		return null;
	}

	@Override
	public Void visitLogicalExpr(Logical expr) {
		resolve(expr.left);
		resolve(expr.right);
		return null;
	}

	@Override
	public Void visitSetExpr(Expr.Set expr) {
		resolve(expr.value);
		resolve(expr.object);
		return null;
	}

	@Override
	public Void visitThisExpr(This expr) {
		if (currentClass == ClassType.NONE) {
			lox.error(expr.keyword, "Can't use 'this' outside of a class.");
			return null;
		}
		resolveLocal(expr, expr.keyword);
		return null;
	}

	@Override
	public Void visitUnaryExpr(Unary expr) {
		resolve(expr.right);
		return null;
	}

	@Override
	public Void visitVariableExpr(Variable expr) {
		if (!scopes.isEmpty() && scopes.peek().get(expr.name.lexeme) == Boolean.FALSE) {
			lox.error(expr.name, "Can't read local variable in its own initializer.");
		}

		resolveLocal(expr, expr.name);
		return null;
	}

}

package org.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

class Environment {
	final Environment enclosing;
	private final ArrayList<Object> values = new ArrayList<>();

	Environment() {
		enclosing = null;
	}

	Environment(Environment enclosing) {
		this.enclosing = enclosing;
	}

	void define(Token token, Object value) {
		int index = values.size();
		values.add(value);
		token.var_declaration_index = index;
	}

	Object getAt(int distance, String name) {
		return ancestor(distance).values.get(name);
	}

	void assignAt(int distance, Token name, Object value) {
		ancestor(distance).values.put(name.lexeme, value);
	}

	Environment ancestor(int distance) {
		var environment = this;
		for (int i = 0; i < distance; i++) {
			environment = environment.enclosing;
		}

		return environment;
	}

	Object get(Token name) {
		if (values.containsKey(name.lexeme)) {
			return values.get(name.lexeme);
		}

		if (enclosing != null)
			return enclosing.get(name);

		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}

	void assign(Token name, Object value) {
		if (values.containsKey(name.lexeme)) {
			values.put(name.lexeme, value);
			return;
		}

		if (enclosing != null) {
			enclosing.assign(name, value);
			return;
		}

		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}
}

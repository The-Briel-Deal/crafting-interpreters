package org.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.List;

class Environment {
	final Environment enclosing;
	private final List<Object> values = new ArrayList<>();

	Environment() {
		enclosing = null;
	}

	Environment(Environment enclosing) {
		this.enclosing = enclosing;
	}

	void define(Object value) {
		values.add(value);
	}

	Object getAt(int distance, int index) {
		return ancestor(distance).values.get(index);
	}

	void assignAt(int distance, int index, Object value) {
		ancestor(distance).values.set(index, value);
	}

	Environment ancestor(int distance) {
		var environment = this;
		for (int i = 0; i < distance; i++) {
			environment = environment.enclosing;
		}

		return environment;
	}

	Object get(Token name, int index) {
		if (values.get(index) != null) {
			return values.get(index);
		}

		if (enclosing != null)
			return enclosing.get(name, index);

		throw new RuntimeError(name, "Could not find item at index: " + index);
	}

	void assign(Token name, int index, Object value) {
		if (values.get(index) != null) {
			values.set(index, value);
			return;
		}

		if (enclosing != null) {
			enclosing.assign(name, index, value);
			return;
		}

		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}
}

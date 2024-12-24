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

	void define(String name, Object value) {
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
}

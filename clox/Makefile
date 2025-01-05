HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

build/clox: $(OBJECTS)
	gcc $^ -std=c99 -Iinclude -o build/clox

build/:
	mkdir -p build

# Compile Object Files
build/%.o: src/%.c $(HEADERS) | build/
	gcc $< -c -std=c99 -Iinclude -o $@

.PHONY: clean run
clean:
	rm build/*.o build/clox 

run: build/clox
	exec build/clox


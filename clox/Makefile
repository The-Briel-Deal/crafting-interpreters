HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

# Compile Executable
build/clox: $(OBJECTS)
	gcc $^ -std=c99 -Iinclude -o build/clox

# Compile Object Files
build/%.o: src/%.c $(HEADERS) | build/
	gcc $< -c -std=c99 -Iinclude -o $@

build/:
	mkdir -p build


.PHONY: clean run
clean:
	rm -r ./build 

run: build/clox
	exec build/clox


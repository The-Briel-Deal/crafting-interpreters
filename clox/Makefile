CFLAGS  := -std=c99 -Iinclude
HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

# Compile Executable
build/clox: $(OBJECTS)
	gcc $^ $(CFLAGS) -o $@

# Compile Object Files
build/%.o: src/%.c $(HEADERS) | build/
	gcc $< -c $(CFLAGS) -o $@

build/:
	mkdir -p build


.PHONY: clean run
clean:
	rm -r ./build 

run: build/clox
	exec build/clox


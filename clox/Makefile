CFLAGS  := -std=gnu99 -Iinclude -g
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


# Janky Test Setup
build/test: build/ test/main.c $(SOURCES)
	gcc test/main.c $(filter-out %/main.c, $(SOURCES)) $(CFLAGS)  -o build/test

.PHONY: clean run test
clean:
	rm -r ./build 

run: build/clox
	exec build/clox

test: build/test
	exec build/test



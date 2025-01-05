HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))


# Compile Object Files
build/%.o: src/%.c $(HEADERS)
	echo "Building Object Files"
	gcc $< -c -std=c99 -Iinclude -o $@

build/clox: $(OBJECTS)
	echo "Compiling Clox"
	mkdir -p build
	gcc $^ -std=c99 -Iinclude -o build/clox

CFLAGS  := -std=gnu99 -Iinclude
HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

ifeq (${DEBUG_SYM}, 1)
	CFLAGS += -g
endif
ifeq (${DEBUG_TRACE_EXECUTION}, 1)
	CFLAGS += -DDEBUG_TRACE_EXECUTION
endif
ifeq (${DEBUG_PRINT_CODE}, 1)
	CFLAGS += -DDEBUG_PRINT_CODE
endif

# Compile Executable
clox: build/clox
build/clox: $(OBJECTS)
	gcc $^ $(CFLAGS) -o $@

# Compile Object Files
build/%.o: src/%.c $(HEADERS) | build/
	gcc $< -c $(CFLAGS) -o $@

build/:
	mkdir -p build

test: build/test
build/test: build/ test/main.c $(SOURCES)
	gcc test/main.c $(filter-out %/main.c, $(SOURCES)) $(CFLAGS)  -o build/test

.PHONY: clean run run-test
clean:
	rm -r ./build 

run-clox: build/clox
	exec build/clox

run-test: build/test
	exec build/test



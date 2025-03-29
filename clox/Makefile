CFLAGS  := -std=gnu99 -Iinclude -Wall -Werror
HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

TEST_HEADERS := $(HEADERS) $(wildcard test/include/*.h)
TEST_SOURCES := $(wildcard test/src/*.c)
TEST_OBJECTS := $(filter-out %/main.o, $(OBJECTS)) $(addprefix build/test/, $(notdir $(TEST_SOURCES:.c=.o)))

ifeq (${DEBUG_SYM}, 1)
	CFLAGS += -g
endif
ifeq (${DEBUG_TRACE_EXECUTION}, 1)
	CFLAGS += -DDEBUG_TRACE_EXECUTION
endif
ifeq (${DEBUG_PRINT_CODE}, 1)
	CFLAGS += -DDEBUG_PRINT_CODE
endif
ifeq (${DEBUG_STRESS_GC}, 1)
	CFLAGS += -DDEBUG_STRESS_GC
endif
ifeq (${DEBUG_LOG_GC}, 1)
	CFLAGS += -DDEBUG_LOG_GC
endif
ifeq (${OPTIMIZE}, 1)
	CFLAGS += -O3
	CFLAGS += -DNDEBUG
endif
ifeq (${NAN_BOXING}, 1)
	CFLAGS += -DNAN_BOXING
endif

# This needs to be below the above modifications to CFLAGS or this var won't contain the changes.
TEST_CFLAGS := $(CFLAGS) -Itest/include

# Compile Executable
clox: build/clox
build/clox: $(OBJECTS)
	gcc $^ $(CFLAGS) -o $@

# Compile Object Files
build/%.o: src/%.c $(HEADERS) | build/
	gcc $< -c $(CFLAGS) -o $@

build/:
	mkdir -p build

test: build/test/test
build/test/test: $(TEST_OBJECTS)
	gcc $^ $(TEST_CFLAGS) -o build/test/test

# Compile Object Files
build/test/%.o: test/src/%.c $(TEST_HEADERS) | build/ build/test/
	gcc $< -c $(TEST_CFLAGS) -o $@

build/test/:
	mkdir -p build/test

# Benchmarking

benchmark: build/benchmark/benchmark
build/benchmark/benchmark: $(filter-out %/main.o, $(OBJECTS)) build/benchmark/main.o
	gcc $^ $(CFLAGS) -o build/benchmark/benchmark

# Compile Object Files
build/benchmark/%.o: benchmark/src/%.c $(HEADERS) | build/ build/benchmark/
	gcc $< -c $(CFLAGS) -o $@

build/benchmark/:
	mkdir -p build/benchmark

.PHONY: clean run run-test
clean:
	rm -r ./build 

run-clox: build/clox
	exec $<

run-test: build/test/test
	exec $<

run-bench: build/benchmark/benchmark
	exec $<



#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"

#include "helper_test.h"

const char TEST_WRITE_CHUNK_EXPECT[] = "== test chunk ==\n"
                                       "0000  123 OP_CONSTANT         0 '1.2'\n"
                                       "0002    | OP_RETURN\n";
void       testWriteChunk() {
  Chunk chunk;
  initChunk(&chunk);

  CONSTANT(NUMBER_VAL(1.2), 123);

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}
const char TEST_WRITE_CHUNK_FROM_SRC_EXPECT[] =
    "== test chunk ==\n"
    "0000    1 OP_CONSTANT         1 'greasy'\n"
    "0002    | OP_DEFINE_GLOBAL    0 'beans'\n"
    "0004    | OP_RETURN\n";
void testWriteChunkFromSrc() {
  Chunk chunk;
  initChunk(&chunk);

  compile("var beans = \"greasy\";", &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const TestCase CHUNK_TESTS[] = {
    {"testWriteChunk",        testWriteChunk,        TEST_WRITE_CHUNK_EXPECT},
    {"testWriteChunkFromSrc", testWriteChunkFromSrc,
     TEST_WRITE_CHUNK_FROM_SRC_EXPECT                                       },
};

const int CHUNK_TESTS_COUNT = sizeof(CHUNK_TESTS) / sizeof(TestCase);

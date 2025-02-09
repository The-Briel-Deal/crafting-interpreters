
#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"

#include "helper_test.h"

const char TEST_WRITE_CHUNK_EXPECT[] =
    "== test chunk ==\n"
    "0000  123 OP_CONSTANT         0 '1.2'\n"
    "0002    | OP_RETURN\n";
void testWriteChunk() {
  Chunk chunk;
  initChunk(&chunk);

  CONSTANT(NUMBER_VAL(1.2), 123);

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}
const char TEST_CHUNK_DEF_GLOBAL_EXPECT[] =
    "== test chunk ==\n"
    "0000    1 OP_CONSTANT         1 'greasy'\n"
    "0002    | OP_DEFINE_GLOBAL    0 'beans'\n"
    "0004    | OP_RETURN\n";
void testChunkDefGlobal() {
  Chunk chunk;
  initChunk(&chunk);

  compile("var beans = \"greasy\";", &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const char TEST_CHUNK_SET_GLOBAL_EXPECT[] =
    "== test chunk ==\n"
    "0000    1 OP_CONSTANT         1 'greasy'\n"
    "0002    | OP_DEFINE_GLOBAL    0 'beans'\n"
    "0004    2 OP_CONSTANT         3 'stinky'\n"
    "0006    | OP_SET_GLOBAL       2 'beans'\n"
    "0008    | OP_POP\n"
    "0009    3 OP_RETURN\n";
void testChunkSetGlobal() {
  Chunk chunk;
  initChunk(&chunk);

  compile(
      "var beans = \"greasy\";\n"
      "beans = \"stinky\";\n",
      &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const char TEST_CHUNK_GET_GLOBAL_EXPECT[] =
    "== test chunk ==\n"
    "0000    1 OP_CONSTANT         1 'greasy'\n"
    "0002    | OP_DEFINE_GLOBAL    0 'beans'\n"
    "0004    2 OP_CONSTANT         3 'stinky'\n"
    "0006    | OP_SET_GLOBAL       2 'beans'\n"
    "0008    | OP_POP\n"
    "0009    3 OP_GET_GLOBAL       5 'beans'\n"
    "0011    | OP_DEFINE_GLOBAL    4 'dog'\n"
    "0013    4 OP_RETURN\n";
void testChunkGetGlobal() {
  Chunk chunk;
  initChunk(&chunk);

  compile(
      "var beans = \"greasy\";\n"
      "beans = \"stinky\";\n"
      "var dog = beans;\n",
      &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const char TEST_CHUNK_LOCAL_EXPECT[] =
    "== test chunk ==\n"
    "0000    2 OP_CONSTANT         0 'greasy'\n"
    "0002    4 OP_CONSTANT         1 'humungooseeee'\n"
    "0004    | OP_SET_LOCAL        0\n"
    "0007    | OP_POP\n"
    "0008    6 OP_GET_LOCAL        0\n"
    "0011    | OP_PRINT\n"
    "0012    7 OP_POP\n"
    "0013    8 OP_RETURN\n";

void testChunkLocal() {
  Chunk chunk;
  initChunk(&chunk);

  compile(
      "{\n"
      "  var beans = \"greasy\";\n"
      "  {\n"
      "    beans = \"humungooseeee\";\n"
      "  }\n"
      "  print(beans);\n"
      "}\n",
      &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const TestCase CHUNK_TESTS[] = {
    {"testWriteChunk",     testWriteChunk,     TEST_WRITE_CHUNK_EXPECT     },
    {"testChunkDefGlobal", testChunkDefGlobal, TEST_CHUNK_DEF_GLOBAL_EXPECT},
    {"testChunkSetGlobal", testChunkSetGlobal, TEST_CHUNK_SET_GLOBAL_EXPECT},
    {"testChunkGetGlobal", testChunkGetGlobal, TEST_CHUNK_GET_GLOBAL_EXPECT},
    {"testChunkLocal",     testChunkLocal,     TEST_CHUNK_LOCAL_EXPECT     },
};

const int CHUNK_TESTS_COUNT = sizeof(CHUNK_TESTS) / sizeof(TestCase);

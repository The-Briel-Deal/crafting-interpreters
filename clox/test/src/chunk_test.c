
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
    "0006    | OP_POP\n"
    "0007    6 OP_GET_LOCAL        0\n"
    "0009    | OP_PRINT\n"
    "0010    7 OP_POP\n"
    "0011    8 OP_RETURN\n";

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
const char TEST_CHUNK_SWITCH_EXPECT[] =
    "== test chunk ==\n"
    "0000    1 OP_CONSTANT         1 '2'\n"
    "0002    | OP_DEFINE_GLOBAL    0 'i'\n"
    "0004    2 OP_GET_GLOBAL       2 'i'\n"
    "0006    3 OP_DUP\n"
    "0007    | OP_CONSTANT         3 '1'\n"
    "0009    | OP_EQUAL\n"
    "0010    | OP_JUMP_IF_FALSE   10 -> 19\n"
    "0013    4 OP_CONSTANT         4 'is one'\n"
    "0015    | OP_PRINT\n"
    "0016    5 OP_JUMP            16 -> 51\n"
    "0019    | OP_POP\n"
    "0020    | OP_DUP\n"
    "0021    | OP_CONSTANT         5 '2'\n"
    "0023    | OP_EQUAL\n"
    "0024    | OP_JUMP_IF_FALSE   24 -> 33\n"
    "0027    6 OP_CONSTANT         6 'is two'\n"
    "0029    | OP_PRINT\n"
    "0030    7 OP_JUMP            30 -> 51\n"
    "0033    | OP_POP\n"
    "0034    | OP_DUP\n"
    "0035    | OP_CONSTANT         7 '3'\n"
    "0037    | OP_EQUAL\n"
    "0038    | OP_JUMP_IF_FALSE   38 -> 47\n"
    "0041    8 OP_CONSTANT         8 'is three'\n"
    "0043    | OP_PRINT\n"
    "0044    9 OP_JUMP            44 -> 51\n"
    "0047    | OP_POP\n"
    "0048   10 OP_CONSTANT         9 'default case'\n"
    "0050    | OP_PRINT\n"
    "0051   11 OP_POP\n"
    "0052   12 OP_RETURN\n";

void testChunkSwitch() {
  Chunk chunk;
  initChunk(&chunk);

  char source[] =
      "var i = 2;                   \n"
      "switch (i) {                 \n"
      "  case 1:                    \n"
      "    print(\"is one\");       \n"
      "  case 2:                    \n"
      "  	 print(\"is two\");       \n"
      "  case 3:                    \n"
      "  	 print(\"is three\");     \n"
      "  default:                   \n"
      "  	 print(\"default case\"); \n"
      "}                            \n";
  compile(source, &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const TestCase CHUNK_TESTS[] = {
    {"testWriteChunk",     testWriteChunk,     TEST_WRITE_CHUNK_EXPECT     },
    {"testChunkDefGlobal", testChunkDefGlobal, TEST_CHUNK_DEF_GLOBAL_EXPECT},
    {"testChunkSetGlobal", testChunkSetGlobal, TEST_CHUNK_SET_GLOBAL_EXPECT},
    {"testChunkGetGlobal", testChunkGetGlobal, TEST_CHUNK_GET_GLOBAL_EXPECT},
    {"testChunkLocal",     testChunkLocal,     TEST_CHUNK_LOCAL_EXPECT     },
    {"testChunkSwitch",    testChunkSwitch,    TEST_CHUNK_SWITCH_EXPECT    },
};

const int CHUNK_TESTS_COUNT = sizeof(CHUNK_TESTS) / sizeof(TestCase);

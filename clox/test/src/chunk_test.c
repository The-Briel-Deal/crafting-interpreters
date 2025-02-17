
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
    "0006    3 OP_JUMP             6 -> 48\n"
    "0009    | OP_DUP\n"
    "0010    | OP_CONSTANT         3 '1'\n"
    "0012    | OP_EQUAL\n"
    "0013    | OP_JUMP_IF_FALSE   13 -> 22\n"
    "0016    4 OP_CONSTANT         4 'is one'\n"
    "0018    | OP_PRINT\n"
    "0019    5 OP_JUMP            19 -> 48\n"
    "0022    | OP_DUP\n"
    "0023    | OP_CONSTANT         5 '2'\n"
    "0025    | OP_EQUAL\n"
    "0026    | OP_JUMP_IF_FALSE   26 -> 35\n"
    "0029    6 OP_CONSTANT         6 'is two'\n"
    "0031    | OP_PRINT\n"
    "0032    7 OP_JUMP            32 -> 48\n"
    "0035    | OP_DUP\n"
    "0036    | OP_CONSTANT         7 '3'\n"
    "0038    | OP_EQUAL\n"
    "0039    | OP_JUMP_IF_FALSE   39 -> 45\n"
    "0042    8 OP_CONSTANT         8 'is three'\n"
    "0044    | OP_PRINT\n"
    "0045   10 OP_CONSTANT         9 'default case'\n"
    "0047    | OP_PRINT\n"
    "0048   11 OP_POP\n"
    "0049   12 OP_RETURN\n";

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

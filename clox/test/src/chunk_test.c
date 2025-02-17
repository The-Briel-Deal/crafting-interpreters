
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

const char TEST_CHUNK_LOOPS_EXPECT[] =
    "== test chunk ==\n"
    "0000    2 OP_CONSTANT         0 '1'\n"
    "0002    | OP_GET_LOCAL        0\n"
    "0004    | OP_CONSTANT         1 '10'\n"
    "0006    | OP_LESS\n"
    "0007    | OP_JUMP_IF_FALSE    7 -> 61\n"
    "0010    | OP_POP\n"
    "0011    | OP_JUMP            11 -> 25\n"
    "0014    | OP_GET_LOCAL        0\n"
    "0016    | OP_CONSTANT         2 '1'\n"
    "0018    | OP_ADD\n"
    "0019    | OP_SET_LOCAL        0\n"
    "0021    | OP_POP\n"
    "0022    | OP_LOOP            22 -> 2\n"
    "0025    4 OP_CONSTANT         3 '2'\n"
    "0027    6 OP_CONSTANT         4 '3'\n"
    "0029    7 OP_CONSTANT         5 '9'\n"
    "0031    8 OP_CONSTANT         6 '9'\n"
    "0033    9 OP_GET_LOCAL        2\n"
    "0035    | OP_CONSTANT         7 '20'\n"
    "0037    | OP_LESS\n"
    "0038    | OP_JUMP_IF_FALSE   38 -> 53\n"
    "0041    | OP_POP\n"
    "0042   10 OP_GET_LOCAL        2\n"
    "0044    | OP_CONSTANT         8 '1'\n"
    "0046    | OP_ADD\n"
    "0047    | OP_SET_LOCAL        2\n"
    "0049    | OP_POP\n"
    "0050   11 OP_LOOP            50 -> 33\n"
    "0053    | OP_POP\n"
    "0054   12 OP_POP\n"
    "0055    | OP_POP\n"
    "0056    | OP_POP\n"
    "0057   13 OP_POP\n"
    "0058   14 OP_LOOP            58 -> 14\n"
    "0061    | OP_POP\n"
    "0062    | OP_POP\n"
    "0063   15 OP_RETURN\n";

void testChunkLoops() {
  Chunk chunk;
  initChunk(&chunk);

  compile(
      "// Scope 0\n"
      "for (var a = 1; a < 10; a = a + 1) { // Scope 1\n"
      "  { // Scope 2\n"
      "    var b = 2;\n"
      "    { // Scope 3\n"
      "      var c = 3; \n"
      "      var d = 9; \n"
      "      var z = 9; \n"
      "      while (c < 20) { // Scope 4\n"
      "			  c = c + 1;\n"
      "      }\n"
      "    }\n"
      "  }\n"
      "}\n",
      &chunk);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

const char TEST_CHUNK_CONTINUE_EXPECT[] =
    "== test chunk ==\n"
    "0000    1 OP_CONSTANT         0 '0'\n"
    "0002    | OP_GET_LOCAL        0\n"
    "0004    | OP_CONSTANT         1 '10'\n"
    "0006    | OP_LESS\n"
    "0007    | OP_JUMP_IF_FALSE    7 -> 60\n"
    "0010    | OP_POP\n"
    "0011    | OP_JUMP            11 -> 25\n"
    "0014    | OP_GET_LOCAL        0\n"
    "0016    | OP_CONSTANT         2 '1'\n"
    "0018    | OP_ADD\n"
    "0019    | OP_SET_LOCAL        0\n"
    "0021    | OP_POP\n"
    "0022    | OP_LOOP            22 -> 2\n"
    "0025    2 OP_GET_LOCAL        0\n"
    "0027    | OP_CONSTANT         3 '2'\n"
    "0029    | OP_EQUAL\n"
    "0030    | OP_JUMP_IF_FALSE   30 -> 36\n"
    "0033    | OP_JUMP            33 -> 42\n"
    "0036    | OP_POP\n"
    "0037    | OP_GET_LOCAL        0\n"
    "0039    | OP_CONSTANT         4 '5'\n"
    "0041    | OP_EQUAL\n"
    "0042    | OP_JUMP_IF_FALSE   42 -> 52\n"
    "0045    | OP_POP\n"
    "0046    3 OP_JUMP            46 -> 57\n"
    "0049    | OP_JUMP            49 -> 53\n"
    "0052    | OP_POP\n"
    "0053    4 OP_GET_LOCAL        0\n"
    "0055    | OP_PRINT\n"
    "0056    5 OP_POP\n"
    "0057    | OP_LOOP            57 -> 14\n"
    "0060    | OP_POP\n"
    "0061    6 OP_RETURN\n";

void testChunkContinue() {
  Chunk chunk;
  initChunk(&chunk);

  compile(
      "for (var i = 0; i < 10; i = i + 1) {\n"
      "  if (i == 2 or i == 5) \n"
      "    continue;\n"
      "  print(i); \n"
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
    {"testChunkLoops",     testChunkLoops,     TEST_CHUNK_LOOPS_EXPECT     },
    {"testChunkContinue",  testChunkContinue,  TEST_CHUNK_CONTINUE_EXPECT  },
};

const int CHUNK_TESTS_COUNT = sizeof(CHUNK_TESTS) / sizeof(TestCase);

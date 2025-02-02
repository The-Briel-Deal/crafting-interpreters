
#include "chunk.h"
#include "debug.h"

#include "helper_test.h"

const char TEST_WRITE_CHUNK_EXPECT[] = "== test chunk ==\n"
                                       "0000  123 OP_CONSTANT         0 '1.2'\n"
                                       "0002    | OP_RETURN\n";
void       testWriteChunk() {
  Chunk chunk;
  initChunk(&chunk);

  CONSTANT(1.2, 123);

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

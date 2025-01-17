#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chunk.h"
#include "debug.h"

static bool runTest(char *testName, void (*testCase)(), char *expect);

static void testWriteChunk();

static void testConstantLong();
static char TEST_CONSTANT_LONG_EXPECTATION[];

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  char testWriteChunkExpect[] = "== test chunk ==\n"
                                "0000  123 OP_CONSTANT         0 '1.2'\n"
                                "0002    | OP_RETURN\n";

  assert(runTest("testWriteChunk", testWriteChunk, testWriteChunkExpect));
  assert(runTest("testConstantLong", testConstantLong, TEST_CONSTANT_LONG_EXPECTATION));
  printf("Tests Succeeded!\n");
}

static void testWriteChunk() {
  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

static void testConstantLong() {
  Chunk chunk;
  initChunk(&chunk);

  // Just running this an arbitrary number of times over 256
  for (int i = 0; i < 260; i++) {
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
  }

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}
/***
 * Takes in a functionPtr test case and checks if the stdout output is equal to
 * the expect string.
 *
 * @param testName The name of the test being run.
 * @param testCase A functionPtr which should print output (ideally) equal to
 * the expect param.
 * @param expect A string which is expected to equal the output of the testCase.
 *
 * @return Whether or not the test passed or failed.
 */
static bool runTest(char *testName, void (*testCase)(), char *expect) {
  char buf[10 * 1024]; // 10KB Buffer
  FILE *membufstdout = fmemopen(buf, sizeof(buf), "w");

  FILE *prev_stdout = stdout;
  stdout = membufstdout;

  testCase();

  long end_pos = ftell(membufstdout);
  fclose(stdout);

  stdout = prev_stdout;

  buf[end_pos] = '\0';
  char *result = buf;

  if (strcmp(result, expect)) {
    printf("Test '%s' failed.\nExpect: '%s'\nResult: '%s'", testName, expect,
           result);

    return false;
  }
  return true;
}

static char TEST_CONSTANT_LONG_EXPECTATION[] =
    "== test chunk ==\n"
    "0000  123 OP_CONSTANT         0 '1.2'\n"
    "0002    | OP_CONSTANT         1 '1.2'\n"
    "0004    | OP_CONSTANT         2 '1.2'\n"
    "0006    | OP_CONSTANT         3 '1.2'\n"
    "0008    | OP_CONSTANT         4 '1.2'\n"
    "0010    | OP_CONSTANT         5 '1.2'\n"
    "0012    | OP_CONSTANT         6 '1.2'\n"
    "0014    | OP_CONSTANT         7 '1.2'\n"
    "0016    | OP_CONSTANT         8 '1.2'\n"
    "0018    | OP_CONSTANT         9 '1.2'\n"
    "0020    | OP_CONSTANT        10 '1.2'\n"
    "0022    | OP_CONSTANT        11 '1.2'\n"
    "0024    | OP_CONSTANT        12 '1.2'\n"
    "0026    | OP_CONSTANT        13 '1.2'\n"
    "0028    | OP_CONSTANT        14 '1.2'\n"
    "0030    | OP_CONSTANT        15 '1.2'\n"
    "0032    | OP_CONSTANT        16 '1.2'\n"
    "0034    | OP_CONSTANT        17 '1.2'\n"
    "0036    | OP_CONSTANT        18 '1.2'\n"
    "0038    | OP_CONSTANT        19 '1.2'\n"
    "0040    | OP_CONSTANT        20 '1.2'\n"
    "0042    | OP_CONSTANT        21 '1.2'\n"
    "0044    | OP_CONSTANT        22 '1.2'\n"
    "0046    | OP_CONSTANT        23 '1.2'\n"
    "0048    | OP_CONSTANT        24 '1.2'\n"
    "0050    | OP_CONSTANT        25 '1.2'\n"
    "0052    | OP_CONSTANT        26 '1.2'\n"
    "0054    | OP_CONSTANT        27 '1.2'\n"
    "0056    | OP_CONSTANT        28 '1.2'\n"
    "0058    | OP_CONSTANT        29 '1.2'\n"
    "0060    | OP_CONSTANT        30 '1.2'\n"
    "0062    | OP_CONSTANT        31 '1.2'\n"
    "0064    | OP_CONSTANT        32 '1.2'\n"
    "0066    | OP_CONSTANT        33 '1.2'\n"
    "0068    | OP_CONSTANT        34 '1.2'\n"
    "0070    | OP_CONSTANT        35 '1.2'\n"
    "0072    | OP_CONSTANT        36 '1.2'\n"
    "0074    | OP_CONSTANT        37 '1.2'\n"
    "0076    | OP_CONSTANT        38 '1.2'\n"
    "0078    | OP_CONSTANT        39 '1.2'\n"
    "0080    | OP_CONSTANT        40 '1.2'\n"
    "0082    | OP_CONSTANT        41 '1.2'\n"
    "0084    | OP_CONSTANT        42 '1.2'\n"
    "0086    | OP_CONSTANT        43 '1.2'\n"
    "0088    | OP_CONSTANT        44 '1.2'\n"
    "0090    | OP_CONSTANT        45 '1.2'\n"
    "0092    | OP_CONSTANT        46 '1.2'\n"
    "0094    | OP_CONSTANT        47 '1.2'\n"
    "0096    | OP_CONSTANT        48 '1.2'\n"
    "0098    | OP_CONSTANT        49 '1.2'\n"
    "0100    | OP_CONSTANT        50 '1.2'\n"
    "0102    | OP_CONSTANT        51 '1.2'\n"
    "0104    | OP_CONSTANT        52 '1.2'\n"
    "0106    | OP_CONSTANT        53 '1.2'\n"
    "0108    | OP_CONSTANT        54 '1.2'\n"
    "0110    | OP_CONSTANT        55 '1.2'\n"
    "0112    | OP_CONSTANT        56 '1.2'\n"
    "0114    | OP_CONSTANT        57 '1.2'\n"
    "0116    | OP_CONSTANT        58 '1.2'\n"
    "0118    | OP_CONSTANT        59 '1.2'\n"
    "0120    | OP_CONSTANT        60 '1.2'\n"
    "0122    | OP_CONSTANT        61 '1.2'\n"
    "0124    | OP_CONSTANT        62 '1.2'\n"
    "0126    | OP_CONSTANT        63 '1.2'\n"
    "0128    | OP_CONSTANT        64 '1.2'\n"
    "0130    | OP_CONSTANT        65 '1.2'\n"
    "0132    | OP_CONSTANT        66 '1.2'\n"
    "0134    | OP_CONSTANT        67 '1.2'\n"
    "0136    | OP_CONSTANT        68 '1.2'\n"
    "0138    | OP_CONSTANT        69 '1.2'\n"
    "0140    | OP_CONSTANT        70 '1.2'\n"
    "0142    | OP_CONSTANT        71 '1.2'\n"
    "0144    | OP_CONSTANT        72 '1.2'\n"
    "0146    | OP_CONSTANT        73 '1.2'\n"
    "0148    | OP_CONSTANT        74 '1.2'\n"
    "0150    | OP_CONSTANT        75 '1.2'\n"
    "0152    | OP_CONSTANT        76 '1.2'\n"
    "0154    | OP_CONSTANT        77 '1.2'\n"
    "0156    | OP_CONSTANT        78 '1.2'\n"
    "0158    | OP_CONSTANT        79 '1.2'\n"
    "0160    | OP_CONSTANT        80 '1.2'\n"
    "0162    | OP_CONSTANT        81 '1.2'\n"
    "0164    | OP_CONSTANT        82 '1.2'\n"
    "0166    | OP_CONSTANT        83 '1.2'\n"
    "0168    | OP_CONSTANT        84 '1.2'\n"
    "0170    | OP_CONSTANT        85 '1.2'\n"
    "0172    | OP_CONSTANT        86 '1.2'\n"
    "0174    | OP_CONSTANT        87 '1.2'\n"
    "0176    | OP_CONSTANT        88 '1.2'\n"
    "0178    | OP_CONSTANT        89 '1.2'\n"
    "0180    | OP_CONSTANT        90 '1.2'\n"
    "0182    | OP_CONSTANT        91 '1.2'\n"
    "0184    | OP_CONSTANT        92 '1.2'\n"
    "0186    | OP_CONSTANT        93 '1.2'\n"
    "0188    | OP_CONSTANT        94 '1.2'\n"
    "0190    | OP_CONSTANT        95 '1.2'\n"
    "0192    | OP_CONSTANT        96 '1.2'\n"
    "0194    | OP_CONSTANT        97 '1.2'\n"
    "0196    | OP_CONSTANT        98 '1.2'\n"
    "0198    | OP_CONSTANT        99 '1.2'\n"
    "0200    | OP_CONSTANT       100 '1.2'\n"
    "0202    | OP_CONSTANT       101 '1.2'\n"
    "0204    | OP_CONSTANT       102 '1.2'\n"
    "0206    | OP_CONSTANT       103 '1.2'\n"
    "0208    | OP_CONSTANT       104 '1.2'\n"
    "0210    | OP_CONSTANT       105 '1.2'\n"
    "0212    | OP_CONSTANT       106 '1.2'\n"
    "0214    | OP_CONSTANT       107 '1.2'\n"
    "0216    | OP_CONSTANT       108 '1.2'\n"
    "0218    | OP_CONSTANT       109 '1.2'\n"
    "0220    | OP_CONSTANT       110 '1.2'\n"
    "0222    | OP_CONSTANT       111 '1.2'\n"
    "0224    | OP_CONSTANT       112 '1.2'\n"
    "0226    | OP_CONSTANT       113 '1.2'\n"
    "0228    | OP_CONSTANT       114 '1.2'\n"
    "0230    | OP_CONSTANT       115 '1.2'\n"
    "0232    | OP_CONSTANT       116 '1.2'\n"
    "0234    | OP_CONSTANT       117 '1.2'\n"
    "0236    | OP_CONSTANT       118 '1.2'\n"
    "0238    | OP_CONSTANT       119 '1.2'\n"
    "0240    | OP_CONSTANT       120 '1.2'\n"
    "0242    | OP_CONSTANT       121 '1.2'\n"
    "0244    | OP_CONSTANT       122 '1.2'\n"
    "0246    | OP_CONSTANT       123 '1.2'\n"
    "0248    | OP_CONSTANT       124 '1.2'\n"
    "0250    | OP_CONSTANT       125 '1.2'\n"
    "0252    | OP_CONSTANT       126 '1.2'\n"
    "0254    | OP_CONSTANT       127 '1.2'\n"
    "0256    | OP_CONSTANT       128 '1.2'\n"
    "0258    | OP_CONSTANT       129 '1.2'\n"
    "0260    | OP_CONSTANT       130 '1.2'\n"
    "0262    | OP_CONSTANT       131 '1.2'\n"
    "0264    | OP_CONSTANT       132 '1.2'\n"
    "0266    | OP_CONSTANT       133 '1.2'\n"
    "0268    | OP_CONSTANT       134 '1.2'\n"
    "0270    | OP_CONSTANT       135 '1.2'\n"
    "0272    | OP_CONSTANT       136 '1.2'\n"
    "0274    | OP_CONSTANT       137 '1.2'\n"
    "0276    | OP_CONSTANT       138 '1.2'\n"
    "0278    | OP_CONSTANT       139 '1.2'\n"
    "0280    | OP_CONSTANT       140 '1.2'\n"
    "0282    | OP_CONSTANT       141 '1.2'\n"
    "0284    | OP_CONSTANT       142 '1.2'\n"
    "0286    | OP_CONSTANT       143 '1.2'\n"
    "0288    | OP_CONSTANT       144 '1.2'\n"
    "0290    | OP_CONSTANT       145 '1.2'\n"
    "0292    | OP_CONSTANT       146 '1.2'\n"
    "0294    | OP_CONSTANT       147 '1.2'\n"
    "0296    | OP_CONSTANT       148 '1.2'\n"
    "0298    | OP_CONSTANT       149 '1.2'\n"
    "0300    | OP_CONSTANT       150 '1.2'\n"
    "0302    | OP_CONSTANT       151 '1.2'\n"
    "0304    | OP_CONSTANT       152 '1.2'\n"
    "0306    | OP_CONSTANT       153 '1.2'\n"
    "0308    | OP_CONSTANT       154 '1.2'\n"
    "0310    | OP_CONSTANT       155 '1.2'\n"
    "0312    | OP_CONSTANT       156 '1.2'\n"
    "0314    | OP_CONSTANT       157 '1.2'\n"
    "0316    | OP_CONSTANT       158 '1.2'\n"
    "0318    | OP_CONSTANT       159 '1.2'\n"
    "0320    | OP_CONSTANT       160 '1.2'\n"
    "0322    | OP_CONSTANT       161 '1.2'\n"
    "0324    | OP_CONSTANT       162 '1.2'\n"
    "0326    | OP_CONSTANT       163 '1.2'\n"
    "0328    | OP_CONSTANT       164 '1.2'\n"
    "0330    | OP_CONSTANT       165 '1.2'\n"
    "0332    | OP_CONSTANT       166 '1.2'\n"
    "0334    | OP_CONSTANT       167 '1.2'\n"
    "0336    | OP_CONSTANT       168 '1.2'\n"
    "0338    | OP_CONSTANT       169 '1.2'\n"
    "0340    | OP_CONSTANT       170 '1.2'\n"
    "0342    | OP_CONSTANT       171 '1.2'\n"
    "0344    | OP_CONSTANT       172 '1.2'\n"
    "0346    | OP_CONSTANT       173 '1.2'\n"
    "0348    | OP_CONSTANT       174 '1.2'\n"
    "0350    | OP_CONSTANT       175 '1.2'\n"
    "0352    | OP_CONSTANT       176 '1.2'\n"
    "0354    | OP_CONSTANT       177 '1.2'\n"
    "0356    | OP_CONSTANT       178 '1.2'\n"
    "0358    | OP_CONSTANT       179 '1.2'\n"
    "0360    | OP_CONSTANT       180 '1.2'\n"
    "0362    | OP_CONSTANT       181 '1.2'\n"
    "0364    | OP_CONSTANT       182 '1.2'\n"
    "0366    | OP_CONSTANT       183 '1.2'\n"
    "0368    | OP_CONSTANT       184 '1.2'\n"
    "0370    | OP_CONSTANT       185 '1.2'\n"
    "0372    | OP_CONSTANT       186 '1.2'\n"
    "0374    | OP_CONSTANT       187 '1.2'\n"
    "0376    | OP_CONSTANT       188 '1.2'\n"
    "0378    | OP_CONSTANT       189 '1.2'\n"
    "0380    | OP_CONSTANT       190 '1.2'\n"
    "0382    | OP_CONSTANT       191 '1.2'\n"
    "0384    | OP_CONSTANT       192 '1.2'\n"
    "0386    | OP_CONSTANT       193 '1.2'\n"
    "0388    | OP_CONSTANT       194 '1.2'\n"
    "0390    | OP_CONSTANT       195 '1.2'\n"
    "0392    | OP_CONSTANT       196 '1.2'\n"
    "0394    | OP_CONSTANT       197 '1.2'\n"
    "0396    | OP_CONSTANT       198 '1.2'\n"
    "0398    | OP_CONSTANT       199 '1.2'\n"
    "0400    | OP_CONSTANT       200 '1.2'\n"
    "0402    | OP_CONSTANT       201 '1.2'\n"
    "0404    | OP_CONSTANT       202 '1.2'\n"
    "0406    | OP_CONSTANT       203 '1.2'\n"
    "0408    | OP_CONSTANT       204 '1.2'\n"
    "0410    | OP_CONSTANT       205 '1.2'\n"
    "0412    | OP_CONSTANT       206 '1.2'\n"
    "0414    | OP_CONSTANT       207 '1.2'\n"
    "0416    | OP_CONSTANT       208 '1.2'\n"
    "0418    | OP_CONSTANT       209 '1.2'\n"
    "0420    | OP_CONSTANT       210 '1.2'\n"
    "0422    | OP_CONSTANT       211 '1.2'\n"
    "0424    | OP_CONSTANT       212 '1.2'\n"
    "0426    | OP_CONSTANT       213 '1.2'\n"
    "0428    | OP_CONSTANT       214 '1.2'\n"
    "0430    | OP_CONSTANT       215 '1.2'\n"
    "0432    | OP_CONSTANT       216 '1.2'\n"
    "0434    | OP_CONSTANT       217 '1.2'\n"
    "0436    | OP_CONSTANT       218 '1.2'\n"
    "0438    | OP_CONSTANT       219 '1.2'\n"
    "0440    | OP_CONSTANT       220 '1.2'\n"
    "0442    | OP_CONSTANT       221 '1.2'\n"
    "0444    | OP_CONSTANT       222 '1.2'\n"
    "0446    | OP_CONSTANT       223 '1.2'\n"
    "0448    | OP_CONSTANT       224 '1.2'\n"
    "0450    | OP_CONSTANT       225 '1.2'\n"
    "0452    | OP_CONSTANT       226 '1.2'\n"
    "0454    | OP_CONSTANT       227 '1.2'\n"
    "0456    | OP_CONSTANT       228 '1.2'\n"
    "0458    | OP_CONSTANT       229 '1.2'\n"
    "0460    | OP_CONSTANT       230 '1.2'\n"
    "0462    | OP_CONSTANT       231 '1.2'\n"
    "0464    | OP_CONSTANT       232 '1.2'\n"
    "0466    | OP_CONSTANT       233 '1.2'\n"
    "0468    | OP_CONSTANT       234 '1.2'\n"
    "0470    | OP_CONSTANT       235 '1.2'\n"
    "0472    | OP_CONSTANT       236 '1.2'\n"
    "0474    | OP_CONSTANT       237 '1.2'\n"
    "0476    | OP_CONSTANT       238 '1.2'\n"
    "0478    | OP_CONSTANT       239 '1.2'\n"
    "0480    | OP_CONSTANT       240 '1.2'\n"
    "0482    | OP_CONSTANT       241 '1.2'\n"
    "0484    | OP_CONSTANT       242 '1.2'\n"
    "0486    | OP_CONSTANT       243 '1.2'\n"
    "0488    | OP_CONSTANT       244 '1.2'\n"
    "0490    | OP_CONSTANT       245 '1.2'\n"
    "0492    | OP_CONSTANT       246 '1.2'\n"
    "0494    | OP_CONSTANT       247 '1.2'\n"
    "0496    | OP_CONSTANT       248 '1.2'\n"
    "0498    | OP_CONSTANT       249 '1.2'\n"
    "0500    | OP_CONSTANT       250 '1.2'\n"
    "0502    | OP_CONSTANT       251 '1.2'\n"
    "0504    | OP_CONSTANT       252 '1.2'\n"
    "0506    | OP_CONSTANT       253 '1.2'\n"
    "0508    | OP_CONSTANT       254 '1.2'\n"
    "0510    | OP_CONSTANT       255 '1.2'\n"
    "0512    | OP_CONSTANT       256 '1.2'\n"
    "0514    | OP_CONSTANT       257 '1.2'\n"
    "0516    | OP_CONSTANT       257 '1.2'\n"
    "0518    | OP_CONSTANT       258 '1.2'\n"
    "0520    | OP_RETURN\n";

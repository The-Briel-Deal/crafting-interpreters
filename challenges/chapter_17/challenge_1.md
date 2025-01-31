(-1 + 2) * 3 - -4

1.  `(` | expression()
2.  `(` |  parsePrecedence(PREC_ASSIGNMENT)
3.  `-` |   grouping()
4.  `-` |    expression()
5.  `-` |     parsePrecedence(PREC_ASSIGNMENT) 
6.  `1` |      unary()
7.  `1` |       parsePrecedence(PREC_UNARY)
8.  `+` |        number()
9.  `+` |         emitConstant(1)
10. `2` |      binary() 
11. `2` |       parsePrecedence(PREC_FACTOR)
12. `)` |        number()
13. `)` |         emitConstant(2)
14. `)` |       emitByte(OP_ADD)



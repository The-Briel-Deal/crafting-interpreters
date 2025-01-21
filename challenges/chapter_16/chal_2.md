# Chal 2 - Bitshift or Generic?
Java and C# both resolve this in the parser, though in different ways. In the
parser when they have unclosed '<' symbols, they will check for the BITSHIFT
operator and the normal angle bracket TOKEN. If C# runs into a BITSHIFT token,
then it splits it into two angle brackets. Java's a bit different, where it
lexes all >> as two angle bracket tokens, then the parser determines if it
should be a bitshift, once the generic brackets are closed.

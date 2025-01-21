# Chal 1 - String Interpolation
I would need to add a $ token at the very least. Then in the parser I would
need to check that its succeeded with '{'.

When scanning the following exression:
```
"Nested ${"interpolation?! Are you ${"mad?!"}"}"
```
It would be quite difficult to scan this as anything but a string literal. If I
did I would need to add another check for '${', and then I would need to
terminate the string to scan for a new token until }.  So I would probably need
a token for STRING_START, STRING_PIECE, STRING_END, in addition to DOLLAR_SIGN.

The alternative is keeping this as a second string literal type like
FORMAT_STRING (but ignoring quotes inside ${}). And then handling this in the
parser where a tree structure makes a bit more sense.


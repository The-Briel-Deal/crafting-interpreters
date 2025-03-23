# Chal 2

There is one big difficulty with storing small strings inline in Value. The
issue is that nearly all functions in clox that work with strings expect
ObjStrings. ObjStrings contain the strings hash, length, and start character.
Since the strings are always less than 8 characters, and they are null
terminated, I can get away with just using strlen(). I can also do the same
with getting the hash on the fly since string's are always short.

The tough thing is that I don't see any way of getting around the fact that I
have to reimplement all of the table functions to work with raw strings, or
convert the short string value to an obj value whenever I need to use it with a
table.

My current approach is going to be changing all functions that take ObjStrings
to take Values.


# Chal 1

Negate and Not could be combined to be context specific to Not when used on a
bool, and to Negate when used with a number. That would remove the ability to
Not a number though.

Negate and Subtract would be an easier combination, since Negating, then adding
is the same as subtracting.

Down the road we could also eliminate either GREATER or LESS, since we could
Just and together a Greater/Less and Equal then optionally Not.

We could also get rid of either TRUE or FALSE easily, since either of those can
be represented with an additional NOT.

You could also get rid of NIL, TRUE, and FALSE by just extending OP_CONSTANT to
let you specify a type, or reserve a 2 bits for type info.

Finally there is the extreme of One instruction which you can do in a few ways
it appears. This of course comes with the tradeoff of speed I would think,
since you wouldn't use the modern hardware in the way its native instruction
set is meant to.

# Chal 2

A GREATER_EQUAL and LESS_EQUAL would make it so that those operations would
only require one instruction.

A NEGATIVE_CONSTANT would also be useful so we don't have to do CONSTANT then
NEGATE for all negative constants.


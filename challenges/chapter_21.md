# Chal 2

All that really comes to mind is adding a resolver pass or giving all constants
an integer ID in the scanner. But that still requires lookup. I think it
honestly may be faster to just use an array or a stack though since the max
size is so small.

After looking at the answer I found that the answer is moving variable maping
to the VM. We're going to have a table mapping the variables name to the index
in an array list. Variables are going to start off as being a new sentinal
value of undefined if they are declared without an initial value.

None of the variable setting should be done in the compiler. The compiler
should just push an OP_VAR_SET instruction to the chunk with the item index
directly below it, and the Value to set right below that.

Currently for setting, the compiler stores the identifier token then runs the expression,
once the expression is done running the resulting value is left on the stack. 
Then two bytes are emitted, OP_SET_GLOBAL and the identifier.

So after compiling a set expression the chunk currently looks like this:
stmt: `a = 1 + 2;`
chunk:

-> 'a'
OP_SET_GLOBAL
OP_ADD
-> 2
CONST
-> 1
CONST

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

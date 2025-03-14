# Chal 1
Done on branch `chap_28_chal_1`

# Chal 2
My first thought on how you could speed up calls to methods on a class is by
giving every call to a method on a class an index in the class itself. Then you
can store pointers to the methods in an array in the class itself. With that
you directly emit the method index in the bytecode to call it without a hash
lookup.

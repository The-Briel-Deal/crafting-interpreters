# Challenge 1

It's safe to eagerly define a variable bound to a function's name because we
expect to be able to reference a function inside of itself. With variables
however, we want self reference assignments to throw an error to point the user
in the right direction of what is wrong with their code. This is because
usually, shadowing a variable with the same name is a mistake which may cause
unexpected behavior.

# Challenge 2

Rust does allow this and I actually find myself frequently using this because
of the type system. Especially with the result type, frequently when a function
call returns a result, i'll use the same name in the result var, and the inner
value var after unwrapping. I don't believe Python allows this.

I just tested if Javascript allows this and it does. I know Java does not allow
this.

# Challenge 3

On branch `chap_11_chal_3`

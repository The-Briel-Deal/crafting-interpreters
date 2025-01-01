# Chal 1

I'm going to guess that smalltalk implementations don't have that problem
because it just drops additional arguments or fills missing args with null or
undefined.

Another possibility could be it doing this check at compile time. This isn't
always possible if types aren't statically checked at compile time since the
same variable could hold multiple functions with different amounts of args in
dynamic langs.

Answer:
Smalltalks syntax uses keyword arguments, this means its not possible to
provide too many since I assume non matching arg names are just dropped.

```smalltalk
list insert: "element" at: 2
```
# Chal 2

On branch `chap_10_chal_2`

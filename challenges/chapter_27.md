# Chal 1
Javascript returns undefined when you access an unset field. I'm going to
implement this in Lox with a new sentinal value.

# Chal 2
Since we don't have methods yet, I'm going to define a native function that
gets the value of a field when passed an instance, and a string.

For example:
```js
getField(instance, "field");
```

# Chal 3
Python uses a delattr function. I'm going to implement something similar like I did for chal 1 and 2.

For example:
```js
delField(instance, "field")
```

# Chal 4
One way I can think of speeding up field access is via caching, you could store
a cache of the last 3 accessed ObjString pointers in a circular buffer where
you attempt to look through the list before hashing, then you can go directly
there if the cache is still valid. I'm not sure what all the cache invalidation
scenarios look like though, I feel like there is probably an edge case where we
access bad data if the table has resized. We could always invalidate the cache
in that situation however.  

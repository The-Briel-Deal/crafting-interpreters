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

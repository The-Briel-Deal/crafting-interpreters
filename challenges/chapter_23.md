# Chal 2

My current plan is to track loops in the compiler with an array of integers.

```js
// Scope 0
for (var a = 1; a < 10; a++) { // Scope 1
  { // Scope 2
    var b = 2;
    { // Scope 3
      var c = 3; 
      while (true) { // Scope 4
        continue; // <-  [0, 3] 
        // To continue here, we know that the last loop opened at depth 3, so we need to pop the stack till depth 3 to continue.
      }
      continue; // <- [0]
      // To continue here, we know that we have to pop from stack till we hit depth 0.
    }
  }
}

```

Actual solution on branch `chap_23_chal_2`.

# Chal 1
On branch `chap_26_chal_1`

# Chal 2
On branch `chap_26_chal_2`

# Chal 3
Plan is as follows, implementation will be on `chap_26_chal_3`.

I am going to implement Cheney's Algorithm.

The first step is to allocate a heap that is double the size of what is needed.
Then to divide it into two, only one half will be needed at a time (besides
during collection).

```
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
```

> NOTE:
>
> Nothing Points to `d`.
```
 a -> fc b -> fa c -> fb d -> fb
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```


Copy first marked node to the `to` section.
```
 *prev
 *curr
 a -> fc b -> fa c -> fb d -> fb a -> fc 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Replace original node with a pointer to the new node.
```
  *prev                          *curr
 -> ta   b -> fa c -> fb d -> fb a -> fc 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Move onto next `fc` at byte 2.
```
                 *curr           *prev
 -> ta   b -> fa -> tc   d -> fb a -> fc c -> fb 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Point prev to location at curr. We want to make sure the last node copied to `to` points to it's new location.
```
                 *curr           *prev
 -> ta   b -> fa -> tc   d -> fb a -> tc c -> fb 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Move onto next at `tc` at byte 5.
```
                 *prev                  *curr
 -> ta   b -> fa -> tc   d -> fb a -> tc c -> fb 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Copy `b` node in the from section to next open field in the `to` section at byte 6.
```
                 *prev                  *curr
 -> ta   -> ta   -> tc  |d -> fb|a -> tc|c -> fb|b -> fa 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Point node `tc` in byte 5 to `tb` at byte 6.
```
         *curr                           *prev
 -> ta   -> ta   -> tc  |d -> fb|a -> tc|c -> tb|b -> fa 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

Finally, point `tb` at byte 6 to `ta` since `fa` was already moved and replaced by a pointer.
```
         *prev                                   *curr
 -> ta   -> ta   -> tc  |d -> fb|a -> tc|c -> tb|b -> ta 
|0......|1......|2......|3......|4......|5......|6......|7......|
| <- start                      | <- middle                     | <- end
     from                            to
```

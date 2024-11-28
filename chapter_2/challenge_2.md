# Why not always use JIT for dynamic languages.
## My guesses
* There is an initial cost at the beginning of a run, and it takes time to
warmup after invocation.
* If the same code isn't repeatedly called than its pointless to spend time
compiling to optimize future runs, especially for things like a shell
interpreter where running the same code over and over in a session is rare
outside of loops.
* You can achieve similar levels of portability with something like the JVM,
where you compile to bytecode ahead of time. But you then don't have the
initial overhead or memory use.
* You will need to load the code necessary to compile, in addition to the code
you write.

## Answer from book Answers Section
1. It's really complex to implement, debug, and maintain. Few people have the skill to do it.
2. Like a native code compiler (which it is), it ties you to a specific CPU architecture.
3. Bytecode is generally more compact than machine code (since it's closer to the semantics of the language), so it takes up less memory. In platforms like embedded devices where memory may matter more than speed, that can be a worthwhile trade-off.
4. Some platforms, like iOS and most game consoles, expressly disallow executing code generated at runtime. The OS simply won't allow you to jump into memory that can be written to.


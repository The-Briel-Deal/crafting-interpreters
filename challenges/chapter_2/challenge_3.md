# Why do list compilers that compile to C also contain lisp interpreters.
## My Answer
* To be able to run most code at high performance, while also having the
ability to do some things dynamically without having to rebuild and relaunch
the process.
* It also may even be more performant in some cases for things like some
generics/templates. For example, if you have a rarely called logging function
that needs to take any type to log it out, in lisp, you can have only 1
definition in memory, rather than having to generate an implementation for each
type that may be used.

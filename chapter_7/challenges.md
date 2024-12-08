1. I would honestly prefer a runtime error rather than implicit casting of
   anything. It just makes it so much harder to find issues where they occur.
   Because more often than not an implicit cast causes issues down stream from the cast.
2. on branch chap\_7\_chal\_2

3. It returns a NaN when 0/0 and infinity when 1/0. I'm thinking this is javas
   behavior? I would prefer it throw a runtime error. I know this is what python does. I believe rust panics, and in C and C++ it produced implementation defined behavior.

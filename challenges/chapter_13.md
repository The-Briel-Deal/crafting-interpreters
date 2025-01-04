1. I think the one thing I would add are traits similar to rust. If not that I
   would add interfaces that are type checked before runtime. The one thing I
   like about interfaces and traits is that you can seperate concerns and
   without having to worry about what else the parents inherit. I also like
   that you can have code that works on one interface which can have multiple
   hidden implementations. It also encourages you to think hard about how you want
   users to interact with you interface.

   I'm going to add traits on branch `chap_13_chal_1_traits`.

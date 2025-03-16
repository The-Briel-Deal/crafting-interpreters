# Chal 1
Ideally I would statically check this, that would be much easier if fields
weren't set dynamically and every class had a set list of fields. In some
languages like python they don't automatically call the initializer of a
superclass. Instead they make it so you have to call `super().__init__()`. That
put's the responsibility more explicitly on the user to make sure you don't
step on the superclasses toes. This is similar to what lox is doing right now.
I don't necessarily have a problem with this in a scripting/toy language. But I
would definitely want this to be checked in a statically typed language.

# Chal 3 Plan
Currently we use copying down to speed up the this keyword. If we want to
implement BETA style inheritance we'll have to figure out a way to start at the
top, this may involving droping the copy down optimization.

After looking into it, it looks like the copy down optimization shouldn't break
an inner keyword since it never overwrites the classes main method.

What i'm going to have to do here for my first naive approach, is climb to the
top inherited class while grabbing a reference to each method with that name.
Then each bound method will store a reference to the method on the next
subclass.

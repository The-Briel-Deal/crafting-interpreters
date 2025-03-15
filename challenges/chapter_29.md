# Chal 1
Ideally I would statically check this, that would be much easier if fields
weren't set dynamically and every class had a set list of fields. In some
languages like python they don't automatically call the initializer of a
superclass. Instead they make it so you have to call `super().__init__()`. That
put's the responsibility more explicitly on the user to make sure you don't
step on the superclasses toes. This is similar to what lox is doing right now.
I don't necessarily have a problem with this in a scripting/toy language. But I
would definitely want this to be checked in a statically typed language.

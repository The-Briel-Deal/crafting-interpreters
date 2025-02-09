# Chal 2

I personally like how rust does shadowing, `var a = a;` doesn't sound super
useful. But with result vals for example I often use `var a = a.unwrap();`.
That said in a scripting language without enforcement for strict types I don't
think it really matters much. I can assign a different type to the same
variable so it isn't super consequential if you can't shadow with a reference
to a variable with the same name.


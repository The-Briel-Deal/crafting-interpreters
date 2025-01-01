1. On branch `chap_12_chal_1`
2. On branch `chap_12_chal_2`
3. I'm mostly in favor of getters/setters/encapsulation of state. I've had
   experiences in the past where i've made the mistake of mutating state
   directly in many places and it becomes rediculously hard to debug. That said,
   there is overhead using getters/setters instead of mutating directly. In
   scripting languages I don't care as much as long as its not a massive
   application, its definitely more convenient to just mutate out of the class,
   and it can be faster than making a new stack frame and copying the pointer or
   val out in the return. I also am a big fan of private and public modifiers,
   especially when private by default.

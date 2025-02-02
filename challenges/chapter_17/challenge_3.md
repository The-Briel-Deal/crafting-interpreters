# Misfix Expressions

Actual solution on chap_17_chal_3 branch.

Adding support to the tokenizer should be pretty easy, we just need to add the ?
and : token.

A ternary statement in an ast would be parsed like

a == b ? "boogie" : "woogie"

tern:
  if: 
    equality:
      left: a
      right: b
  then: "boogie"
  else: "woogie"
 

What would the precedence be? It should probably be just above assignment.

#!/bin/sh

mvn compile

RESULT="$(java -cp ./target/classes/ org.craftinginterpreters.lox.Lox ./scripts/inner.lox)"
EXPECT="Fry until golden brown.
Pipe full of custard and coat with chocolate.
Place in a nice box.
"

if [[ "$EXPECT" == "$RESULT" ]]; then
	echo "PASS!"
else
	echo "FAILED"
	echo ""
	echo "Expected:"
	echo ">$EXPECT<"
	echo ""
	echo "Result:"
	echo ">$RESULT<"
fi

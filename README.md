Regular expression to DFA
=========================
Implementation of the algorithm `Converting a Regular Expression Directly to a DFA` from the book `Compilers: Principles, Techniques, and Tools`.

The program takes as input a regular expression and generates as output a DFA.

Example:
```
./regex_to_dfa "(a|b)*abb"
Number of states: 4.

State (1, 2, 3)
  a -> (1, 2, 3, 4)
  b -> (1, 2, 3)

State (1, 2, 3, 4)
  a -> (1, 2, 3, 4)
  b -> (1, 2, 3, 5)

State (1, 2, 3, 5)
  a -> (1, 2, 3, 4)
  b -> (1, 2, 3, 6)

State (1, 2, 3, 6) (accepting state)
  a -> (1, 2, 3, 4)
  b -> (1, 2, 3)

```


Regular expressions to recognize a number (either an integer or a float):
```
[0-9]+                                e.g.: 123
[0-9]+[Ee][+-]?[0-9]+                 e.g.: 123e+10
[0-9]*"."[0-9]+([Ee][+-]?[0-9]+)?     e.g.: .123e-12
[0-9]+"."[0-9]*([Ee][+-]?[0-9]+)?     e.g.: 123.e12
```

It can be tested with:
```
./regex_to_dfa "([0-9]+)|([0-9]+[Ee][+-]?[0-9]+)|([0-9]*\.[0-9]+([Ee][+-]?[0-9]+)?)|([0-9]+\.[0-9]*([Ee][+-]?[0-9]+)?)"
```


The following patterns can be used:

- Any character (`.`).
- Character class (e.g.: `[a-z]`).
- Negated character class (e.g.: `[^a-z]`).
- Repetition zero or more (`*`).
- Repetition one or more (`+`).
- Optional (`?`).
- Or (`|`).
- Escape character (e.g.: `\n`).

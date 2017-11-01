#include <stdlib.h>
#include <stdio.h>
#include "lex/dfa.h"

int main(int argc, const char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <regular-expression>\n", argv[0]);
    return -1;
  }

  // Parse regular expression and build syntax tree.
  lex::regular_expression regex;
  if (regex.parse(argv[1])) {
    // Build DFA.
    lex::dfa dfa;
    if (dfa.build(regex)) {
      dfa.print();
      return 0;
    } else {
      fprintf(stderr, "Error building DFA.\n");
    }
  } else {
    fprintf(stderr, "Error parsing regular expression.\n");
  }

  return -1;
}

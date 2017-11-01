#include <stdint.h>
#include <string.h>
#include <memory>
#include "lex/regular_expression.h"
#include "macros/macros.h"

bool lex::regular_expression::parse(const char* regex)
{
  nodes nodes;

  bool chars[256];
  bool negated_char_class = false;

  int state = 0; // Initial state.

  uint8_t c;
  uint8_t prevc = 0;
  while ((c = *regex) != 0) {
    switch (state) {
      case 0: // Initial state.
        switch (c) {
          case '\\':
            if ((c = escape_character(regex[1])) != 0) {
              regex++;
            } else {
              // Invalid escape character.
              return false;
            }

            // Fall through.
          default:
            node* n;
            if ((add(c, _M_npositions)) &&
                ((n = new (std::nothrow) node()) != nullptr)) {
              n->t = node::type::symbol;

              n->s = c;
              n->pos = _M_npositions++;

              // Add node.
              if (!add(nodes, n, regex)) {
                // Node 'n' has been already deleted.
                return false;
              }
            } else {
              return false;
            }

            break;
          case '.':
            {
              for (size_t i = 0; i < ARRAY_SIZE(chars); i++) {
                chars[i] = ((i != '\n') && (i != '\r'));
              }

              node* n;
              if (((n = create_char_class_subtree(chars, 128)) == nullptr) ||
                  (!add(nodes, n, regex))) {
                return false;
              }
            }

            break;
          case '[':
            state = 1; // Character class.
            break;
          case '*':
          case '+':
          case '?':
            return false;
          case '(':
            // Push an empty node.
            if (!nodes.push()) {
              return false;
            }

            break;
          case ')':
            if (nodes.size() > 1) {
              node* n;
              if ((n = nodes.top()) != nullptr) {
                nodes.pop();

                if (!nodes.top()) {
                  nodes.pop();

                  // Add node.
                  if (!add(nodes, n, regex)) {
                    // Node 'n' has been already deleted.
                    return false;
                  }
                } else {
                  delete n;
                  return false;
                }
              } else {
                return false;
              }
            } else {
              return false;
            }

            break;
          case '|':
            if (nodes.top()) {
              node* n;
              if ((n = new (std::nothrow) node()) != nullptr) {
                n->t = node::type::alternation;

                n->left = nodes.top();

                nodes.pop();
                nodes.push(n);
              } else {
                return false;
              }
            } else {
              return false;
            }

            break;
        }

        break;
      case 1: // Character class.
        switch (c) {
          case '\\':
            if ((c = escape_character(regex[1])) != 0) {
              regex++;
            } else {
              // Invalid escape character.
              return false;
            }

            // Fall through.
          default:
            for (size_t i = 0; i < ARRAY_SIZE(chars); i++) {
              chars[i] = false;
            }

            chars[c] = true;

            // Save previous character.
            prevc = c;

            state = 2; // Parsing character class.
            break;
          case '^':
            for (size_t i = 0; i < ARRAY_SIZE(chars); i++) {
              chars[i] = true;
            }

            negated_char_class = true;

            state = 4; // Negated character class.
            break;
          case ']':
            return false;
        }

        break;
      case 2: // Parsing character class.
        switch (c) {
          case '\\':
            if ((c = escape_character(regex[1])) != 0) {
              regex++;
            } else {
              // Invalid escape character.
              return false;
            }

            // Fall through.
          default:
            chars[c] = !negated_char_class;

            // Save previous character.
            prevc = c;

            break;
          case '-':
            state = 3; // Range.
            break;
          case ']':
            {
              node* n;
              if (((n = create_char_class_subtree(chars, 128)) == nullptr) ||
                  (!add(nodes, n, regex))) {
                return false;
              }

              state = 0; // Initial state.
            }

            break;
        }

        break;
      case 3: // Range.
        switch (c) {
          case ']':
            {
              chars['-'] = !negated_char_class;

              node* n;
              if (((n = create_char_class_subtree(chars, 128)) == nullptr) ||
                  (!add(nodes, n, regex))) {
                return false;
              }

              state = 0; // Initial state.
            }

            break;
          case '\\':
            if ((c = escape_character(regex[1])) != 0) {
              regex++;
            } else {
              // Invalid escape character.
              return false;
            }

            // Fall through.
          default:
            if (c >= prevc) {
              if (prevc != 0) {
                for (size_t i = prevc; i <= c; i++) {
                  chars[i] = !negated_char_class;
                }

                // Reset previous character.
                prevc = 0;

                state = 2; // Parsing character class.
              } else {
                // Unrecognized rule.
                return false;
              }
            } else {
              // Negative range in character class.
              return false;
            }
        }

        break;
      case 4: // Negated character class.
        switch (c) {
          case '\\':
            if ((c = escape_character(regex[1])) != 0) {
              regex++;
            } else {
              // Invalid escape character.
              return false;
            }

            // Fall through.
          default:
            chars[c] = false;

            // Save previous character.
            prevc = c;

            state = 2; // Parsing character class.
            break;
          case ']':
            return false;
        }

        break;
    }

    regex++;
  }

  if ((state == 0) &&
      (nodes.size() == 1) &&
      (nodes.top()) &&
      ((nodes.top()->t != node::type::alternation) || (nodes.top()->right))) {
    node* endmark;
    if ((endmark = new (std::nothrow) node()) != nullptr) {
      node* concatenation;
      if ((concatenation = new (std::nothrow) node()) != nullptr) {
        endmark->t = node::type::endmark;
        endmark->pos = _M_npositions++;

        concatenation->t = node::type::concatenation;

        concatenation->left = nodes.top();
        concatenation->right = endmark;

        _M_root = concatenation;

        nodes.pop();

        // Compute nullable, firstpos and lastpos.
        return _M_root->init();
      } else {
        delete endmark;
      }
    }
  }

  return false;
}

lex::node* lex::regular_expression::create_char_class_subtree(const bool* chars,
                                                              size_t size)
{
  node* root = nullptr;

  for (size_t i = 0; i < size; i++) {
    if (chars[i]) {
      node* n;
      if ((add(i, _M_npositions)) &&
          ((n = new (std::nothrow) node()) != nullptr)) {
        n->t = node::type::symbol;

        n->s = i;
        n->pos = _M_npositions++;

        if (root) {
          node* alternation;
          if ((alternation = new (std::nothrow) node()) != nullptr) {
            alternation->t = node::type::alternation;

            alternation->left = root;
            alternation->right = n;

            root = alternation;
          } else {
            delete n;
            delete root;

            return nullptr;
          }
        } else {
          root = n;
        }
      } else {
        if (root) {
          delete root;
        }

        return nullptr;
      }
    }
  }

  return root;
}

bool lex::regular_expression::add(symbol s, position p)
{
  for (size_t i = 0; i < _M_nsymbols; i++) {
    if (_M_symbols[i].s == s) {
      return _M_symbols[i].p.add(p);
    }
  }

  if (_M_nsymbols < max_symbols) {
    _M_symbols[_M_nsymbols].s = s;

    if (_M_symbols[_M_nsymbols].p.add(p)) {
      _M_nsymbols++;

      return true;
    }
  }

  return false;
}

bool lex::regular_expression::add(nodes& nodes, node* n, const char*& regex)
{
  switch (regex[1]) {
    case '*':
    case '+':
    case '?':
      {
        node* repetition;
        if ((repetition = new (std::nothrow) node()) != nullptr) {
          switch (regex[1]) {
            case '*':
              repetition->t = node::type::repetition_zero_or_more;
              break;
            case '+':
              repetition->t = node::type::repetition_one_or_more;
              break;
            case '?':
              repetition->t = node::type::optional;
              break;
          }

          repetition->left = n;

          n = repetition;

          regex++;
        } else {
          delete n;
          return false;
        }
      }

      break;
  }

  if (!nodes.top()) {
    if (!nodes.push(n)) {
      delete n;
      return false;
    }
  } else if ((nodes.top()->t != node::type::alternation) ||
             (nodes.top()->right)) {
    node* concatenation;
    if ((concatenation = new (std::nothrow) node()) != nullptr) {
      concatenation->t = node::type::concatenation;

      concatenation->left = nodes.top();
      concatenation->right = n;

      nodes.pop();
      nodes.push(concatenation);
    } else {
      delete n;
      return false;
    }
  } else {
    nodes.top()->right = n;
  }

  return true;
}

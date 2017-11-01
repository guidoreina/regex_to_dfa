#include <memory>
#include "lex/dfa.h"

bool lex::dfa::build(const regular_expression& regex)
{
  // Save number of positions.
  _M_npositions = regex.number_positions();

  if ((_M_followpos = new (std::nothrow) positions[_M_npositions]) != nullptr) {
    // Compute followpos for T.
    if (compute_followpos(regex.root())) {
      // Construct Dstates, the set of states of DFA D, and Dtran, the
      // transition function for D.
      return construct_transition_function(regex);
    }
  }

  return false;
}

bool lex::dfa::compute_followpos(const node* n)
{
  // If not a leaf node...
  if (!n->leaf()) {
    if (((!n->left) || (compute_followpos(n->left))) &&
        ((!n->right) || (compute_followpos(n->right)))) {
      switch (n->t) {
        case node::type::concatenation:
          // If n is a cat-node with left child c1 and right child c2, then for
          // every position i in lastpos(c1), all positions in firspos(c2) are
          // in followpos(i).
          for (size_t i = 0; i < n->left->lastpos.size(); i++) {
            if (!_M_followpos[n->left->lastpos.get(i)]
                .add(n->right->firstpos)) {
              return false;
            }
          }

          break;
        case node::type::repetition_zero_or_more:
        case node::type::repetition_one_or_more:
          // If n is a star-node, and i is a position in lastpos(n), then all
          // positions in firstpos(n) are in followpos(i).
          for (size_t i = 0; i < n->left->lastpos.size(); i++) {
            if (!_M_followpos[n->left->lastpos.get(i)].add(n->left->firstpos)) {
              return false;
            }
          }

          break;
        case node::type::alternation:
        case node::type::optional:
        case node::type::symbol:
        case node::type::endmark:
          break;
      }
    } else {
      return false;
    }
  }

  return true;
}

bool lex::dfa::construct_transition_function(const regular_expression& regex)
{
  // Initialize Dstates to contain only the unmarked state firstpos(n0), where
  // n0 is the root of syntax tree T for (r)#;
  states dstates;

  state* s;
  if ((s = state::create(regex.root()->firstpos)) != nullptr) {
    if (dstates.add(s)) {
      // while (there is an unmarked state S in Dstates) {
      do {
        s = nullptr;

        for (size_t i = 0; i < dstates.size(); i++) {
          if (!dstates.get(i)->marked()) {
            s = dstates.get(i);
            break;
          }
        }

        if (!s) {
          break;
        }

        // mark S;
        s->mark();

        // for (each input symbol a) {
        for (size_t i = 0; i < regex.number_symbols(); i++) {
          // let U be the union of followpos(p) for all p in S that correspond
          // to a;
          state* u;
          if ((u = new (std::nothrow) state()) != nullptr) {
            for (size_t j = 0; j < s->size(); j++) {
              position p = s->get(j);

              if (regex.get_positions(i).contains(p)) {
                if (!u->add(_M_followpos[p])) {
                  delete u;
                  return false;
                }
              }
            }

            if (!u->empty()) {
              // if (U is not in Dstates)
              if (!dstates.contains(*u)) {
                // add U as an unmarked state to Dstates;
                if (dstates.add(u)) {
                  if ((u = state::create(*u)) == nullptr) {
                    return false;
                  }
                } else {
                  delete u;
                  return false;
                }
              }

              // Dtran[S, a] = U;
              if (!_M_transition_table.add(*s, regex.get_symbol(i), u)) {
                delete u;
                return false;
              }
            } else {
              delete u;
            }
          } else {
            return false;
          }
        }
      } while (true);

      return true;
    }

    delete s;
  }

  return false;
}

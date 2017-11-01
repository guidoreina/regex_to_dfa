#include <stdio.h>
#include <ctype.h>
#include "lex/transition_table.h"

lex::transition_table::~transition_table()
{
  if (_M_nodes) {
    for (size_t i = 0; i < _M_used; i++) {
      delete _M_nodes[i].s;

      for (size_t j = 0; j < _M_nodes[i].trans.ntrans; j++) {
        delete _M_nodes[i].trans.trans[j].u;
      }
    }

    free(_M_nodes);
  }
}

bool lex::transition_table::add(const state& s, symbol a, state* u)
{
  node* n;
  if (((n = add(s)) != nullptr) && (n->trans.ntrans < max_transitions)) {
    transition* trans = n->trans.trans + n->trans.ntrans;

    trans->a = a;
    trans->u = u;

    if (add(*u)) {
      n->trans.ntrans++;

      return true;
    }
  }

  return false;
}

void lex::transition_table::print(position endmark) const
{
  printf("Number of states: %zu.\n", _M_used);
  printf("\n");

  for (size_t i = 0; i < _M_used; i++) {
    const node* n = _M_nodes + i;

    printf("State ");
    n->s->print();

    // Accepting state?
    if (n->s->contains(endmark)) {
      printf(" (accepting state)\n");
    } else {
      printf("\n");
    }

    for (size_t j = 0; j < n->trans.ntrans; j++) {
      // If the symbol is printable...
      if (isprint(n->trans.trans[j].a)) {
        printf("\t%c -> ", n->trans.trans[j].a);
      } else {
        printf("\t0x%02x -> ", n->trans.trans[j].a);
      }

      n->trans.trans[j].u->print();
      printf("\n");
    }

    printf("\n");
  }
}

lex::transition_table::node* lex::transition_table::add(const state& s)
{
  // Search state s.
  for (size_t i = 0; i < _M_used; i++) {
    node* n = _M_nodes + i;

    if (s == *n->s) {
      return n;
    }
  }

  if (allocate_nodes()) {
    node* n = _M_nodes + _M_used;

    if ((n->s = state::create(s)) != nullptr) {
      n->trans.ntrans = 0;

      _M_used++;

      return n;
    }
  }

  return nullptr;
}

bool lex::transition_table::allocate_nodes()
{
  if (_M_used < _M_size) {
    return true;
  }

  size_t size = (_M_size > 0) ? (_M_size * 2) : 32;

  node* nodes;
  if ((nodes = static_cast<node*>(
                 realloc(_M_nodes, size * sizeof(node))
               )) != nullptr) {
    _M_nodes = nodes;
    _M_size = size;

    return true;
  }

  return false;
}

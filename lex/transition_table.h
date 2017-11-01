#ifndef LEX_TRANSITION_TABLE_H
#define LEX_TRANSITION_TABLE_H

#include "lex/state.h"
#include "lex/symbol.h"

namespace lex {
  class transition_table {
    public:
      // Constructor.
      transition_table();

      // Destructor.
      ~transition_table();

      // Add.
      bool add(const state& s, symbol a, state* u);

      // Print transition table.
      void print(position endmark) const;

    private:
      static const size_t max_transitions = 256;

      struct transition {
        symbol a;
        state* u;
      };

      struct transitions {
        transition trans[max_transitions];
        size_t ntrans;
      };

      struct node {
        state* s;
        transitions trans;
      };

      node* _M_nodes;
      size_t _M_size;
      size_t _M_used;

      // Add.
      node* add(const state& s);

      // Allocate nodes.
      bool allocate_nodes();
  };

  inline transition_table::transition_table()
    : _M_nodes(nullptr),
      _M_size(0),
      _M_used(0)
  {
  }
}

#endif // LEX_TRANSITION_TABLE_H

#ifndef LEX_DFA_H
#define LEX_DFA_H

#include "lex/regular_expression.h"
#include "lex/transition_table.h"

namespace lex {
  class dfa {
    public:
      // Constructor.
      dfa();

      // Destructor.
      ~dfa();

      // Build DFA.
      bool build(const regular_expression& regex);

      // Print.
      void print() const;

    private:
      // Number of positions.
      size_t _M_npositions;

      // followpos(p), for a position p, is the set of positions q in the entire
      // syntax tree such that there is some string x = a1 a2 ... an in L((r)#)
      // such that for some i, there is a way to explain the membership of x in
      // L((r)#) by matching ai to position p of the syntax tree and ai+1 to
      // position q.
      positions* _M_followpos;

      // Transition table.
      transition_table _M_transition_table;

      // Compute followpos.
      bool compute_followpos(const node* n);

      // Construct Dstates, the set of states of DFA D, and Dtran, the
      // transition function for D.
      bool construct_transition_function(const regular_expression& regex);
  };

  inline dfa::dfa()
    : _M_followpos(nullptr)
  {
  }

  inline dfa::~dfa()
  {
    if (_M_followpos) {
      delete [] _M_followpos;
    }
  }

  inline void dfa::print() const
  {
    _M_transition_table.print(_M_npositions - 1);
  }
}

#endif // LEX_DFA_H

#ifndef LEX_NODE_H
#define LEX_NODE_H

#include <stdint.h>
#include "lex/position.h"
#include "lex/symbol.h"

namespace lex {
  struct node {
    enum class type {
      concatenation, // c1 . c2
      alternation, // c1 | c2
      repetition_zero_or_more, // c*
      repetition_one_or_more, // c+
      optional, // c?
      symbol,
      endmark
    };

    type t;

    symbol s;
    position pos;

    node* left;
    node* right;

    // nullable(n) is true for a syntax-tree node n if and only if the
    // subexpression represented by n has the empty-string in its language.
    bool nullable;

    // firstpos(n) is the set of positions in the subtree rooted at n that
    // correspond to the first symbol of at least one string in the language of
    // the subexpression rooted at n.
    positions firstpos;

    // lastpos(n) is the set of positions in the subtree rooted at n that
    // correspond to the last symbol of at least one string in the language of
    // the subexpression rooted at n.
    positions lastpos;

    // Constructor.
    node();

    // Destructor.
    ~node();

    // Initialize.
    bool init();

    // Leaf node?
    bool leaf() const;
  };

  class nodes {
    public:
      // Constructor.
      nodes();

      // Destructor.
      ~nodes();

      // Test whether container is empty.
      bool empty() const;

      // Return size.
      size_t size() const;

      // Access next element.
      const node* top() const;
      node* top();

      // Insert element.
      bool push(node* n = nullptr);

      // Remove top element.
      void pop();

    private:
      node** _M_nodes;
      size_t _M_size;
      size_t _M_used;
  };

  inline node::node()
    : left(nullptr),
      right(nullptr)
  {
  }

  inline node::~node()
  {
    if (left) {
      delete left;
    }

    if (right) {
      delete right;
    }
  }

  inline bool node::leaf() const
  {
    switch (t) {
      case type::concatenation:
      case type::alternation:
      case type::repetition_zero_or_more:
      case type::repetition_one_or_more:
      case type::optional:
        return false;
      case type::symbol:
      case type::endmark:
        return true;
      default:
        return false;
    }
  }

  inline nodes::nodes()
    : _M_nodes(nullptr),
      _M_size(0),
      _M_used(0)
  {
  }

  inline nodes::~nodes()
  {
    if (_M_nodes) {
      for (size_t i = 0; i < _M_used; i++) {
        if (_M_nodes[i]) {
          delete _M_nodes[i];
        }
      }

      free(_M_nodes);
    }
  }

  inline bool nodes::empty() const
  {
    return (_M_used == 0);
  }

  inline size_t nodes::size() const
  {
    return _M_used;
  }

  inline const node* nodes::top() const
  {
    return (_M_used > 0) ? _M_nodes[_M_used - 1] : nullptr;
  }

  inline node* nodes::top()
  {
    return (_M_used > 0) ? _M_nodes[_M_used - 1] : nullptr;
  }

  inline void nodes::pop()
  {
    if (_M_used > 0) {
      _M_used--;
    }
  }
}

#endif // LEX_NODE_H

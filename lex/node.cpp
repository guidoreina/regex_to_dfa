#include "lex/node.h"

bool lex::node::init()
{
  // If not a leaf node...
  if (!leaf()) {
    // Initialize left and right children.
    if ((!left->init()) || ((right) && (!right->init()))) {
      return false;
    }
  }

  switch (t) {
    case type::concatenation:
      nullable = left->nullable && right->nullable;

      if ((!firstpos.add(left->firstpos)) ||
          ((left->nullable) && (!firstpos.add(right->firstpos)))) {
        return false;
      }

      if ((!lastpos.add(right->lastpos)) ||
          ((right->nullable) && (!lastpos.add(left->lastpos)))) {
        return false;
      }

      break;
    case type::alternation:
      nullable = left->nullable || right->nullable;

      if ((!firstpos.add(left->firstpos)) || (!firstpos.add(right->firstpos))) {
        return false;
      }

      if ((!lastpos.add(left->lastpos)) || (!lastpos.add(right->lastpos))) {
        return false;
      }

      break;
    case type::repetition_zero_or_more:
      nullable = true;

      if (!firstpos.add(left->firstpos)) {
        return false;
      }

      if (!lastpos.add(left->lastpos)) {
        return false;
      }

      break;
    case type::repetition_one_or_more:
      nullable = left->nullable;

      if (!firstpos.add(left->firstpos)) {
        return false;
      }

      if (!lastpos.add(left->lastpos)) {
        return false;
      }

      break;
    case type::optional:
      nullable = true;

      if (!firstpos.add(left->firstpos)) {
        return false;
      }

      if (!lastpos.add(left->lastpos)) {
        return false;
      }

      break;
    case type::symbol:
    case type::endmark:
      nullable = false;

      if (!firstpos.add(pos)) {
        return false;
      }

      if (!lastpos.add(pos)) {
        return false;
      }

      break;
  }

  return true;
}

bool lex::nodes::push(node* n)
{
  if (_M_used == _M_size) {
    size_t size = (_M_size > 0) ? (_M_size * 2) : 32;

    node** nodes;
    if ((nodes = static_cast<node**>(
                   realloc(_M_nodes, size * sizeof(node*))
                 )) != nullptr) {
      _M_nodes = nodes;
      _M_size = size;
    } else {
      return false;
    }
  }

  _M_nodes[_M_used++] = n;

  return true;
}

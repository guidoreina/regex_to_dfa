#ifndef LEX_REGULAR_EXPRESSION_H
#define LEX_REGULAR_EXPRESSION_H

#include "lex/node.h"

namespace lex {
  class regular_expression {
    public:
      // Constructor.
      regular_expression();

      // Destructor.
      ~regular_expression();

      // Parse.
      bool parse(const char* regex);

      // Get root of the syntax tree.
      const node* root() const;

      // Get number of positions.
      size_t number_positions() const;

      // Get number of symbols.
      size_t number_symbols() const;

      // Get symbol.
      symbol get_symbol(size_t idx) const;

      // Get positions.
      const positions& get_positions(size_t idx) const;

    private:
      static const size_t max_symbols = 256;

      // Root of the syntax tree.
      node* _M_root;

      size_t _M_npositions;

      struct symbol_positions_pair {
        symbol s;
        positions p;
      };

      symbol_positions_pair _M_symbols[max_symbols];
      size_t _M_nsymbols;

      // Create character class subtree.
      node* create_char_class_subtree(const bool* chars, size_t size);

      // Add (symbol, position) pair.
      bool add(symbol s, position p);

      // Add node.
      static bool add(nodes& nodes, node* n, const char*& regex);

      // Get escape character.
      static uint8_t escape_character(uint8_t c);
  };

  inline regular_expression::regular_expression()
    : _M_root(nullptr),
      _M_npositions(0),
      _M_nsymbols(0)
  {
  }

  inline regular_expression::~regular_expression()
  {
    if (_M_root) {
      delete _M_root;
    }
  }

  inline const node* regular_expression::root() const
  {
    return _M_root;
  }

  inline size_t regular_expression::number_positions() const
  {
    return _M_npositions;
  }

  inline size_t regular_expression::number_symbols() const
  {
    return _M_nsymbols;
  }

  inline symbol regular_expression::get_symbol(size_t idx) const
  {
    return _M_symbols[idx].s;
  }

  inline const positions& regular_expression::get_positions(size_t idx) const
  {
    return _M_symbols[idx].p;
  }

  inline uint8_t regular_expression::escape_character(uint8_t c)
  {
    switch (c) {
      case 'a':
        return '\a';
      case 'b':
        return '\b';
      case 'f':
        return '\f';
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case 't':
        return '\t';
      case 'v':
        return '\v';
      default:
        return c;
    }
  }
}

#endif // LEX_REGULAR_EXPRESSION_H

#ifndef LEX_POSITION_H
#define LEX_POSITION_H

#include <stdlib.h>
#include <stdio.h>

namespace lex {
  typedef size_t position;

  class positions {
    public:
      // Constructor.
      positions();

      // Destructor.
      ~positions();

      // Clear.
      void clear();

      // Empty?
      bool empty() const;

      // Get number of positions.
      size_t size() const;

      // Equal operator.
      bool operator==(const positions& p) const;

      // Add position.
      bool add(position p);

      // Add positions.
      bool add(const positions& p);

      // Remove position.
      bool remove(position p);

      // Get position.
      position get(size_t idx) const;

      // Has the position been inserted?
      bool contains(position p) const;

      // Print.
      void print() const;

    private:
      position* _M_positions;
      size_t _M_size;
      size_t _M_used;

      // Search.
      bool search(position p, size_t& idx) const;
  };

  inline positions::positions()
    : _M_positions(nullptr),
      _M_size(0),
      _M_used(0)
  {
  }

  inline positions::~positions()
  {
    if (_M_positions) {
      free(_M_positions);
    }
  }

  inline void positions::clear()
  {
    _M_used = 0;
  }

  inline bool positions::empty() const
  {
    return (_M_used == 0);
  }

  inline size_t positions::size() const
  {
    return _M_used;
  }

  inline bool positions::operator==(const positions& p) const
  {
    if (_M_used == p._M_used) {
      for (size_t i = 0; i < _M_used; i++) {
        if (_M_positions[i] != p._M_positions[i]) {
          return false;
        }
      }

      return true;
    }

    return false;
  }

  inline bool positions::add(const positions& p)
  {
    for (size_t i = 0; i < p._M_used; i++) {
      if (!add(p._M_positions[i])) {
        return false;
      }
    }

    return true;
  }

  inline position positions::get(size_t idx) const
  {
    return _M_positions[idx];
  }

  inline bool positions::contains(position p) const
  {
    size_t idx;
    return search(p, idx);
  }

  inline void positions::print() const
  {
    for (size_t i = 0; i < _M_used; i++) {
      printf("%s%u", (i > 0) ? ", " : "", _M_positions[i] + 1);
    }
  }
}

#endif // LEX_POSITION_H

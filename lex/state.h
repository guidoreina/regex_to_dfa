#ifndef LEX_STATE_H
#define LEX_STATE_H

#include <memory>
#include "lex/position.h"

namespace lex {
  class state {
    public:
      // Constructor.
      state();

      // Destructor.
      ~state() = default;

      // Create.
      static state* create(const positions& p);
      static state* create(const state& s);

      // Clear.
      void clear();

      // Empty?
      bool empty() const;

      // Get number of positions.
      size_t size() const;

      // Equal operator.
      bool operator==(const state& s) const;

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

      // Marked?
      bool marked() const;

      // Mark.
      void mark();

      // Print.
      void print() const;

    private:
      positions _M_positions;

      bool _M_marked;
  };

  class states {
    public:
      // Constructor.
      states();

      // Destructor.
      ~states();

      // Clear.
      void clear();

      // Empty?
      bool empty() const;

      // Get number of states.
      size_t size() const;

      // Add state.
      bool add(state* s);

      // Get state.
      const state* get(size_t idx) const;
      state* get(size_t idx);

      // Has the state been inserted?
      bool contains(const state& s) const;

    private:
      state** _M_states;
      size_t _M_size;
      size_t _M_used;
  };

  inline state::state()
    : _M_marked(false)
  {
  }

  inline state* state::create(const positions& p)
  {
    state* s;
    if ((s = new (std::nothrow) state()) != nullptr) {
      if (s->add(p)) {
        return s;
      }

      delete s;
    }

    return nullptr;
  }

  inline state* state::create(const state& s)
  {
    return create(s._M_positions);
  }

  inline void state::clear()
  {
    _M_positions.clear();

    _M_marked = false;
  }

  inline bool state::empty() const
  {
    return _M_positions.empty();
  }

  inline size_t state::size() const
  {
    return _M_positions.size();
  }

  inline bool state::operator==(const state& s) const
  {
    return (_M_positions == s._M_positions);
  }

  inline bool state::add(position p)
  {
    return _M_positions.add(p);
  }

  inline bool state::add(const positions& p)
  {
    return _M_positions.add(p);
  }

  inline bool state::remove(position p)
  {
    return _M_positions.remove(p);
  }

  inline position state::get(size_t idx) const
  {
    return _M_positions.get(idx);
  }

  inline bool state::contains(position p) const
  {
    return _M_positions.contains(p);
  }

  inline bool state::marked() const
  {
    return _M_marked;
  }

  inline void state::mark()
  {
    _M_marked = true;
  }

  inline void state::print() const
  {
    printf("(");
    _M_positions.print();
    printf(")");
  }

  inline states::states()
    : _M_states(nullptr),
      _M_size(0),
      _M_used(0)
  {
  }

  inline states::~states()
  {
    if (_M_states) {
      for (size_t i = 0; i < _M_used; i++) {
        delete _M_states[i];
      }

      free(_M_states);
    }
  }

  inline void states::clear()
  {
    if (_M_states) {
      for (size_t i = 0; i < _M_used; i++) {
        delete _M_states[i];
      }

      _M_used = 0;
    }
  }

  inline bool states::empty() const
  {
    return (_M_used == 0);
  }

  inline size_t states::size() const
  {
    return _M_used;
  }

  inline const state* states::get(size_t idx) const
  {
    return _M_states[idx];
  }

  inline state* states::get(size_t idx)
  {
    return _M_states[idx];
  }

  inline bool states::contains(const state& s) const
  {
    for (size_t i = 0; i < _M_used; i++) {
      if (s == *_M_states[i]) {
        return true;
      }
    }

    return false;
  }
}

#endif // LEX_STATE_H

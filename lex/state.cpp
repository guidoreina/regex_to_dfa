#include "lex/state.h"

bool lex::states::add(state* s)
{
  if (_M_used == _M_size) {
    size_t size = (_M_size > 0) ? (_M_size * 2) : 32;

    state** tmpstates;
    if ((tmpstates = static_cast<state**>(
                       realloc(_M_states, size * sizeof(state*))
                     )) != nullptr) {
      _M_states = tmpstates;
      _M_size = size;
    } else {
      return false;
    }
  }

  _M_states[_M_used++] = s;

  return true;
}

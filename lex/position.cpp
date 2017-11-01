#include <string.h>
#include "lex/position.h"

bool lex::positions::add(position p)
{
  // If the position has not been already inserted...
  size_t idx;
  if (!search(p, idx)) {
    if (_M_used == _M_size) {
      size_t size = (_M_size > 0) ? (_M_size * 2) : 8;

      position* tmppos;
      if ((tmppos = static_cast<position*>(
                      realloc(_M_positions, size * sizeof(position))
                    )) != nullptr) {
        _M_positions = tmppos;
        _M_size = size;
      } else {
        return false;
      }
    }

    if (idx < _M_used) {
      memmove(&_M_positions[idx + 1],
              &_M_positions[idx],
              (_M_used - idx) * sizeof(position));
    }

    _M_positions[idx] = p;

    _M_used++;
  }

  return true;
}

bool lex::positions::remove(position p)
{
  // If the position has been inserted...
  size_t idx;
  if (search(p, idx)) {
    if (idx < --_M_used) {
      memmove(&_M_positions[idx],
              &_M_positions[idx + 1],
              (_M_used - idx) * sizeof(position));
    }

    return true;
  }

  return false;
}

bool lex::positions::search(position p, size_t& idx) const
{
  int i = 0;
  int j = _M_used - 1;

  while (i <= j) {
    int mid = (i + j) / 2;

    if (p < _M_positions[mid]) {
      j = mid - 1;
    } else if (p > _M_positions[mid]) {
      i = mid + 1;
    } else {
      idx = mid;
      return true;
    }
  }

  idx = i;

  return false;
}

#ifndef MACROS_H
#define MACROS_H

#define ARRAY_SIZE(x)     (sizeof(x) / sizeof(*(x)))

#ifndef MAX
  #define MAX(x, y)       (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
  #define MIN(x, y)       (((x) < (y)) ? (x) : (y))
#endif

#endif // MACROS_H

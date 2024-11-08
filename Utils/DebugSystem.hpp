#ifndef DBG_SYS
#define DBG_SYS

#include <iostream>

#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL 0
#endif

#define msg(level) if (level <= DEBUG_LEVEL) std::cout << "[Debug]: "
#define msgEndl std::endl

#endif

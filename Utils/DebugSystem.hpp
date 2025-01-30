#ifndef DBG_SYS
  #define DBG_SYS

  #include <iostream>

  #ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 0
  #endif

  #define msg(level) if (level <= DEBUG_LEVEL) std::cout << "[Debug]: "
  #define msgEndl std::endl

  #ifdef DEBUG_LEVEL
    #define dbg_assert(cond, message)                                   \
      do {                                                              \
        if (not cond) {                                                \
          std::cerr << "Assertion failed: (" << #cond << "), "      \
          << "function " << __FUNCTION__                 \
          << ", file " << __FILE__                       \
          << ", line " << __LINE__ << ".\n"              \
          << "Message: " << message << "\n";             \
          std::abort();                                             \
        }                                                             \
      } while (false)
  #else
    #define dbg_assert(cond, message) do { } while (false)
  #endif
#endif

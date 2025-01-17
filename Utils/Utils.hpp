#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <map>
#include <chrono>

#if defined(BITS64)
  typedef long long int Int;
  typedef unsigned long long int uInt;
  #define UINTMAX ULONG_MAX
  #define INTMAX LONG_MAX
#else
  typedef int Int;
  typedef unsigned int uInt;
  #define UINTMAX 4294967295
  #define INTMAX 2147483647
#endif


#define TIMERSTART(label)             \
  std::chrono::time_point<std::chrono::high_resolution_clock> a##label, b##label; \
  a##label = std::chrono::high_resolution_clock::now();

#define TIMERSTOP(label)                                            \
  b##label = std::chrono::high_resolution_clock::now();             \
  std::chrono::duration<double> delta##label = b##label - a##label; \
  std::cout << "# elapsed time (" << #label << "): "                \
            << delta##label.count() << "s" << std::endl;


namespace utils
{
  std::string now_time();
  std::vector<std::string> splitString(std::string line, std::string delims);
  #if 0
  std::unordered_map<std::string, std::string> parseArguments(int argc, char const *argv[], std::unordered_map<std::string, std::string> *);
  #endif
  std::string modify_path(std::string old_path, int n,std::string text);
  bool validateExtension(std::string, std::string);

  class SigHnd {
    private:
      static int done;
    public:
      static void signal_handler(int) {
        char ans = '0';
        std::cout << "\n\nContinue, interrupt, exit? [1/2/3]\n";
        std::cin >> ans;
        if (ans == '1') {
            std::cout << "continuing\n\n";
        } else if (ans == '2') {
            done = 1;
            std::cout << "Interrupt next iteration \n\n";
        } else if (ans == '3'){
            std::cout << "exit\n";
            exit(0);
        } else {
            std::cout << "continuing\n\n";
        }
        return;
      }   
      static int get_state() {
        return done;
      }   
  };
};

#endif
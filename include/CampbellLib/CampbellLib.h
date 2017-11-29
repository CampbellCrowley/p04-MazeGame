#ifndef CAMPBELL_LIB_H
#define CAMPBELL_LIB_H

#include <algorithm>
#include <iostream>

namespace Campbell {
// Other common things that don't have their own place yet.
namespace Common {
static bool isdigit(const char input);
}; // namespace Common

// For things relating to color in the terminal.
namespace Color {
// Escape codes for terminal colors.
static const char *reset = "\033[1;0m";
static const char *black = "\033[1;30m";
static const char *red = "\033[1;31m";
static const char *green = "\033[1;32m";
static const char *yellow = "\033[1;33m";
static const char *blue = "\033[1;34m";
static const char *magenta = "\033[1;35m";
static const char *cyan = "\033[1;36m";
static const char *white = "\033[1;37m";
}; // namespace Color

// For things relating to arrays.
namespace Arrays {
// Finds the number of elements in an array input[].
static inline int length(const char *input) {
  int Size = 0;
  while (input[Size] != '\0')
    Size++;
  return Size;
}
template <typename T> static inline int length(T &input) {
  return sizeof input / sizeof input[0];
}
}; // namespace Arrays

// For things relating to arrays of characters and their manipuation or
// comparison.
namespace Strings {
// Check if a string is a valid number. May start with -/+ and contain one
// decimal and contain only numbers.
static bool isNumber(const char input[]);
// Converts a string to a double if isStringNumber returns true. Otherwise
// returns 0.
static double toNumber(const char input[]);
// Takes a user input and chooses whether the input is equivalent to "Yes" or
// "No". If defaultYes is true, it will assume true unless the input begins
// with an 'n'. If false, it will assume "No" unless the input begins with
// 'y'.
static bool toYesNo(const char input[], bool defaultYes = true);
static bool getYesNo(bool defaultYes = true);

}; // namespace Strings

// Class for writing tests.
class Test {
private:
  Test() {}

  // Outputs the message that the test passed.
  static void PASS() {
    std::cout << Campbell::Color::green << "PASS\n" << Campbell::Color::reset;
  }
  // Outputs message that the test failed as well as expected and actual values.
  template <typename T> static void FAIL(T actual, T expected) {
    std::cout << Campbell::Color::red << "FAIL" << Campbell::Color::reset
              << "\n Expected: " << expected << "\n Actual: " << actual
              << std::endl;
  }

public:
  // Checks for exact equality.
  template <typename T> static void ASSERT(T actual, T expected) {
    if (expected == actual) {
      PASS();
    } else {
      FAIL(actual, expected);
      exit(1);
    }
  }
  // Checks that two numbers are similar, to account for floating point
  // precision issues.
  template <typename T> static void ASSERT_SIMILAR(T actual, T expected) {
    double epsilon = std::max(abs(actual), abs(expected)) * 1E-15;
    if (abs(actual - expected) <= epsilon) {
      PASS();
    } else {
      FAIL(actual, expected);
    }
  }

}; // class Test
} // namespace Campbell

#include "CampbellLib.cc"
#endif // CAMPBELL_LIB_H

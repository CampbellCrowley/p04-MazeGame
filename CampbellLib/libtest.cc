#include "CampbellLib.h"
#include <iostream>

int main() {
  // cout rounds to a number that's too small by default.
  std::cout.precision(12);

  {
    using namespace Campbell::Color;
    std::cout << "Normal " << red << "Red " << yellow << "Yellow " << green
              << "Green " << blue << "Blue " << cyan << "Cyan " << magenta
              << "Magenta " << white << "White " << black << "Black " << reset
              << "Reset.\n";
  }

  std::cout << std::endl;

  {
    // Test Arrays
    std::cout << "Arrays\n";
    const char *string = "I am 13 long.";
    Campbell::Test::ASSERT(Campbell::Arrays::length(string), 13);
    string = "";
    Campbell::Test::ASSERT(Campbell::Arrays::length(string), 0);
    int integers[] = {1, 2, 3, 4, 5, 6, 7};
    Campbell::Test::ASSERT(Campbell::Arrays::length(integers), 7);
    int integers2[0];
    Campbell::Test::ASSERT(Campbell::Arrays::length(integers2), 0);
    bool booleans[] = {true, false, true, false, true, true};
    Campbell::Test::ASSERT(Campbell::Arrays::length(booleans), 6);
    bool booleans2[0];
    Campbell::Test::ASSERT(Campbell::Arrays::length(booleans2), 0);
  }

  std::cout << std::endl;

  {
    // Test Numbers
    std::cout << "Number checks\n";
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("Not a number"), false);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber(""), false);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("1"), true);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("10"), true);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("1.0"), true);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("-1.0"), true);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("+1.0"), true);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("1234567890"), true);
    Campbell::Test::ASSERT(Campbell::Strings::isNumber("123abc"), false);
  }

  std::cout << std::endl;

  {
    // Test string to double conversion.
    std::cout << "toNumber\n";
    std::string num = "-1234.56789";
    Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()),
                           -1234.56789);
    num = "123";
    Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()),
                           (double)123);
    num = "1.23";
    Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()), 1.23);
    num = "-123";
    Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()),
                           (double)-123);
    num = ".123";
    Campbell::Test::ASSERT_SIMILAR(Campbell::Strings::toNumber(num.c_str()),
                                   .123);
    num = "-.123";
    Campbell::Test::ASSERT_SIMILAR(Campbell::Strings::toNumber(num.c_str()),
                                   -0.123);
  }

  std::cout << std::endl;

  {
    // Test yesno input.
    std::cout << "yesno input\n";
    std::string yes = "Yes";
    std::string no = "No";
    std::string unk = "Uknown";

    Campbell::Test::ASSERT(Campbell::Strings::toYesNo(yes.c_str(), true), true);
    Campbell::Test::ASSERT(Campbell::Strings::toYesNo(no.c_str(), true), false);
    Campbell::Test::ASSERT(Campbell::Strings::toYesNo(unk.c_str(), true), true);
    Campbell::Test::ASSERT(Campbell::Strings::toYesNo(yes.c_str(), false),
                           true);
    Campbell::Test::ASSERT(Campbell::Strings::toYesNo(no.c_str(), false),
                           false);
    Campbell::Test::ASSERT(Campbell::Strings::toYesNo(unk.c_str(), false),
                           false);
  }

  std::cout << std::endl;

  {
    // Test Digits
    std::cout << "Digit checks\n";
    Campbell::Test::ASSERT(Campbell::Common::isdigit('0'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('1'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('2'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('3'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('4'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('5'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('6'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('7'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('8'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('9'), true);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('a'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('b'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('c'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('-'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('.'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('+'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('+'), false);
    Campbell::Test::ASSERT(Campbell::Common::isdigit('/'), false);
  }
}

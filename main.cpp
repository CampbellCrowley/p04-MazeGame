#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "CampbellLib/CampbellLib.h"

using namespace std;

// Global Vars
struct winsize size;

// Entry
int main() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  cout << "Width: " << size.ws_col << " Height: " << size.ws_row << endl;

  return 0;
}  // int main()

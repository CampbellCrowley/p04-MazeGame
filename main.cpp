#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  cout << "Width: " << size.ws_col << " Height: " << size.ws_row << endl;
  return 0;
}

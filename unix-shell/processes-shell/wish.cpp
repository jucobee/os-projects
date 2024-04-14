#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  // repeat forever
  while (1) {
    string cmd;

    cout << "wish> ";
    getline(cin, cmd);
  }
}

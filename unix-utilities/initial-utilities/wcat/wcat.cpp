#include <algorithm>
#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  // file descriptor
  // basically will be a reference to the opened file
  int fd;

  // see if the user passed in an argument
  // if argc == 1, no additional arguments passed besides the program name
  if (argc == 1) {
    return 0;
  } else {
    for (int i = 1; i < argc; i++) {
      fd = open(argv[i], O_RDONLY);
      if (fd == -1) {
        cout << "wcat: cannot open file" << endl;
        // this signifies an error
        return 1;
      }

      char buffer[4096];
      // stores the return value of read function
      int ret;

      while ((ret = read(fd, buffer, 4096)) > 0) {
        // this prints what was read by read()
        write(STDOUT_FILENO, buffer, ret);
      }
      close(fd);
    }
  }

  return 0;
}

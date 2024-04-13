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
  // itself
  if (argc == 1) {
    // it's already open
    fd = STDIN_FILENO;
    // STDIN_FILENO is the standard input file descriptor
  } else {
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
      cerr << "could not open " << argv[1] << endl;
      // this signifies an error
      return 1;
    }
  }

  char buffer[4096];
  int bytesRead = 0;
  // stores the return value of read function
  int ret;

  while ((ret = read(fd, buffer, 4096)) > 0) {
    bytesRead += ret;
  }

  if (argc != 1) {
    close(fd);
  }

  cout << "Byte count: " << bytesRead << endl;

  return 0;
}

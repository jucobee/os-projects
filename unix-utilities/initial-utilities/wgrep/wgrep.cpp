#include <algorithm>
#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace std;

void grep(int fd, const char *searchterm) {
  const int BUFFER_SIZE = 4096;
  char buffer[BUFFER_SIZE];
  int bytesRead;    // stores the return value of read function
  string line = ""; // stores the current line

  while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
    line.append(buffer, bytesRead); // appends what was read to the string
    int pos = 0;                    // start at the first character of line

    // iterate through buffer, storing each newline position
    // find() will return the position of the first occurence of the serach
    // term and the last position (string::npos) if search pattern is not
    // found
    while ((pos = line.find('\n')) != string::npos) {
      // check if searchterm is in this line
      if (line.find(searchterm) < pos) {
        // print the line searchterm was found on
        write(STDOUT_FILENO, line.c_str(), pos + 1);
      }
      // erase this line and move on to the next one
      line.erase(0, pos + 1);
    }

    // print the last line if searchterm found
    if (!line.empty() && line.find(searchterm) != string::npos) {
      write(STDOUT_FILENO, line.c_str(), pos);
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  // file descriptor
  // basically will be a reference to the opened file
  int fd;

  // see if the user passed in an argument
  // if argc == 1, no additional arguments passed besides the program name
  if (argc == 1) {
    cout << "wgrep: searchterm [file ...]" << endl;
    return 1;
  }

  // pattern to search for
  const char *searchterm = argv[1];

  // if argc == 2, no file specified, read from standard input
  if (argc == 2) {
    fd = STDIN_FILENO;
    grep(fd, searchterm);
  } else {
    // iterate through all files
    for (int i = 2; i < argc; i++) {
      fd = open(argv[i], O_RDONLY);
      if (fd == -1) {
        cout << "wgrep: cannot open file" << endl;
        // error
        return 1;
      }
      grep(fd, searchterm);
    } // end for
  }   // end else

  return 0;
}

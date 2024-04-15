#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  // repeat forever
  while (true) {
    string cmd;

    cout << "wish> ";
    getline(cin, cmd); // read input from terminal

    pid_t pid = fork();
    if (pid == 0) {
      /* Child */
      // TODO
      // execv(const char *path, char *const *argv)

    } else if (pid > 0) {
      /* Parent */
      int status;
      waitpid(pid, &status, 0);

    } else {
      cerr << "fork error" << endl;
      return 1;
    }
  }
  return 0;
}

#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int built_in_command(string cmd) {
  if (cmd == "exit")
    exit(0);

  // return 0 back to main if command is not built in
  return 0;
}

int main(int argc, char *argv[]) {
  // Path to directory with all programs
  string path = "/bin/";

  // repeat forever
  while (true) {
    string cmd;
    cout << "wish> ";
    getline(cin, cmd); // read input from terminal

    // attempt to execute built in command and restart while loop
    if (built_in_command(cmd))
      continue;

    // make new process for command
    pid_t pid = fork();
    if (pid == 0) {
      /* Child */
      path += cmd;

      // Arguments to be passed to the program
      const char *args[] = {path.c_str(), NULL};
      execv(path.c_str(), const_cast<char *const *>(args));

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

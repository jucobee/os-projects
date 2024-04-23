#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

// Path to directory with all programs
string path = "/bin/";

vector<string> process_input(string input) {
    vector<string> result;

    // define starting position of string parsing at first occurence of
    // non-whitespace then define first found position of whitespace after that
    size_t start_pos = input.find_first_not_of(" \t");
    size_t found_pos = input.find_first_of(" \t", start_pos);

    while (start_pos != string::npos) {
        // extract first argument and append it to result
        string substring =
            input.substr(start_pos, min(found_pos, input.size()) - start_pos);
        result.push_back(substring);
        // update positions and restart loop
        start_pos = input.find_first_not_of(" \t", found_pos);
        found_pos = input.find_first_of(" \t", start_pos);
    }

    return result;
}

int built_in_command(vector<string> cmd) {
    if (cmd[0] == "exit")
        exit(0);

    else if (cmd[0] == "cd") {
        // cd only takes one arg and chdir syscall must return 0
        if (cmd.size() != 2 || chdir(cmd[1].c_str()) != 0) {
            cerr << "An error has occured" << endl;
        }
        return 0;

    } else if (cmd[0] == "path") {
        if (cmd.size() == 1)
            path = "";
        else // TODO: handle multiple paths
            path = cmd[1];
        return 0;
    }

    // return 1 back to main if command is not built in
    return 1;
}

int main(int argc, char *argv[]) {
    // repeat forever
    while (true) {
        cout << "wish> ";
        string input;
        getline(cin, input); // read input from terminal

        // split input into vector of arguments
        vector<string> args = process_input(input);

        // attempt to execute built in command and restart while loop if so
        if (built_in_command(args) == 0) {
            continue;
        }

        // make new process for command
        pid_t pid = fork();
        if (pid == 0) {
            /* Child */
            // append program to path
            path.append(args[0]);

            // get argv in form that execv wants
            const char *argv[args.size() + 1];
            argv[0] = path.c_str();
            for (int i = 1; i < args.size(); i++) {
                argv[i] = args[i].c_str();
            }
            argv[args.size()] = NULL;

            // Arguments to be passed to the program
            execv(argv[0], const_cast<char *const *>(argv));

            cerr << "An error has occurred" << endl;
            exit(1);

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

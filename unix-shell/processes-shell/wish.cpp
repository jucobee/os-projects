#include <iostream>

#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

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
        cout << "wish> ";
        string input;
        getline(cin, input); // read input from terminal

        // attempt to execute built in command and restart while loop if so
        built_in_command(input);

        // split input into vector of arguments
        vector<string> args = process_input(input);

        // make new process for command
        pid_t pid = fork();
        if (pid == 0) {
            /* Child */
            path.append(args[0]);

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

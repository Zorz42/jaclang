#pragma once

namespace error {
    void syntaxError(const std::string &error); // Print out syntax error

    void treeError(const std::string &error); // Print out syntax error

    void terminate(const std::string &reason, int exitCode); // Exit if error or something
}

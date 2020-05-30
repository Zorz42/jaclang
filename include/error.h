#pragma once

enum errorType {et_arg_count, et_syntax_err, et_unable_to_open_file, et_invalid_arg, et_invalid_opt, et_data_err};

namespace error {
    void syntaxError(const std::string &error); // Print out syntax error

    void treeError(const std::string &error); // Print out syntax error

    void terminate(const std::string &reason, errorType errorType_); // Exit if error or something
}

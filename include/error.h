#pragma once

enum ErrorType {Err_Arg_Count, Err_Syntax_Error, Err_Semantic_Error, Err_Unable_To_Open_File, Err_Invalid_Arg, Err_Invalid_Opt, Err_Data_Error};

namespace error {
    void syntaxError(const std::string &error); // Print out syntax error

    void semanticError(const std::string &error); // Print out syntax error

    void terminate(const std::string &reason, ErrorType error_type); // Exit if error or something
}

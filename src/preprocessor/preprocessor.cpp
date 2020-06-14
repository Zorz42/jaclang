// this file will preprocess the input code.

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#include <fstream>  // open file

void preprocessor::main(const std::list<std::string> &raw_input_file) {
    /*
     Preprocessor removes comments and follows directives
     */
    bool in_multiline_comment = false, line_comment = false; // says if its in multiline comment or line comment
    for(const std::string &line : raw_input_file) {
        // preprocessor
        for(unsigned long i = 0; i < line.length(); i++) {
            if(i < line.size())
                if(line.at(i) == ';')
                    line_comment = true;

            if(i < line.size() - 1 && !line_comment)
                if(line.at(i) == '/' && line.at(i + 1) == '*') // if multiline comment starts
                    in_multiline_comment = true;

            if(i > 1 && !line_comment)
                if(line.at(i - 1) == '*' && line.at(i) == '/') { // if multiline comment ends
                    in_multiline_comment = false;
                    if(++i == line.length())
                        break;
                }

            if(!line_comment && !in_multiline_comment)
                file::input_text.push_back(line.at(i)); // adds line of code to the output

        }
        if(!in_multiline_comment)
            file::input_text.push_back('\n'); // adds line of code to the output
        line_comment = false;
    }

    file::input_text.push_back(' '); // So that lexer stores the last token for certainty
}

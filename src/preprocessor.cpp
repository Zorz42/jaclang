// this file will preprocess the input code.

#include "jaclang.h"

#include <fstream>  // open file

void preprocessor::main(const std::list<std::string> &rawInputFile) {
    bool inMultilineComment = false;
    bool lineComment = false;
    for (const std::string &line : rawInputFile) {
        // preprocessor
        for (unsigned long i = 0; i < line.length(); i++) {
            if (i < line.size())
                if (line.at(i) == ';')
                    lineComment = true;

            if (i < line.size() - 1 && !lineComment) // prevent out of range
                if (line.at(i) == '/' && line.at(i + 1) == '*') // if multiline comment starts
                    inMultilineComment = true;

            if (i > 1 && !lineComment)
                if (line.at(i - 2) == '*' && line.at(i - 1) == '/') // if multiline comment ends
                    inMultilineComment = false;

            if (!lineComment && !inMultilineComment) // character is not in comment
                file::inputText.push_back(line.at(i)); // adds line of code to the output

        }
        if (!inMultilineComment) // character is not in multiline comment
            file::inputText.push_back('\n'); // adds line of code to the output
        lineComment = false;
    }

    file::inputText.push_back(' '); // So that lexer stores the last token for sure
}

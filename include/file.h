#pragma once

namespace file {
    inline unsigned long asm_data;
    inline unsigned long asm_bss;
    inline unsigned long asm_text;
    inline unsigned long asm_func;

    inline std::list<char> inputText; // Text of the input file
    inline std::vector<std::string> outputVector; // Vector of lines of code (output file)

    void read(const std::string &text); // Read input file and put result in inputFileVector
    void write(const std::string &output); // When finished compiling, c++ code will be on outputFileVector. Transfer it to output file

    std::string getLine(int LINE); // Get text of line

    void append_data(const std::string &line);

    void append_bss(const std::string &line);

    void append_text(const std::string &line);

    void append_func(const std::string &line);

    void insert(const std::string &line, unsigned long position);
}

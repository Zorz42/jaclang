#pragma once

namespace file {
    inline unsigned long asm_data;
    inline unsigned long asm_bss;
    inline unsigned long asm_text;
    inline unsigned long asm_func;

    inline std::list<char> input_text; // Text of the input file
    inline std::vector<std::string> output_vector; // Vector of lines of code (output file)

    void read(const std::string &text); // Read input file and put result in inputFileVector
    void write(const std::string &output); // When finished compiling, c++ code will be on outputFileVector. Transfer it to output file

    std::string getLine(int LINE); // Get text of line

    void appendData(const std::string &line);

    void appendBss(const std::string &line);

    void appendText(const std::string &line);

    void appendFunc(const std::string &line);

    void insert(const std::string &line, unsigned long position);
}

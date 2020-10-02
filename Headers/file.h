namespace file {
    inline unsigned long asm_data;
    inline unsigned long asm_bss;
    inline unsigned long asm_text;
    inline unsigned long asm_func;

    inline std::list<char> input_text; // Text of the input file
    inline std::vector<std::string> output_vector; // Vector of lines of code (output file)
    inline std::string input_file, output_file;

    void read(); // Read input file and put result in input_file_vector
    void write(); // When finished compiling, c++ code will be on output_file_vector. Transfer it to output file

    std::string getLine(int LINE); // Get text of line

    void appendData(const std::string &line);
    void appendBss(const std::string &line);
    void appendText(const std::string &line);
    void appendFunc(const std::string &line);
    void insert(const std::string &line, unsigned long position);
}

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void file::read(const std::string &text) { // read file
    std::ifstream input_file_obj(text); // open file and store it in std::ifstream object
    if(!input_file_obj.is_open()) { // if didn't open (file missing,...)
        std::cerr << "\033[1;31mFile does not exist!\033[0m" << std::endl;
        error::terminate("UNABLE TO OPEN FILE", Err_Unable_To_Open_File);
    }

    std::string line;
    std::list<std::string> raw_input_file;
    while(std::getline(input_file_obj, line)) // iterate through lines of input file
        raw_input_file.push_back(line);
    input_file_obj.close(); // close the file - file has been read
    preprocessor::main(&raw_input_file); // call preprocessor
}

void file::write(const std::string &file_output) { // write to file
    std::ofstream output_file_obj(file_output); // open file (or create)
    if(output_file_obj.is_open()) // if file was opened (or created)
        for(auto &t : file::output_vector) // add line from vector
            output_file_obj << t << "\n"; // add new line so that the code wont be in the same line

    output_file_obj.close(); // close file, c++ code has been written
}

void file::appendData(const std::string &line) {
    file::insert(line, file::asm_data); // append text to data section
    file::asm_data++;
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::appendBss(const std::string &line) {
    file::insert(line, file::asm_bss); // append text to bss section
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::appendText(const std::string &line) {
    file::insert(line, file::asm_text); // append text to text section
    file::asm_text++;
    file::asm_func++;
}

void file::appendFunc(const std::string &line) {
    file::insert(line, file::asm_func); // append text to text section
    file::asm_func++;
}


void file::insert(const std::string &line, unsigned long position) {
    file::output_vector.insert(file::output_vector.begin() + position, line); // insert line of code into asm file
}

std::string file::getLine(int line) { // get line of code
    int current_line = 1;
    std::string current_string;
    auto iter = file::input_text.begin();
    for(; current_line != line; iter++) // until we are in the line desired
        if(*iter == '\n')
            current_line++; // if '\n' (newline) then it is next line
    iter--;
    for(; *++iter != '\n';) // when desired line, read line until '\n'
        current_string += *iter;

    return current_string;
}

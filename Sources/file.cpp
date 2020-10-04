#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void file::read() { // read file
    std::ifstream input_file_obj(input_file);
    if(!input_file_obj.is_open()) {
        std::cerr << RED << "File does not exist!" << WHITE << std::endl;
        error::terminate("UNABLE TO OPEN FILE", Err_Unable_To_Open_File);
    }
    input_text.assign((std::istreambuf_iterator<char>(input_file_obj)), std::istreambuf_iterator<char>()); // convert file to std::list<char>
    
    preprocessor::main(file::input_text.begin(), file::input_text.end(), file::input_file); // call preprocessor
}

void file::write() { // write to file
    std::ofstream output_file_obj(file::output_file);
    if(output_file_obj.is_open())
        for(auto &t : file::output_vector)
            output_file_obj << t << "\n"; // add new line so that the code wont be in the same line
    else {
        std::cerr << RED << "Could not write to file " << file::output_file << "!" << WHITE << std::endl;
        error::terminate("WRITE ERROR", Err_Data_Error);
    }
    output_file_obj.close();
}

void file::appendData(const std::string &line) {
    file::output_vector.insert(file::output_vector.begin() + file::asm_data, line); // append text to data section
    file::asm_data++;
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::appendBss(const std::string &line) {
    file::output_vector.insert(file::output_vector.begin() + file::asm_bss, line); // append text to bss section
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::appendText(const std::string &line) {
    file::output_vector.insert(file::output_vector.begin() + file::asm_text, line); // append text to text section
    file::asm_text++;
    file::asm_func++;
}

void file::appendFunc(const std::string &line) {
    file::output_vector.insert(file::output_vector.begin() + file::asm_func, line); // append text to text section
    file::asm_func++;
}

std::string file::getLine(int line) { // get line of code
    auto iter = file::input_text.begin();
    for(int i = 1; i != line; iter++) // until we are in the line desired
        if(*iter == '\n')
            i++; // if '\n' (newline) then it is next line
    std::string result;
    for(; *iter != '\n'; iter++) // when desired line, read line until '\n'
        result += *iter;
    return result;
}

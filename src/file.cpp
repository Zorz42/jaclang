#include "jaclang.h"

#include <fstream>  // open file

void file::read(const std::string& text) { // read file
    std::ifstream inputFileObj(text); // open file and store it in std::ifstream object
    if(!inputFileObj.is_open()) { // if didn't open (file missing,...)
        std::cout << "\033[1;31mFile does not exist!\033[0m" << std::endl;
        error::terminate("UNABLE TO OPEN FILE", ERROR_UNABLE_TO_OPEN_FILE);
    }

    std::string line;
    std::list<std::string> rawInputFile;
    while(std::getline(inputFileObj,line)) // iterate through lines of input file
        rawInputFile.push_back(line);
    inputFileObj.close(); // close the file - file has been read
    preprocessor::main(rawInputFile); // call preprocessor
}

void file::write(const std::string& file_output) { // write to file
    std::ofstream outputFileObj(file_output); // open file (or create)
    if (outputFileObj.is_open()) // if file was opened (or created)
        for (auto & t : file::outputVector) // add line from vector
            outputFileObj << t << "\n"; // add new line so that the code wont be in the same line

    outputFileObj.close(); // close file, c++ code has been written
}

void file::append_data(const std::string& line) {
    file::add(line, file::asm_data); // append text to data section
    file::asm_data++;
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::append_bss(const std::string& line) {
    file::add(line, file::asm_bss); // append text to bss section
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::append_text(const std::string& line) {
    file::add(line, file::asm_text); // append text to text section
    file::asm_text++;
    file::asm_func++;
}

void file::append_func(const std::string& line) {
    file::add(line, file::asm_func); // append text to text section
    file::asm_func++;
}


void file::add(const std::string& line, unsigned long position) {
    file::outputVector.insert(file::outputVector.begin() + position, line); // insert line of code into asm file
}

std::string file::getLine(int LINE) { // get line of code
    int currentLine = 1;
    std::string currentString;
    std::list<char>::iterator iter = file::inputText.begin();
    for(; currentLine != LINE; iter++) // until we are in the line desired
        if(*iter == '\n')
            currentLine++; // if '\n' (newline) then it is next line
    iter--;
    for(;*++iter != '\n';) // when desired line, read line until '\n'
        currentString += *iter;

    return currentString;
}

void file::append_instruction(const std::string& instruction, const std::string& arg1, const std::string& arg2) {
    std::string expr; // returns asm instruction: instruction arg1, arg2
    expr = "   ";
    expr += instruction;
    expr += " ";
    expr += arg1;
    if(!arg2.empty())
        expr += ", ";
    expr += arg2;
    file::append(expr);
}

void file::append(const std::string& line) {
    if(generator::inFunction) // if code should be in function append it to function section
        file::append_func(line);
    else
        file::append_text(line);
}

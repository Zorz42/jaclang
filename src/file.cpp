#include "jaclang.h"

void file::read(const std::string& text) // read file
{
    preprocessor::main(text); // call preprocessor
}

void file::write(const std::string& file_output) // write to file
{
    std::string command = "touch "; // create file (if not existing)
    command += file_output;
    command += ".asm";
    system(command.c_str()); // create file
    std::ofstream outputFileObj(file_output + ".asm"); // open file (or create)
    if (outputFileObj.is_open()) // if file was opened (or created)
        for (auto & t : file::outputVector) // add line from vector
            outputFileObj << t << "\n"; // add new line so that the code wont be in the same line

    outputFileObj.close(); // close file, c++ code has been written
}

void file::append_data(const std::string& line)
{
    file::add(line, file::asm_data); // append text to data section
    file::asm_data++;
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::append_bss(const std::string& line)
{
    file::add(line, file::asm_bss); // append text to bss section
    file::asm_bss++;
    file::asm_text++;
    file::asm_func++;
}

void file::append_text(const std::string& line)
{
    file::add(line, file::asm_text); // append text to text section
    file::asm_text++;
    file::asm_func++;
}

void file::append_func(const std::string& line)
{
    file::add(line, file::asm_func); // append text to text section
    file::asm_func++;
}


void file::add(const std::string& line, unsigned long position)
{
    file::outputVector.insert(file::outputVector.begin() + position, line); // insert line of code into asm file
}

/*void file::add(const std::string& line)
{
	file::outputVector.push_back(line); // append line of code into asm file
}*/

std::string file::getLine(int LINE) // get line of code
{
    int currentLine = 1;
    std::string currentString;
    int i = 0;
    for(; currentLine != LINE; i++) // until we are in the line desired
        if(file::inputText.at(i) == '\n')
            currentLine++; // if '\n' (newline) then it is next line

    for(;file::inputText.at(i) != '\n'; i++) // when desired line, read line until '\n'
        currentString += file::inputText.at(i);

    return currentString;
}

void file::append_instruction(const std::string& instruction, const std::string& arg1, const std::string& arg2)
{
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

void file::append(const std::string& line)
{
    if(generator::inFunction) // if code should be in function append it to function section
        file::append_func(line);
    else
        file::append_text(line);
}

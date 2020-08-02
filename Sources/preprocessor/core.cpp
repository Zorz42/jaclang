// this file will preprocess the input code.

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#include <fstream>  // open file

void removeComments();
void processIncludes();

void convertIntoString();

void getInputFileDirectory();
std::string input_file_directory;
extern std::string input_file;

std::list<std::string> *raw_input_file;

void preprocessor::main(std::list<std::string> *raw_input_file_) {
    /*
     Preprocessor removes comments and follows directives
     */
    raw_input_file = raw_input_file_;
    getInputFileDirectory();
    
    removeComments();
    processIncludes();
    removeComments();
    
    convertIntoString();
}

void getInputFileDirectory() {
    input_file_directory = input_file; // pops characters until '/' to get directory
    while(!input_file_directory.empty() && input_file_directory.back() != '/')
        input_file_directory.pop_back();
    if(input_file_directory.empty()) // if there is just "example.jl" (for example) for input file, then do "./"
        input_file_directory = "./";
}

void removeComments() {
    bool in_multiline_comment = false;
    unsigned long start = 0;
    
    for(std::string &line : *raw_input_file) {
        for(unsigned long i = 0; i < line.length(); i++) {
            if(line.at(i) == ';') // remove single line comments
                line.erase(line.begin() + i, line.end());
            
            else if(i < line.size() - 1 && line.at(i) == '/' && line.at(i + 1) == '*') { // if multiline comment starts
                in_multiline_comment = true;
                start = i;
                i += 3;
            }
            
            if(in_multiline_comment) {
                if(!i)
                    i = 1;
                while(true) {
                    if(i >= line.size()) {
                        line.erase(line.begin() + start, line.end());
                        break;
                    }
                    else if(line.at(i - 1) == '*' && line.at(i) == '/') {
                        line.erase(line.begin() + start, line.begin() + i + 1);
                        in_multiline_comment = false;
                        break;
                    }
                    i++;
                }
            }
        }
    }
}

void processIncludes() {
    std::string include_string = "include ";
    for(auto line = raw_input_file->begin(); line != raw_input_file->end(); line++) {
        for(unsigned long i = 0; i < line->length(); i++) {
            if(line->at(i) == include_string.at(0)) { // check if line starts with "include"
                bool exists = true;
                for(unsigned long i2 = 1; i2 < include_string.length() && i2 < line->length(); i2++)
                    if(line->at(i + i2) != include_string.at(i2)) {
                        exists = false;
                        break;
                    }
                if(exists) {
                    std::string include_path = *line;
                    include_path.erase(include_path.begin(), include_path.begin() + i + include_string.size()); // remove tabs and "include" in front
                    
                    while(include_path.back() == ' ') // remove spaces in the end
                        include_path.pop_back();
                    
                    unsigned long spaces_in_front = 0; // remove spaces in the front
                    while(include_path.at(spaces_in_front) == ' ')
                        spaces_in_front++;
                    include_path.erase(include_path.begin(), include_path.begin() + spaces_in_front);
                    
                    line->clear(); // remove include directive
                    
                    // read file and insert it
                    std::ifstream input_file_obj(input_file_directory + include_path + ".jlh"); // all headers must end with .jlh
                    if(!input_file_obj.is_open()) { // if didn't open (file could be missing)
                        std::cout << "\033[1;31mFile '" << include_path << "' could not be included!\033[0m" << std::endl;
                        error::terminate("SYNTAX ERROR", Err_Syntax_Error);
                    }

                    std::string line_;
                    while(std::getline(input_file_obj, line_)) // iterate through lines of input file
                        raw_input_file->insert(line, line_);
                    input_file_obj.close(); // close the file
                }
            }
            else if(line->at(i) != ' ' && line->at(i) != '\t') // ignore spaces and tabs in the front
                break;
        }
    }
}

void convertIntoString() {
    for(const std::string &line : *raw_input_file) { // append every single line
        for(unsigned long i = 0; i < line.length(); i++)
            file::input_text.push_back(line.at(i));
        file::input_text.push_back('\n');
    }
    file::input_text.push_back(' '); // So that lexer stores the last token for certainty
}

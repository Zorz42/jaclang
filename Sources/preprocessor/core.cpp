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

bool startsWith(const std::string& target, const std::string& phrase, unsigned long index) {
    for(unsigned long i = 0; i < phrase.size(); i++)
        if(target.at(i + index) != phrase.at(i))
            return false;
    return true;
}

#define LIBRARY_DIRECTORY "/usr/local/Jac/Libraries/"

bool includeFile(const std::string& path, std::list<std::string>::iterator& line) {
    // read file and insert it
    std::ifstream input_file_obj(path); // all headers must end with .jlh
    if(!input_file_obj.is_open()) { // if didn't open (file could be missing)
        return false;
    }

    line->clear(); // remove include directive
    
    std::string line_;
    while(std::getline(input_file_obj, line_)) // iterate through lines of input file
        raw_input_file->insert(line, line_);
    input_file_obj.close(); // close the file
    return true;
}

std::string parseDirective(std::string line) {
    unsigned int i = 0;
    while(line.at(i) == ' ' || line.at(i) == '\t') // remove all spaces and tabs in front of directive
        i++;
    while(line.at(i) != ' ' && line.at(i) != '\t') // remove directive word
        i++;
    while(line.at(i) == ' ' || line.at(i) == '\t') // remove spaces and tabs between word and parameter
        i++;

    line.erase(line.begin(), line.begin() + i);
    
    i = 0;
    while(line.at(line.size() - i - 1) == ' ' || line.at(line.size() - i - 1) == '\t')
        i++;
    
    line.erase(line.end() - i, line.end());
    return line;
}

void processIncludes() {
    std::string include_string = "include ";
    std::string import_string = "import ";
    for(auto line = raw_input_file->begin(); line != raw_input_file->end(); line++) {
        for(unsigned long i = 0; i < line->length(); i++) {
            if(startsWith(*line, include_string, i)) {
                std::string include_path = parseDirective(*line);
                
                if(!includeFile(input_file_directory + include_path + ".jlh", line)) {
                    std::cout << "\033[1;31m" << *line << std::endl
                    << "File '" << include_path << "' could not be included!\033[0m" << std::endl;
                    error::terminate("SYNTAX ERROR", Err_Syntax_Error);
                }
            }
            else if(startsWith(*line, import_string, i)) {
                std::string include_path = parseDirective(*line);
                
                if(!includeFile(LIBRARY_DIRECTORY + include_path + "/Headers/__main__.jlh", line)) {
                    std::cout << "\033[1;31m" << *line << std::endl
                    << "Library '" << include_path << "' does not exist!\033[0m" << std::endl;
                    error::terminate("SYNTAX ERROR", Err_Syntax_Error);
                }
                std::cout << include_path << std::endl;
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

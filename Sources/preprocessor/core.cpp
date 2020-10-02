// this file will preprocess the input code.

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void preprocess(std::list<char>::iterator start, std::list<char>::iterator end, std::string file_dir);
void removeComments(std::list<char>::iterator start, std::list<char>::iterator end);
void processIncludes(std::list<char>::iterator start, std::list<char>::iterator end);
std::string getDirectoryOfFile(std::string path);

std::stack<std::string> file_dirs;

void preprocessor::main() {
    /*
     Preprocessor removes comments and follows directives
     */
    
    file::input_text.insert(file::input_text.begin(), '\n');
    preprocess(file::input_text.begin(), file::input_text.end(), getDirectoryOfFile(file::input_file));
}

void preprocess(std::list<char>::iterator start, std::list<char>::iterator end, std::string file_dir) {
    file_dirs.push(file_dir);
    
    removeComments(start, end);
    processIncludes(start, end);
    
    file_dirs.pop();
}

std::string getDirectoryOfFile(std::string path) {
    while(!path.empty() && path.back() != '/')
        path.pop_back();
    // if there is just "example.jl" for input file, then do "./"
    return path.empty() ? "./" : path;
}

void removeComments(std::list<char>::iterator start, std::list<char>::iterator end) {
    std::list<char>::iterator prev = file::input_text.begin();
    for(auto i = start; i != end; i++) {
        if(*i == '#') {
            for(start = i; *i != '\n' && i != end; i++);
            file::input_text.erase(start, i);
        }
        else if(*prev == '/' && *i == '*') {
            for(start = prev; i != end && (*i != '*' || *++i != '/'); i++);
            if(i != end)
                i++;
            file::input_text.erase(start, i);
        }
        prev = i;
    }
}

bool startsWith(const std::string phrase, std::list<char>::iterator iter) {
    for(unsigned long i = 0; i < phrase.size(); i++)
        if(*iter++ != phrase.at(i))
            return false;
    return true;
}

bool includeFile(const std::string path, std::list<char>::iterator iter) {
    // read file and insert it
    std::ifstream input_file_obj(path); // all headers must end with .jlh
    if(!input_file_obj.is_open()) { // if didn't open (file could be missing)
        return false;
    }
    
    std::list<char>::iterator start = iter;
    while(*iter != '\n') iter++;
    file::input_text.erase(start, iter); // remove include directive
    
    std::string line;
    while(std::getline(input_file_obj, line)) // iterate through lines of input file
        for(char c : line)
            file::input_text.insert(iter, c);
    input_file_obj.close(); // close the file
    return true;
}

bool includeFileFromDirs(const std::string& file_name, std::list<char>::iterator iter) {
    if(includeFile(file_dirs.top() + file_name + ".jlh", iter))
        return true;
    
    for(const std::string& i : preprocessor::include_dirs)
        if(includeFile(i + file_name + ".jlh", iter))
            return true;
    return false;
}

std::string parseDirective(std::list<char>::iterator i) {
    std::string result;
    while(*i == ' ' || *i == '\t') // remove all spaces and tabs in front of directive
        i++;
    while(*i != ' ' && *i != '\t') // remove directive word
        i++;
    while(*i == ' ' || *i == '\t') // remove spaces and tabs between word and parameter
        i++;
    
    while(*i != '\n')
        result.push_back(*i++);
    return result;
}

void processIncludes(std::list<char>::iterator start, std::list<char>::iterator end) {
    std::string include_string = "include ";
    std::string import_string = "import ";
    
    for(auto i = start; i != end; i++) {
        if(startsWith(include_string, i)) {
            std::string include_path = parseDirective(i);
            
            if(!includeFileFromDirs(include_path, i)) {
                std::cerr << "File '" << include_path << "' could not be included!\033[0m" << std::endl;
                error::terminate("SYNTAX ERROR", Err_Syntax_Error);
            }
        }
        else if(startsWith(import_string, i)) {
            std::string include_path = parseDirective(i);
            
            if(!includeFile("/usr/local/Jac/Libraries/" + include_path + "/Headers/__main__.jlh", i)) {
                std::cerr << "Library '" << include_path << "' does not exist!\033[0m" << std::endl;
                error::terminate("SYNTAX ERROR", Err_Syntax_Error);
            }
            preprocessor::imports_to_dump.push_back(include_path);
        }
        else if(*i != ' ' && *i != '\t')
            while(i != end && *i != '\n')
                i++;
    }
    
    /*for(unsigned long iter = 0; iter < raw_input_file->size(); iter++)
        for(unsigned long i = 0; i < line.length(); i++) {
            if(startsWith(line, include_string, i)) {
                std::string include_path = parseDirective(line);
                
                if(!includeFileFromDirs(include_path, iter)) {
                    std::cerr << "\033[1;31m" << line << std::endl
                    << "File '" << include_path << "' could not be included!\033[0m" << std::endl;
                    error::terminate("SYNTAX ERROR", Err_Syntax_Error);
                }
            }
            else if(startsWith(line, import_string, i)) {
                std::string include_path = parseDirective(line);
                
                if(!includeFile(LIBRARY_DIRECTORY + include_path + "/Headers/__main__.jlh", iter)) {
                    std::cerr << "\033[1;31m" << line << std::endl
                    << "Library '" << include_path << "' does not exist!\033[0m" << std::endl;
                    error::terminate("SYNTAX ERROR", Err_Syntax_Error);
                }
                preprocessor::imports_to_dump.push_back(include_path);
            }
            else if(line.at(i) != ' ' && line.at(i) != '\t') // ignore spaces and tabs in the front
                break;
        }*/
}

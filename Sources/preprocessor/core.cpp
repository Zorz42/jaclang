// this file will preprocess the input code.

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void removeComments(std::list<char>::iterator start, std::list<char>::iterator end);
void processIncludes(std::list<char>::iterator start, std::list<char>::iterator end);

std::string file_dir;

std::string getDirectoryOfFile(std::string path) {
    while(!path.empty() && path.back() != '/')
        path.pop_back();
    return path.empty() ? "./" : path; // if there is just "example.jl" for input file, then do "./"
}

void preprocessor::main(std::list<char>::iterator start, std::list<char>::iterator end, std::string file) {
    // Preprocessor removes comments and follows directives
    std::string prev_file_dir = file_dir;
    file_dir = getDirectoryOfFile(file);
    
    removeComments(start, end);
    processIncludes(start, end);
    
    file_dir = prev_file_dir;
}

void removeComments(std::list<char>::iterator start, std::list<char>::iterator end) {
    for(auto i = start; i++ != end;) {
        if(*i == '#') { // #single line comment
            for(start = i; *i != '\n' && i++ != end;);
            file::input_text.erase(start, i);
        }
        else if(*i == '/' && *++i == '*') { // /* multiline comment */
            for(start = --i; i != end && (*i != '*' || *++i != '/'); i++);
            file::input_text.erase(start, ++i);
        }
    }
}

bool startsWith(const std::string phrase, std::list<char>::iterator iter) {
    for(unsigned long i = 0; i < phrase.size(); i++)
        if(*iter++ != phrase.at(i))
            return false;
    return *iter == ' ' || *iter == '\t';
}

bool includeFile(const std::string path, std::list<char>::iterator& iter) {
    // read file and insert it
    std::ifstream input_file_obj(path);
    if(!input_file_obj.is_open()) // if didn't open (file could be missing)
        return false;
    
    std::list<char>::iterator start = iter, pre_start = --iter;
    while(*++iter != '\n');
    file::input_text.erase(start, iter); // remove include directive
    
    std::string line;
    while(std::getline(input_file_obj, line)) { // iterate through lines of input file
        for(char c : line)
            file::input_text.insert(iter, c);
        file::input_text.insert(iter, '\n');
    }
    
    preprocessor::main(pre_start, iter, path);
    input_file_obj.close(); // close the file
    return true;
}

bool includeFileFromDirs(const std::string& file_name, std::list<char>::iterator& iter) {
    if(includeFile(file_dir + file_name + ".jlh", iter))
        return true;
    
    for(const std::string& i : preprocessor::include_dirs)
        if(includeFile(i + file_name + ".jlh", iter))
            return true;
    return false;
}

std::string parseDirective(std::list<char>::iterator i) {
    while(*i != ' ' && *i++ != '\t'); // remove directive word
    while(*i == ' ' || *i == '\t') i++; // remove spaces and tabs between word and parameter
    std::string result;
    while(*i != '\n' && *i != ' ' && *i != '\t')
        result.push_back(*i++);
    return result;
}

void processIncludes(std::list<char>::iterator start, std::list<char>::iterator end) {
    for(auto i = start; i != end; i++) {
        if(startsWith("include", i)) {
            std::string include_path = parseDirective(i);
            
            if(!includeFileFromDirs(include_path, i)) {
                std::cerr << RED << "File '" << include_path << "' could not be included!" << WHITE << std::endl;
                error::terminate("SYNTAX ERROR", Err_Syntax_Error);
            }
        }
        else if(startsWith("import", i)) {
            std::string include_path = parseDirective(i);
            
            if(!includeFile("/usr/local/Jac/Libraries/" + include_path + "/Headers/__main__.jlh", i)) {
                std::cerr << RED << "Library '" << include_path << "' does not exist!" << WHITE << std::endl;
                error::terminate("SYNTAX ERROR", Err_Syntax_Error);
            }
            if(preprocessor::dump_imports)
                preprocessor::imports_to_dump.push_back(include_path);
        }
        else if(*i != ' ' && *i != '\t')
            while(*i != '\n') {
                i++;
                if(i == end)
                    return;
            }
    }
}

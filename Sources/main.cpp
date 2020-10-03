#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

// the main file, where the spine of the program is
bool optimize = false;

void init();
void handle_arguments(int argc, char **argv);
void compile_jaclang();

int main(int argc, char **argv) {
    /*
     The main file where everything happens. This is the center of all code.
     It first handles command line arguments and then compiles jaclang.
     */

    handle_arguments(argc, argv);
    init();
    
    compile_jaclang();

    return 0; // exit success
}

void compile_jaclang() {
    /*
     This is the backbone of the actual compiler.
     There are all the steps executed to compile.
     */
    
    file::read(); // Read file
    
    lexer::main(); // convert code into tokens
    
    parser::main(); // convert tokens into syntax tree
    lexer::tokens.clear();
    file::input_text.clear();
    
    if(optimize)
        optimizer::optimize(&parser::main_branch);
    
    if(parser::debug_show_ast)
        printAST(parser::main_branch);
    
    generator::current_branch_scope = &parser::main_branch;
    generator::initialMain(); // generate assembly tokens out of syntax tree
    
    parser::main_branch.sub.clear();
    
    asm_::main(); // generate assembly code from assembly tokens and optimize it
    
    asm_::instructions.clear();
    file::write(); // writes to file
        
    for(const std::string& import : preprocessor::imports_to_dump)
        std::cout << import << std::endl;
}

void handle_arguments(int argc, char **argv) {
    // This iterates through arguments to parse them and separate --options and -p parameters and parameters
    
    std::vector<std::string> args, complete_args, args_with_params;
    for(int i = 1; i < argc; i++)
        complete_args.emplace_back(argv[i]);
    for(const std::string &i : complete_args) {
        if(i[0] == '-' && i[1] == '-') {
            if(i == "--debug") {
                lexer::debug_show_tokens = true;
                parser::debug_show_ast = true;
            } else if(i == "--debug-tokens")
                lexer::debug_show_tokens = true;
            else if(i == "--debug-ast")
                parser::debug_show_ast = true;
            else if(i == "--version") {
                std::cout << "BETA " << MAJOR << "." << MINOR << "." << PATCH << std::endl;
                exit(0);
            }
            else if(i == "--no-optimizations")
                optimize = false;
            else if(i == "--__dump-imports")
                preprocessor::dump_imports = true;
            else {
                std::cerr << RED << i << " is not a valid argument!" << WHITE << std::endl;
                error::terminate("INVALID OPTION", Err_Invalid_Opt);
            }
        } else // it does not belong to options, so its something else
            args_with_params.emplace_back(i);
    }
    
    file::output_file = "out.s"; // output file is default 'out.o', but it's different if specified
    for(unsigned long i = 0; i < args_with_params.size(); i++) {
        if(args_with_params.at(i) == "-") {
            std::cerr << RED << args_with_params.at(i) << " is a forbidden argument!" << WHITE << std::endl; // error
            error::terminate("INVALID ARGUMENT", Err_Invalid_Arg);
        } else if(args_with_params.at(i)[0] == '-') { // parse the option
            char curr_arg_name = args_with_params.at(i)[1];
            if(args_with_params.at(i).size() == 2 && ++i == args_with_params.size()) {
                std::cerr << RED << "-" << curr_arg_name << " has no argument!" << WHITE << std::endl; // error
                error::terminate("INVALID ARGUMENT", Err_Invalid_Arg);
            } else
                args_with_params.at(i).erase(args_with_params.at(i).begin(), args_with_params.at(i).begin() + 2); // remove -[any character] in the beggining
            std::string curr_arg = args_with_params.at(i);
            switch(curr_arg_name) {
                case 'o':
                    file::output_file = curr_arg;
                    break;
                case 'i':
                    preprocessor::include_dirs.push_back(curr_arg + "/");
                    break;
                default:
                    std::cerr << RED << "-" << curr_arg_name << " is not a valid argument!" << WHITE << std::endl; // error
                    error::terminate("INVALID ARGUMENT", Err_Invalid_Arg);
            }
        } else
            args.emplace_back(args_with_params.at(i));
    }
    
    if(args.empty() && args_with_params.empty()) { // if there are no arguments or help
        std::ifstream help_file("/usr/local/Jac/Data/jaclang-help.txt");
        if(help_file.is_open())
            std::cout << "Jaclang beta " << MAJOR << "." << MINOR << "." << PATCH << " - help" << std::endl << help_file.rdbuf(); // print help text
        else {
            std::cerr << RED << "Cannot open help-text file (/usr/local/Jac/Data/jaclang-help.txt)!" << WHITE << std::endl; // file missing
            error::terminate("DATA MISSING OR CORRUPTED", Err_Data_Error);
        }
        exit(0);
    } else if(args.size() != 1) { // if there is more args than 1
        std::cerr << RED << "You must specify one input file and not more or less!" << WHITE << std::endl;
        error::terminate("INVALID ARGUMENT COUNT", Err_Arg_Count);
    }
    file::input_file = args.at(0);
}

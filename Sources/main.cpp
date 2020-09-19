#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

// the main file, where the spine of the program is

std::string input_file;
std::string output_file;

bool quiet = false, optimize = false, dump_imports = false;

long start;
void init();
void start_timer();
void end_timer();
void handle_arguments(int argc, char **argv);
void compile_jaclang();

int main(int argc, char **argv) {
    /*
     The main file where everything happens. This is the center of all code.
     It first handles command line arguments and then compiles jaclang.
     */
    
    if(!quiet)
        start_timer();

    handle_arguments(argc, argv);
    init();

    compile_jaclang();

    if(!quiet)
        end_timer();

    return 0; // exit success
}

void compile_jaclang() {
    /*
     This is the backbone of the actual compiler.
     There are all the steps executed to compile.
     */
    file::read(input_file); // Read file

    lexer::main(); // convert code into tokens
    
    parser::main(input_file); // convert tokens into syntax tree
    lexer::tokens.clear();
    file::input_text.clear();
    
    if(optimize)
        optimizer::optimize(&parser::main_branch);
    if(parser::debug_show_ast)
        printAST(parser::main_branch);
    
    generator::current_branch_scope = &parser::main_branch;
    std::string main_func_name = "s" + input_file;
    for(unsigned int i = 0; i < main_func_name.size(); i++) // replace all "/" with ".."
        if(main_func_name.at(i) == '/') {
            main_func_name.at(i) = '.';
        } else if((main_func_name.at(i) < 'A' || main_func_name.at(i) > 'Z') &&
                  (main_func_name.at(i) < 'a' || main_func_name.at(i) > 'z'))
            main_func_name.erase(i, 1);
    asm_::append_instruction(".globl", main_func_name);
    asm_::append_instruction(main_func_name + ":");
    generator::main(true); // generate assembly tokens out of syntax tree
    asm_::append_instruction("ret");
    
    asm_::main(); // generate assembly code from assembly tokens and optimize it
    
    file::write(output_file); // writes to file
    
    if(dump_imports)
        for(const std::string& import : preprocessor::imports_to_dump)
            std::cout << import << std::endl;
}

void start_timer() {
    // Just a timer to time compilation in milliseconds using std::chrono
    using namespace std::chrono;
    start = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count();
}

void end_timer() {
    // Ends timer and prints compilation time
    using namespace std::chrono;
    long end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); // end timer
    std::string ms = std::to_string((end - start) % 1000); // get milliseconds time difference
    unsigned long len = ms.length(); // add zeros to have 4 char string for milliseconds
    for(unsigned int i = 4; i > len; i--)
        ms.insert(ms.begin(), '0'); // insert zeros

    std::cout << "Compilation time: " << (end - start) / 1000 << "." << ms << " s"
              << std::endl;
}

void handle_arguments(int argc, char **argv) {
    /*
     This iterates through arguments to parse them and separate --options and -o specific_parameters and normal_parameters
     */
    bool display_version = false;
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
            else if(i == "--quiet")
                quiet = true;
            else if(i == "--version")
                display_version = true;
            else if(i == "--no-optimizations")
                optimize = false;
            else if(i == "--__dump-imports") {
                quiet = true;
                dump_imports = true;
            }
            else {
                std::cout << "\033[1;31m" << i << " is not a valid argument!\033[0m"
                          << std::endl;
                error::terminate("INVALID OPTION", Err_Invalid_Opt);
            }
        } else // it does not belong to options, so its something else
            args_with_params.emplace_back(i);
    }
    complete_args.clear();
    
    output_file = "out.s"; // output file is default 'out.o', but it's different if specified
    for(unsigned long i = 0; i < args_with_params.size(); i++) {
        if(args_with_params.at(i) == "-") {
            std::cout << "\033[1;31m" << args_with_params.at(i) << " is a forbidden argument!\033[0m"
                      << std::endl; // error
            error::terminate("INVALID ARGUMENT", Err_Invalid_Arg);
        } else if(args_with_params.at(i)[0] == '-') { // parse the option
            char curr_arg_name = args_with_params.at(i)[1];
            std::string curr_arg;
            if(args_with_params.at(i).size() == 2) {
                i++;
                if(i == args_with_params.size()) {
                    std::cout << "\033[1;31m-" << curr_arg_name << " has no argument!\033[0m"
                              << std::endl; // error
                    error::terminate("INVALID ARGUMENT", Err_Invalid_Arg);
                }
                curr_arg = args_with_params.at(i);
            } else {
                curr_arg = args_with_params.at(i);
                curr_arg.erase(curr_arg.begin());
                curr_arg.erase(curr_arg.begin());
            }
            switch(curr_arg_name) {
                case 'o':
                    output_file = curr_arg;
                    break;
                default:
                    std::cout << "\033[1;31m-" << curr_arg_name << " is not a valid argument!\033[0m"
                              << std::endl; // error
                    error::terminate("INVALID ARGUMENT", Err_Invalid_Arg);
            }
        } else
            args.emplace_back(args_with_params.at(i));
    }

    // do specific thing is before specified
    
    if(display_version) {
        std::cout << "BETA " << MAJOR << "." << MINOR << "." << PATCH << std::endl;
        exit(0);
    } else if(args.empty() && args_with_params.empty()) { // if there are no arguments or help
        std::ifstream help_file("/usr/local/Jac/Data/jaclang-help.txt");
        if(help_file.is_open()) {
            std::cout << "Jaclang beta " << MAJOR << "." << MINOR << "." << PATCH << " - help" << std::endl;
            std::cout << help_file.rdbuf(); // print help text
        } else {
            std::cerr << "\033[1;31mCannot open help-text file (/usr/local/Jac/Data/jaclang-help.txt)!\033[0m"
                      << std::endl; // file missing
            error::terminate("DATA MISSING OR CORRUPTED", Err_Data_Error);
        }
        exit(0);
    } else if(args.size() > 1) { // if there is more args than 1
        std::cerr << "\033[1;31mOnly one input file is allowed for now!\033[0m" << std::endl;
        error::terminate("INVALID ARGUMENT COUNT", Err_Arg_Count);
    } else if(args.empty()) {
        std::cerr << "\033[1;31mNo input file!\033[0m" << std::endl;
        error::terminate("INVALID ARGUMENT COUNT", Err_Arg_Count);
    }
    args_with_params.clear();
    input_file = args.at(0);
}

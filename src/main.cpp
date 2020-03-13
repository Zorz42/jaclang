#include "jaclang.h"

// the main file, where the main function is happening

#include <chrono>   // time
#include <fstream>  // read/write to file
#include <unistd.h>
#include <pwd.h>

std::string homeDir;

// if compilation is being debugged (default: false)
bool debug_show_tokens = false;
bool debug_show_ast = false;

unsigned long file::asm_data; // integers for tracking sections in asm file
unsigned long file::asm_bss;
unsigned long file::asm_text;
unsigned long file::asm_func;

std::list<char> file::inputText; // input file
std::vector<std::string> file::outputVector; // prefix for asm file

std::string inputFile;
std::string outputFile;

bool quiet = false;

long start;

void init();

void start_timer();

void end_timer();

void handle_arguments(int argc, char **argv);

void compile_jaclang();

int main(int argc, char **argv) {
    if (!quiet)
        start_timer();

    handle_arguments(argc, argv);
    init();

    compile_jaclang();

    if (!quiet)
        end_timer();

    return 0; // exit success
}

void compile_jaclang() {
    file::read(inputFile); // Read file

    lexer::main(); // convert code into tokens
    parser::main(inputFile); // convert tokens into syntax tree
    lexer::tokens.clear();
    file::inputText.clear();
    if (debug_show_ast)
        printAST(mainBranch);
    currentBranchScope = &mainBranch;
    generator::main(); // generate assembly code out of syntax tree

    file::write(outputFile); // Writes to file
}

void start_timer() {
    using namespace std::chrono;
    start = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count(); // get current time in milliseconds for timing compilation
}

void end_timer() {
    using namespace std::chrono;
    long end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); // end timer
    std::string ms = std::to_string((end - start) % 1000); // get milliseconds time difference
    unsigned long len = ms.length(); // add zeros to have 4 char string for milliseconds
    for (unsigned int i = 4; i > len; i--)
        ms.insert(ms.begin(), '0'); // insert zeros

    std::cout << "Compilation time: " << (end - start) / 1000 << "." << ms << " s"
              << std::endl; // tell compilation time
}

void handle_arguments(int argc, char **argv) {
    bool help = false;
    std::vector<std::string> args, completeArgs, argsWithParams; // vector of command line arguments
    for (int i = 1; i < argc; i++)
        completeArgs.emplace_back(argv[i]);
    for (const std::string &i : completeArgs) { // go through arguments
        if (i[0] == '-' && i[1] == '-') { // if option
            if (i == "--help")
                help = true;
            else if (i == "--debug-all") {
                debug_show_tokens = true;
                debug_show_ast = true;
            } else if (i == "--debug-tokens")
                debug_show_tokens = true;
            else if (i == "--debug-ast")
                debug_show_ast = true;
            else if (i == "--quiet")
                quiet = true;
            else {
                std::cout << "\033[1;31m" << i << " is not a valid argument!\033[0m"
                          << std::endl; // error
                error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
            }
        } else // if not option
            argsWithParams.emplace_back(i); // append it to args
    }
    completeArgs.clear();
    outputFile = "out.asm";
    for (unsigned long i = 0; i < argsWithParams.size(); i++) {
        if (argsWithParams.at(i) == "-") {
            std::cout << "\033[1;31m" << argsWithParams.at(i) << " is a forbidden argument!\033[0m"
                      << std::endl; // error
            error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
        } else if (argsWithParams.at(i)[0] == '-') {
            char currArgName = argsWithParams.at(i)[1];
            std::string currArg;
            if (argsWithParams.at(i).size() == 2) {
                i++;
                if (i == argsWithParams.size()) {
                    std::cout << "\033[1;31m-" << currArgName << " has no argument!\033[0m"
                              << std::endl; // error
                    error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
                }
                currArg = argsWithParams.at(i);
            } else {
                currArg = argsWithParams.at(i);
                currArg.erase(currArg.begin());
                currArg.erase(currArg.begin());
            }
            switch (currArgName) {
                case 'o':
                    outputFile = currArg;
                    break;
                default:
                    std::cout << "\033[1;31m-" << currArgName << " is not a valid argument!\033[0m"
                              << std::endl; // error
                    error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
            }
        } else
            args.emplace_back(argsWithParams.at(i));
    }

    if ((args.empty() && argsWithParams.empty()) || help) { // if there are no arguments or help
        passwd *pw = getpwuid(getuid());
        homeDir = pw->pw_dir;
        std::ifstream helpFile(homeDir + "/.local/share/jaclang-data/help-text.txt");
        if (helpFile.is_open()) {
            std::cout << VERSION << std::endl;
            std::cout << helpFile.rdbuf(); // print help text
        } else {
            std::cout << "\033[1;31mCannot open help-text file (~/.local/share/jaclang-data/help-text.txt)!\033[0m"
                      << std::endl; // file missing
            error::terminate("DATA MISSING OR CORRUPTED", ERROR_DATA_ERROR);
        }
        exit(0);
    } else if (args.size() > 1) { // if there is more args than 1
        std::cout << "\033[1;31mOnly one input file is allowed for now!\033[0m" << std::endl;
        error::terminate("INVALID ARGUMENT COUNT", ERROR_ARGUMENT_COUNT);
    } else if (args.empty()) {
        std::cout << "\033[1;31mNo input file!\033[0m" << std::endl;
        error::terminate("INVALID ARGUMENT COUNT", ERROR_ARGUMENT_COUNT);
    }
    argsWithParams.clear();
    inputFile = args.at(0);
}

#include "jaclang.h"

// the main file, where the main function is happening

#include <chrono>   // time
#include <fstream>  // read/write to file

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
bool keep = false;

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
    std::vector<std::string> args; // vector of command line arguments
    std::vector<file::param> temp;
    for (int i = 1; i < argc; i++) { // go through arguments
        if (argv[i][0] == '-' && argv[i][1] == '-') { // if option
            if(strcmp(argv[i], "--help"))
                help = true;
            else if(strcmp(argv[i], "--debug-all")) {
                debug_show_tokens = true;
                debug_show_ast = true;
            }
            else if(strcmp(argv[i], "--debug-tokens"))
                debug_show_tokens = true;
            else if(strcmp(argv[i], "--debug-ast"))
                debug_show_ast = true;
            else if(strcmp(argv[i], "--quiet"))
                quiet = true;
            else {
                std::cout << "\033[1;31m" << argv[i] << " is not a valid argument!\033[0m"
                          << std::endl; // error
                error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
            }
        } else // if not option
            args.emplace_back(argv[i]); // append it to args
    }

    if (args.empty() || help) { // if there are no arguments or help
        std::ifstream helpFile("/usr/local/bin/jaclang-data/help-text.txt");
        if (helpFile.is_open()) {
            std::cout << VERSION << std::endl;
            std::cout << helpFile.rdbuf(); // print help text
        } else {
            std::cout << "\033[1;31mCannot open help-text file (/usr/local/bin/jaclang-data/help-text.txt)!\033[0m"
                      << std::endl; // file missing
            error::terminate("DATA MISSING OR CORRUPTED", ERROR_DATA_ERROR);
        }
        exit(0);
    } else if (args.size() > 1) { // if there is more args than 1
        std::cout << "\033[1;31mOnly one input file is allowed for now!\033[0m" << std::endl;
        error::terminate("INVALID ARGUMENT COUNT", ERROR_ARGUMENT_COUNT);
    }

    std::string inputFile = args.at(0);
    outputFile = "out.asm";
}

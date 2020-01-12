#include "jaclang.h"

// the main file, where the main function is happening

#include <chrono>   // time
#include <sys/stat.h> // check if file/directory exists
#include <fstream>

struct stat info;

bool cacheDirExisted;
std::string cacheDir;

// if compilation is being debugged (default: false)
bool debug_show_tokens = false;
bool debug_show_ast = false;

unsigned long file::asm_data; // integers for tracking sections in asm file
unsigned long file::asm_bss;
unsigned long file::asm_text;
unsigned long file::asm_func;

std::list<char> file::inputText; // input file
std::vector<std::string> file::outputVector; // prefix for asm file

std::string jaclangInput;
std::string jaclangToNasm;
std::string nasmToLinker;
std::string binaryOutput;

bool quiet = false;
bool keep = false;

long start;

void init();

void start_timer();

void end_timer();

void handle_arguments(int argc, char **argv);

void compile_jaclang();

void compile_assembly();

void link_object();

void create_cache_dir();

void remove_cache_dir(bool exitSuccess);

std::string getFormat(std::string &file);

std::string join(const std::string &filename, const std::string &end) {
    return filename + std::string(".") + end;
}

int main(int argc, char **argv) {
    cacheDir = ".jlcache";

    if (!quiet)
        start_timer();

    handle_arguments(argc, argv);

    if (!jaclangInput.empty() && !jaclangToNasm.empty())
        init();

    create_cache_dir();

    if (!jaclangInput.empty() && !jaclangToNasm.empty())
        compile_jaclang();
    if (!jaclangToNasm.empty() && !nasmToLinker.empty())
        compile_assembly();
    if (!nasmToLinker.empty() && !binaryOutput.empty())
        link_object();

    if (!quiet)
        std::cout << "\033[1;32mCompilation successful!\033[0m" << std::endl; // compilation successful!

    if (!keep)
        remove_cache_dir(true);

    if (!quiet)
        end_timer();

    return 0; // exit success
}

void compile_jaclang() {
    file::read(jaclangInput); // Read file

    lexer::main(); // convert code into tokens
    parser::main(jaclangInput); // convert tokens into syntax tree
    lexer::tokens.clear();
    file::inputText.clear();
    if (debug_show_ast)
        printAST(mainBranch);
    currentBranchScope = &mainBranch;
    generator::main(); // generate assembly code out of syntax tree


    file::write(jaclangToNasm); // Writes to file
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
    std::vector<std::string> args; // vector of command line arguments
    for (int i = 1; i < argc; i++) { // go through arguments
        if (argv[i][0] == '-') { // if option
            for (int i2 = 1; argv[i][i2] != 0; i2++) { // go through options
                switch (argv[i][i2]) {
                    case 'd': // debug (show tokens and ast)
                        debug_show_tokens = true;
                        debug_show_ast = true;
                        break;
                    case 't': // show tokens
                        debug_show_tokens = true;
                        break;
                    case 'a': // show ast
                        debug_show_ast = true;
                        break;
                    case 'q': // be quiet (still show errors)
                        quiet = true;
                        break;
                    case 'k': // keep cache
                        keep = true;
                        break;
                    default:
                        std::cout << "\033[1;31m" << argv[i][i2] << " is not a valid argument!\033[0m"
                                  << std::endl; // error
                        error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
                }
            }
        } else // if not option
            args.emplace_back(argv[i]); // append it to args
    }

    if (args.empty()) { // if there are no arguments
        std::ifstream helpFile("/usr/local/bin/jaclang-data/help-text.txt");
        if (helpFile.is_open()) {
            std::cout << helpFile.rdbuf(); // print help text
        } else {
            std::cout << "\033[1;31mCannot open help-text file (/usr/local/bin/jaclang-data/help-text.txt)!\033[0m"
                      << std::endl; // file missing
            error::terminate("DATA MISSING OR CORRUPTED", ERROR_DATA_ERROR);
        }
        exit(0);
    } else if (args.size() == 1) { // if there is misc option
        if (args.at(0) == "version") // check for every misc option
            std::cout << VERSION_STR << " ID:" << VERSION_INT << std::endl;
        else if (args.at(0) == "versionid")
            std::cout << VERSION_INT << std::endl;
        else if (args.at(0) == "versionstr")
            std::cout << VERSION_STR << std::endl;
        else {
            std::cout << "\033[1;31mInvalid option: " << args.at(0) << "\033[0m" << std::endl;
            error::terminate("INVALID OPTION", ERROR_INVALID_OPTION);
        }
        exit(0);
    } else if (args.size() > 2) { // if there is more args than 2
        std::cout << "\033[1;31mMust input 2 arguments or less!\033[0m" << std::endl;
        error::terminate("TOO MANY ARGUMENTS", ERROR_ARGUMENT_COUNT);
    }

    std::string outputFileName = args.at(1);
    std::string outputFormat = getFormat(args.at(1));
    std::string inputFormat = getFormat(args.at(0));
    if (!outputFormat.empty()) {
        for (; outputFileName.at(outputFileName.size() - 1) != '.';)
            outputFileName.pop_back();
        outputFileName.pop_back();
    }
    if (!inputFormat.empty()) {
        if (inputFormat == "lj") { // because format gets read backwards
            jaclangInput = args.at(0);
            jaclangToNasm = cacheDir + "/" + join(outputFileName, "asm");
            nasmToLinker = cacheDir + "/" + join(outputFileName, "o");
            binaryOutput = cacheDir + "/" + outputFileName;
        } else if (inputFormat == "msa") {
            jaclangToNasm = args.at(0);
            nasmToLinker = cacheDir + "/" + join(outputFileName, "o");
            binaryOutput = cacheDir + "/" + outputFileName;
        } else if (inputFormat == "o") {
            nasmToLinker = join(outputFileName, "o");
            binaryOutput = cacheDir + "/" + outputFileName;
        } else {
            std::cout << "\033[1;31mUnrecognized input file format!\033[0m" << std::endl;
            error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
        }
    } else {
        std::cout << "\033[1;31mUnrecognized input file format!\033[0m" << std::endl;
        error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
    }
    if (!outputFormat.empty()) {
        if (outputFormat == "msa") { // because format gets read backwards
            jaclangToNasm = join(outputFileName, "asm");
            nasmToLinker = "";
            binaryOutput = "";
        } else if (outputFormat == "o") {
            nasmToLinker = join(outputFileName, "o");
            binaryOutput = "";
        } else {
            std::cout << "\033[1;31mUnrecognized output file format!\033[0m" << std::endl;
            error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
        }
    } else
        binaryOutput = outputFileName;
}

void compile_assembly() {
    std::string command = OS_NASM;
    command += jaclangToNasm;
    command += " -o ";
    command += nasmToLinker;
    system(command.c_str());
}

void link_object() {
    std::string command = OS_LINKER; // link assembly code
    command += binaryOutput;
    command += " ";
    command += nasmToLinker;
    system(command.c_str());
}

std::string getFormat(std::string &file) {
    std::string format;
    if (contains(file, '.'))
        for (unsigned long i = file.length() - 1; file.at(i) != '.'; i--)
            format += file.at(i);
    return format;
}

void create_cache_dir() {
    if (stat(cacheDir.c_str(), &info) != 0)
        cacheDirExisted = false;
    else if (info.st_mode & S_IFDIR)
        cacheDirExisted = true;
    else {
        std::cout << "There is a file named " << cacheDir << "! ABORTING..." << std::endl;
        error::terminate("CACHE ERROR", ERROR_CACHE_ERROR);
    }

    if (!cacheDirExisted) {
        std::string command = "mkdir ";
        command += cacheDir;
        system(command.c_str());
    }
}

void remove_cache_dir(bool exitSuccess) {
    if (!cacheDirExisted) {
        stat(cacheDir.c_str(), &info);
        if (info.st_mode & S_IFDIR) {
            std::string command = "rm -r ";
            command += cacheDir;
            system(command.c_str());
        }
    } else if (exitSuccess) {
        if (jaclangToNasm.rfind(cacheDir + "/", 0) == 0)
            remove(jaclangToNasm.c_str());
        if (nasmToLinker.rfind(cacheDir + "/", 0) == 0)
            remove(nasmToLinker.c_str());
    }
}

#include "jaclang.h"

#include <chrono>   // time
#include <sys/stat.h> // check if file/directory exists

struct stat info;

bool cacheDirExisted;
std::string cacheDir;

// the main file, where the main loop is happening, and also where the file members are defined

unsigned int file::inputLineCount = 0; // number of lines in input file

bool debug = false; // if compilation is being debugged (default: false)

unsigned long file::asm_data; // integers for tracking sections in asm file
unsigned long file::asm_bss;
unsigned long file::asm_text;
unsigned long file::asm_func;

std::string file::inputText; // input file
std::vector<std::string> file::outputVector; // prefix for asm file

std::string jaclangInput;
std::string jaclangToNasm;
std::string nasmToLinker;
std::string binaryOutput;

long start;

// help text (if no arguments provided
#define HELP_TEXT \
"Jaclang help: \n"\
"usage:\n"\
"	jaclang - for help\n"\
"	jaclang [option] - for misc\n"\
"	jaclang [input file] [output file] - for compilation\n"\
"\n"\
"options:\n"\
"	-d - debug - get more detailed compilation (for nerds)\n"\
"	-k - keep  - keep the assembly file\n"\
"\n"\
"misc options:\n"\
"	version    - check the version name and id\n"\
"	versionid  - show only version id\n"\
"	versionstr - show only version name\n"\

void init();

void start_timer();
void end_timer();
void handle_arguments(int argc, char **argv);

void compile_jaclang(const std::string& jaclangInput, const std::string& jaclangToNasm2);
void compile_assembly(const std::string& inputFile, const std::string& outputFile);
void link_object(const std::string& inputFile, const std::string& outputFile);

void create_cache_dir();
void remove_cache_dir(bool exitSuccess);

std::string getFormat(std::string& file);

std::vector<std::string> args; // vector of command line arguments
std::string ops;         // vector of command line options

std::string join(const std::string& filename, const std::string& end)
{
    return filename + std::string(".") + end;
}

int main(int argc, char **argv)
{
    init();
    if(!contains(ops, 'q'))
        start_timer();

    handle_arguments(argc, argv);
    
    create_cache_dir();
    
    if(!jaclangInput.empty() && !jaclangToNasm.empty())
        compile_jaclang(jaclangInput, jaclangToNasm);
    if(!jaclangToNasm.empty() && !nasmToLinker.empty())
        compile_assembly(jaclangToNasm, nasmToLinker);
    if(!nasmToLinker.empty() &&!binaryOutput.empty())
        link_object(nasmToLinker, binaryOutput);
	
    if(!contains(ops, 'q'))
        std::cout << "\033[1;32mCompilation successful!\033[0m" << std::endl; // compilation successful!
    
    if(!contains(ops, 'k'))
        remove_cache_dir(true);
    
	if(!contains(ops, 'q'))
        end_timer();

	return 0; // exit success
}

void compile_jaclang(const std::string& jaclangInput2, const std::string& jaclangToNasm2)
{
    file::read(jaclangInput); // Read file

    lexer::main(); // convert code into tokens
    parser::main(jaclangInput); // convert tokens into syntax tree
    if (debug)
        printAST(mainBranch);
    currentBranchScope = &mainBranch;
    generator::main(); // generate assembly code out of syntax tree

    file::write(jaclangToNasm2); // Writes to file
}

void start_timer()
{
    using namespace std::chrono;
    start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); // get current time in milliseconds for timing compilation
}

void end_timer()
{
    using namespace std::chrono;
    long end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); // end timer
    std::string ms = std::to_string((end - start) % 1000); // get milliseconds time difference
    unsigned long len = ms.length(); // add zeros to have 4 char string for milliseconds
    for(unsigned int i = 4; i > len; i--)
        ms.insert(ms.begin(), '0'); // insert zeros

    std::cout << "Compilation time: " << (end - start) / 1000 << "." << ms << " s" << std::endl; // tell compilation time
}

void handle_arguments(int argc, char **argv)
{
    const char allowedOptions[] = { // all allowed options
            'd', // debug
            'k', // keep cache
            'q', // be quiet (still show errors)
    };

    for(int i = 1; i < argc; i++) // go through arguments
    {
        if(argv[i][0] == '-') // if option
        {
            for(int i2 = 1; argv[i][i2] != 0; i2++) // go through options
            {
                if(find(allowedOptions, argv[i][i2]) == 3) // if option is not valid
                {
                    std::cout << "\033[1;31m" << argv[i][i2] << " is not a valid argument!\033[0m" << std::endl; // error
                    error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
                }
                ops.push_back(argv[i][i2]); // else append option
            }
        }
        else // if not option
            args.emplace_back(argv[i]); // append it to args
    }

    if(args.empty()) // if there are no arguments
    {
        std::cout << HELP_TEXT << std::endl; // print help text
        exit(0);
    }

    else if(args.size() == 1) // if there is misc option
    {
        if(args.at(0) == "version") // check for every misc option
            std::cout << VERSION_STR << " ID:" << VERSION_INT << std::endl;
        else if(args.at(0) == "versionid")
            std::cout << VERSION_INT << std::endl;
        else if(args.at(0) == "versionstr")
            std::cout << VERSION_STR << std::endl;
        else
        {
            std::cout << "\033[1;31mInvalid option: " << args.at(0) << "\033[0m" << std::endl;
            error::terminate("INVALID OPTION", ERROR_INVALID_OPTION);
        }
        exit(0);
    }
    else if (args.size() > 2) // if there is more args than 2
    {
        std::cout << "\033[1;31mMust input 2 arguments or less!\033[0m" << std::endl;
        error::terminate("TOO MANY ARGUMENTS", ERROR_ARGUMENT_COUNT);
    }
    
    std::string outputFileName = args.at(1);
    std::string outputFormat = getFormat(args.at(1));
    std::string inputFormat = getFormat(args.at(0));
    if(!outputFormat.empty())
    {
        for(;outputFileName.at(outputFileName.size()-1) != '.';)
            outputFileName.pop_back();
        outputFileName.pop_back();
    }
    if(!inputFormat.empty())
    {
        if(inputFormat == "lj") // because format gets read backwards
        {
            jaclangInput = args.at(0);
            jaclangToNasm = cacheDir + "/" + join(outputFileName, "asm");
            nasmToLinker = cacheDir + "/" + join(outputFileName, "o");
            binaryOutput = cacheDir + "/" + outputFileName;
        }
        else if(inputFormat == "msa")
        {
            jaclangToNasm = args.at(0);
            nasmToLinker = cacheDir + "/" + join(outputFileName, "o");
            binaryOutput = cacheDir + "/" + outputFileName;
        }
        else if(inputFormat == "o")
        {
            nasmToLinker = join(outputFileName, "o");
            binaryOutput = cacheDir + "/" + outputFileName;
        }
        else
        {
            std::cout << "\033[1;31mUnrecognized input file format!\033[0m" << std::endl;
            error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
        }
    }
    else
    {
        std::cout << "\033[1;31mUnrecognized input file format!\033[0m" << std::endl;
        error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
    }
    if(!outputFormat.empty())
    {
        if(outputFormat == "msa") // because format gets read backwards
        {
            jaclangToNasm = join(outputFileName, "asm");
            nasmToLinker = "";
            binaryOutput = "";
        }
        else if(outputFormat == "o")
        {
            nasmToLinker = join(outputFileName, "o");
            binaryOutput = "";
        }
        else
        {
            std::cout << "\033[1;31mUnrecognized output file format!\033[0m" << std::endl;
            error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
        }
    }
    else
        binaryOutput = outputFileName;
    if(contains(ops, 'd'))
        debug = true;
}

void compile_assembly(const std::string& inputFile, const std::string& outputFile)
{
    std::string command = OS_NASM;
    command += inputFile;
    command += " -o ";
    command += outputFile;
    system(command.c_str());
}

void link_object(const std::string& inputFile, const std::string& outputFile)
{
    std::string command = OS_LINKER; // link assembly code
    command += outputFile;
    command += " ";
    command += inputFile;
    system(command.c_str());
}

std::string getFormat(std::string& file)
{
    std::string format;
    if(contains(file, '.'))
        for (unsigned long i = file.length() - 1; file.at(i) != '.'; i--)
            format += file.at(i);
    return format;
}

void create_cache_dir()
{
    if(stat(cacheDir.c_str(), &info) != 0)
        cacheDirExisted = false;
    else if(info.st_mode & S_IFDIR)
        cacheDirExisted = true;
    else
    {
        std::cout << "There is a file named " << cacheDir << "! ABORTING..." << std::endl;
        error::terminate("CACHE ERROR", ERROR_CACHE_ERROR);
    }
    
    if(!cacheDirExisted)
    {
        std::string command = "mkdir ";
        command += cacheDir;
        system(command.c_str());
    }
}

void remove_cache_dir(bool exitSuccess)
{
    if(!cacheDirExisted)
    {
        stat(cacheDir.c_str(), &info);
        if(info.st_mode & S_IFDIR)
        {
            std::string command = "rm -r ";
            command += cacheDir;
            system(command.c_str());
        }
    }
    else if(exitSuccess)
    {
        if(jaclangToNasm.rfind(cacheDir + "/", 0) == 0)
            remove(jaclangToNasm.c_str());
        if(nasmToLinker.rfind(cacheDir + "/", 0) == 0)
            remove(nasmToLinker.c_str());
    }
}

#include "jaclang.h"

#include <fstream>

void init() // initialize global variables
{
    jaclangInput = "";
    jaclangToNasm = "";
    nasmToLinker = "";
    binaryOutput = "";

    cacheDir = ".jlcache";

    generator::availableRegisters[0] = {
            "sil",
            "dil",
            "r8b",
            "r9b",
            "r10b",
            "r11b",
            "r12b",
            "r13b",
            "r14b",
            "r15b",
    };
    generator::availableRegisters[1] = {
            "si",
            "di",
            "r8w",
            "r9w",
            "r10w",
            "r11w",
            "r12w",
            "r13w",
            "r14w",
            "r15w",
    };
    generator::availableRegisters[2] = {
            "esi",
            "edi",
            "r8d",
            "r9d",
            "r10d",
            "r11d",
            "r12d",
            "r13d",
            "r14d",
            "r15d",
    };
    generator::availableRegisters[3] = {
            "rsi",
            "rdi",
            "r8",
            "r9",
            "r10",
            "r11",
            "r12",
            "r13",
            "r14",
            "r15",
    };
#if OS_TYPE == 0 // Linux
    std::string fileToRead = "/usr/local/bin/jaclang-data/empty-gnu.asm";
#elif OS_TYPE == 1 // MACOS
    std::string fileToRead = "/usr/local/bin/jaclang-data/empty-macho.asm";
#endif
    std::ifstream readFile(fileToRead);

    std::string line;
    if(!readFile.is_open())
    {
        std::cout << "\033[1;31mCannot open empty assembly framework file!\033[0m" << std::endl; // file missing
        error::terminate("DATA MISSING OR CORRUPTED", ERROR_DATA_ERROR);
    }
    while(std::getline(readFile, line))
        file::outputVector.push_back(line);

    parser::scopes = {&mainBranch};

#define find(x) find(file::outputVector, x)

    file::asm_data = find("section .data")     + 1; // locate each section
    file::asm_bss  = find("section .bss")      + 1;
    file::asm_text = find("section .text")     + 4;
    file::asm_func = file::outputVector.size();

#undef find
}

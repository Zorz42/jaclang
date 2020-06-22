#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#include <fstream>

void init() { // initialize global variables
    asm_::available_registers[0] = {
            "bl",
            "cl",
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
    asm_::available_registers[1] = {
            "bx",
            "cx",
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
    asm_::available_registers[2] = {
            "ebx",
            "ecx",
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
    asm_::available_registers[3] = {
            "rbx",
            "rcx",
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
    
    // Read empty frame file
#if OS_TYPE == 0 // Linux
    std::string file_to_read = "/usr/local/share/jaclang-data/empty-gnu.s";
#elif OS_TYPE == 1 // MACOS
    std::string file_to_read = "/usr/local/share/jaclang-data/empty-macho.s";
#endif
    std::ifstream read_file(file_to_read);

    std::string line;
    if(!read_file.is_open()) {
        std::cout << "\033[1;31mCannot open empty assembly frame file!\033[0m" << std::endl; // file missing
        error::terminate("DATA MISSING OR CORRUPTED", Err_Data_Error);
    }
    while(std::getline(read_file, line))
        file::output_vector.push_back(line);

    parser::scopes = {&parser::main_branch};

#define FIND(x) find(file::output_vector, x)

    file::asm_data = FIND(OS_SECTION_DATA) + 1; // locate each section
    file::asm_bss = FIND(OS_SECTION_BSS) + 1;
    file::asm_text = FIND(OS_SECTION_TEXT) + 4;
    file::asm_func = file::output_vector.size();

#undef FIND
    // define all primitive datatypes and their sizes and also their matches
    
    generator::primitive_datatype_sizes.reserve(5);
    generator::primitive_datatype_sizes["void"] = 0;
    generator::primitive_datatype_sizes["char"] = 1;
    generator::primitive_datatype_sizes["short"] = 2;
    generator::primitive_datatype_sizes["int"] = 4;
    generator::primitive_datatype_sizes["long"] = 8;
    generator::primitive_datatypes = {
            "void",
            "char",
            "short",
            "int",
            "long",
    };
    generator::operator_matches.reserve(2);
    generator::implicit_conversations.reserve(generator::primitive_datatypes.size());
    for(const std::string &primitive_datatype : generator::primitive_datatypes) {
        for(const std::string &curr_operator : {"+", "-", "*", "/", "==", "<", ">"})
            generator::operator_matches.push_back(DatatypeMatches(primitive_datatype + curr_operator, {Match({primitive_datatype, primitive_datatype})}));
        generator::implicit_conversations[primitive_datatype].reserve(generator::primitive_datatypes.size() - 1);
        for(const std::string &primitiveDatatype2 : generator::primitive_datatypes)
            if(primitive_datatype != primitiveDatatype2)
                generator::implicit_conversations[primitive_datatype].push_back(primitiveDatatype2);
    }

    generator::size_keywords.reserve(4);
    generator::size_keywords[1] = "b";
    generator::size_keywords[2] = "w";
    generator::size_keywords[4] = "l";
    generator::size_keywords[8] = "q";

    lexer::keywords = {
        "return",
        "if",
    };
}




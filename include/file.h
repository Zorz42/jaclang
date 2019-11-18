#pragma once

namespace file
{
	extern unsigned long asm_data;
	extern unsigned long asm_bss;
	extern unsigned long asm_text;
	extern unsigned long asm_func;
	
	extern std::string inputText; // Text of the input file
	extern unsigned int inputLineCount; // How many lines does the input file have
	extern std::string input;  // Path to input file
	extern std::string output; // Path to output file
	extern std::vector<std::string> outputVector; // Vector of lines of code (output file)
	
	void read(std::string text); // Read input file and put result in inputFileVector
	void write(std::string output); // When finished compiling, c++ code will be on outputFileVector. Transfer it to output file

	std::string getLine(int LINE); // Get text of line
	
	void append_data(std::string line);
	void append_bss(std::string line);
	void append_text(std::string line);
	void append_func(std::string line);
	void append(std::string line);
	void add(std::string line, unsigned long position);
	void add(std::string line);
	
	void append_instruction(std::string instruction, std::string arg1="", std::string arg2="");
}

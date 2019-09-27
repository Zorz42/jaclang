#pragma once

namespace file
{
	extern int asm_data;
	extern int asm_bss;
	extern int asm_text;
	
	extern std::string inputText; // Text of the input file
	extern int inputLineCount; // How many lines does the input file have
	extern std::string input;  // Path to input file
	extern std::string output; // Path to output file
	extern std::vector<std::string> outputVector; // Vector of lines of code (output file)
	
	void read(); // Read input file and put result in inputFileVector
	void write(); // When finished compiling, c++ code will be on outputFileVector. Transfer it to output file

	std::string getLine(int LINE); // Get text of line
	
	void append_data(std::string line);
	void append_bss(std::string line);
	void append_text(std::string line);
	void add(std::string line, int position);
	void add(std::string line);
	
	void append_instruction(std::string instruction, std::string arg1, std::string arg2);
}
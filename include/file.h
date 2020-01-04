#pragma once

namespace file
{
	extern unsigned long asm_data;
	extern unsigned long asm_bss;
	extern unsigned long asm_text;
	extern unsigned long asm_func;
	
	extern std::string inputText; // Text of the input file
	extern std::vector<std::string> outputVector; // Vector of lines of code (output file)
	
	void read(const std::string& text); // Read input file and put result in inputFileVector
	void write(const std::string& output); // When finished compiling, c++ code will be on outputFileVector. Transfer it to output file

	std::string getLine(int LINE); // Get text of line
	
	void append_data(const std::string& line);
	void append_bss(const std::string& line);
	void append_text(const std::string& line);
	void append_func(const std::string& line);
	void append(const std::string& line);
	void add(const std::string& line, unsigned long position);
	//void add(const std::string& line);
	
	void append_instruction(const std::string& instruction, const std::string& arg1="", const std::string& arg2="");
}

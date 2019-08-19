#include "jaclang.h"
// FILE

int file::inputLineCount = 0;

int file::asm_data = 1; 
int file::asm_bss = 3;  
int file::asm_text = 9;

std::string file::input;
std::string file::output;

std::string file::inputText;
std::vector<std::string> file::outputVector;

int main(int argc, char **argv)
{
	if (argc < 3) // At least two arguments - input and output file
	{
		std::cout << "\033[1;31mMust input at least 2 arguments!\033[0m" << std::endl;
		error::terminate("NOT ENOUGH ARGUMENTS", ERROR_ARGUMENT_COUNT);
	}
	
	std::vector<std::string> args;
	
	for(int i = 3; i < argc; i++)
		args.push_back(argv[i]);
	
	
	file::input = argv[1];  // argv[1] first argument
	file::output = argv[2]; // argv[2] second argument
	
	
	file::read(); // Read file
	
	file::add("section .data"); // data section
	file::add("");
	file::add("section .bss");  // bss section
	file::add("");
	file::add("section .text"); // text section
	file::add("	global _start"); // for linker
	file::add("_start:");
	file::add("	mov rbp, rsp"); // set stack
	file::add("");
	file::add("");
	file::add("	mov eax, 1");  // sys exit
	file::add("	int 0x80");  // call kernel
	
	lexer::main(); // convert code into tokens
	parser::main(); // convert tokens into syntax tree
	printAST(mainBranch);
	generator::main(); // generate assembly code out of syntax tree
	
	file::write(); // Writes to file
	
	std::string command = "nasm -f elf64 "; // compile assembly code
	command += file::output;
	command += ".asm";
	system(command.c_str());
	
	command = "ld -m elf_x86_64 -s -o "; // link assembly code
	command += file::output;
	command += " ";
	command += file::output;
	command += ".o";
	system(command.c_str());
	
	bool remove_cache = true; // cache = assembly code
	
	if(contains(args, "keep")) // if argument is keep, then keep the cache (aka assembly code)
		if(argv[3] != "keep")
			remove_cache = false;
	
	if(remove_cache) // remove cache if needed
	{
		command = "rm ";
		command += file::output;
		command += ".asm";
		system(command.c_str());
	}
	
	command = "rm "; // remove object file
	command += file::output;
	command += ".o";
	system(command.c_str());
	
	std::cout << "\033[1;32mCompilation sucsessful!\033[0m" << std::endl; // Yay, compilation sucsessful!
	
	if(!contains(args, "norun")) // also run file if not specified not to (confusing)
	{
		std::string command = "./";
		command += file::output;
		system(command.c_str());
	}
	
	return 0;
}

void file::read()
{
	std::ifstream inputFileObj(file::input); // Open file and store it in std::ifstream object
	if(!inputFileObj.is_open()) // If didnt open (file missing,...)
	{
		std::cout << "Unable to open file!" << std::endl;
		return;
	}
	//                              |||| String for iteration
	std::string line; //            VVVV
	while(std::getline(inputFileObj,line)) // Iterate through lines of input file
	{
		// preprocessor
		bool lineComment = false;
		for(int i = 0; i < line.length(); i++)
		{
			if(i < line.length() - 3) // if not in the last 3 characters of line
				if(line[i] == '/' && line[i + 1] == '/' && line[i + 2] == '/') // if triple slash
					lineComment = true; // then there is comment
			if(!lineComment) // character is not in comment
				file::inputText += line[i]; // Adds line of code to the output
		}
		file::inputText += '\n'; // Adds line of code to the output
		file::inputLineCount++;  // Adds one line count
	}
	
	file::inputText += " "; // So that lexer stores the last token for sure
	
	inputFileObj.close(); // Close the file - file have been read
}

void file::write()
{
	std::ofstream outputFileObj(file::output + ".asm"); // Open file (or create)
	if (outputFileObj.is_open()) // If file was opened (or created)
	{
		for (std::vector<std::string>::iterator t=file::outputVector.begin(); t!=file::outputVector.end(); ++t) // Add line from vector 
			outputFileObj << *t << "\n"; // Add new line so that the code wont be in the same line
	}
	else std::cout << "Unable to write to file!" << std::endl; // If file failed to open (or create)
	
	outputFileObj.close(); // Close file, c++ code has been written
}

void file::append_data(std::string line)
{
	file::add(line, file::asm_data); // append text to data section
	file::asm_data++;
	file::asm_bss++;
	file::asm_text++;
}

void file::append_bss(std::string line)
{
	file::add(line, file::asm_bss); // append text to bss section
	file::asm_bss++;
	file::asm_text++;
}

void file::append_text(std::string line)
{
	file::add(line, file::asm_text); // append text to text section
	file::asm_text++;
}

void file::add(std::string line, int position)
{
	file::outputVector.insert(file::outputVector.begin() + position, line); // Insert line of code into asm file
}

void file::add(std::string line)
{
	file::outputVector.push_back(line); // Insert line of code into asm file
}

std::string file::getLine(int LINE) // get line of code
{
	int currentLine = 1;
	std::string currentString = "";
	int i = 0;
	for(; currentLine != LINE; i++) // until we are in the line desired
	{
		if(file::inputText.at(i) == '\n')
			currentLine++; // if '\n' (newline) then it is next line
	}
	
	for(;file::inputText.at(i) != '\n'; i++) // when desired line, read line until '\n'
	{
		currentString += file::inputText.at(i);
	}
	
	return currentString;
}

void file::append_instruction(std::string instruction, std::string arg1, std::string arg2)
{
	std::string expr; // returns asm instruction: instruction arg1, arg2
	expr = "	";
	expr += instruction;
	expr += " ";
	expr += arg1;
	expr += ", ";
	expr += arg2;
	file::append_text(expr);
}

std::string onStack(int offset)
{
	return "[rbp-" + std::to_string(offset) + "]"; // returns position on stack: [rbp-offset]
}

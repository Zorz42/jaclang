#include "jaclang.h"

// the main file, where the main loop is happening, and also where the file members are defined

int file::inputLineCount = 0;

bool debug = false;
std::streambuf* orig_buf;

int file::asm_data = 1; 
int file::asm_bss = 3;  
int file::asm_text = 8;

std::string file::input;
std::string file::output;

std::string file::inputText;
std::vector<std::string> file::outputVector;

int main(int argc, char **argv)
{
	using namespace std::chrono;
	long start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	
	orig_buf = std::cout.rdbuf();
	if (argc < 3) // At least two arguments - input and output file
	{
		coutn << "\033[1;31mMust input at least 2 arguments, input and output file!\033[0m" << std::endl;
		error::terminate("NOT ENOUGH ARGUMENTS", ERROR_ARGUMENT_COUNT);
	}
	
	file::input = argv[1]; 
	file::output = argv[2];
	
	if(file::input.length() > 3)
	{
		if( !(
			file::input[file::input.length()-3] == '.' &&
			file::input[file::input.length()-2] == 'j' &&
			file::input[file::input.length()-1] == 'l'
		) )
		{
			coutn << "\033[1;31mUnrecognized input file format!\033[0m" << std::endl;
			error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
		}
	}
	else
	{
		coutn << "\033[1;31mUnrecognized input file format!\033[0m" << std::endl;
		error::terminate("INVALID FORMAT", ERROR_INVALID_FORMAT);
	}
	
	std::vector<std::string> args;
	
	for(int i = 3; i < argc; i++)
		args.push_back(argv[i]); // set args vector
	
	if(contains(args, "debug"))
		debug = true;
	
	file::read(); // Read file
	
	file::add("section .data"); // data section
	file::add("");
	file::add("section .bss");  // bss section
	file::add("");
	file::add("section .text"); // text section
	file::add("	global _start"); // for linker
	file::add("_start:");
	file::add("");
	file::add("");
	file::add("	mov eax, 1");  // sys exit
	file::add("	int 0x80");  // call kernel
	
	lexer::main(); // convert code into tokens
	parser::main(); // convert tokens into syntax tree
	if(debug)
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
	
	if(!contains(args, "keep")) // remove cache if needed
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
	
	coutn << "\033[1;32mCompilation sucsessful!\033[0m" << std::endl; // compilation sucsessful!
	
	long end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	std::string ms = std::to_string((end - start) % 1000);
	int len = ms.length();
	for(int i = 4; i > len; i--)
		ms.insert(ms.begin(), '0');
	coutn << "Compilation time: " << (end - start) / 1000 << "." << ms << " s" << std::endl;
	
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
	preprocessor::main(file::input);
}

void file::write()
{
	std::string command = "touch ";
	command += file::output;
	command += ".asm";
	system(command.c_str()); // create file
	std::ofstream outputFileObj(file::output + ".asm"); // Open file (or create)
	if (outputFileObj.is_open()) // If file was opened (or created)
		for (std::vector<std::string>::iterator t=file::outputVector.begin(); t!=file::outputVector.end(); ++t) // Add line from vector 
			outputFileObj << *t << "\n"; // Add new line so that the code wont be in the same line
	
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
		if(file::inputText.at(i) == '\n')
			currentLine++; // if '\n' (newline) then it is next line
	
	for(;file::inputText.at(i) != '\n'; i++) // when desired line, read line until '\n'
		currentString += file::inputText.at(i);
	
	return currentString;
}

void file::append_instruction(std::string instruction, std::string arg1, std::string arg2)
{
	std::string expr; // returns asm instruction: instruction arg1, arg2
	expr = "	";
	expr += instruction;
	expr += " ";
	expr += arg1;
	if(arg2.c_str())
		expr += ", ";
	expr += arg2;
	file::append_text(expr);
}
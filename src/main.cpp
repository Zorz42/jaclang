#include "jaclang.h"

// the main file, where the main loop is happening, and also where the file members are defined

int file::inputLineCount = 0;

bool debug = false;
std::streambuf* orig_buf;

int file::asm_data; 
int file::asm_bss;  
int file::asm_text;

std::string file::input;
std::string file::output;

std::string file::inputText;
std::vector<std::string> file::outputVector = {
	"section .data", // data section
	"",
	"section .bss",  // bss section
	"",
	"section .text", // text section
	"	global _start", // for linker
	"_start:",
	"",
	"",
	"	mov eax, 1",  // sys exit
	"	int 0x80",
	};

const std::string helpText =
"Jaclang help: \n"
"usage:\n"
"	jaclang - for help\n"
"	jaclang [option] - for misc\n"
"	jaclang [input file] [output file] - for compilation\n"
"\n"
"options:\n"
"	-n - norun - do not run file after compilation\n"
"	-d - debug - get more detailed compilation (for nerds)\n"
"	-k - keep  - keep the assembly file\n"
"\n"
"misc options:\n"
"	version   - check the version\n"
"	uninstall - remove jaclang"
;

int main(int argc, char **argv)
{
	using namespace std::chrono;
	long start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	
	std::vector<std::string> args;
	std::vector<char> ops;
	const char allowedOptions[] = {
		'd', // debug
		'k', // keep
	};
	
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			for(int i2 = 1; argv[i][i2] != 0; i2++)
			{
				if(find(allowedOptions, argv[i][i2]) == 3)
				{
					std::cout << "\033[1;31m" << argv[i][i2] << " is not a valid argument!\033[0m" << std::endl;
					error::terminate("INVALID ARGUMENT", ERROR_INVALID_ARGUMENT);
				}
				ops.push_back(argv[i][i2]);
			}
		}
		else
			args.push_back(argv[i]);
	}
	
	if(args.size() == 0)
	{
		std::cout << helpText << std::endl;
		return 0;
	}
	
	else if(args.size() == 1)
	{
		if(args.at(0) == "version")
			std::cout << VERSION_STR << " ID:" << VERSION_INT << std::endl;
		else if(args.at(0) == "uninstall")
		{
			system("sudo rm /usr/local/bin/jaclang");
			std::cout << "Jaclang sucsessfully removed!" << std::endl;
		}
		else if(args.at(0) == "versionid")
			std::cout << VERSION_INT << std::endl;
		else if(args.at(0) == "versionstr")
			std::cout << VERSION_STR << std::endl;
		else
		{
			std::cout << "\033[1;31mInvalid option: " << args.at(0) << "\033[0m" << std::endl;
			error::terminate("INVALID OPTION", ERROR_INVALID_OPTION);
		}
		return 0;
	}
	else if (args.size() > 2)
	{
		std::cout << "\033[1;31mMust input 2 arguments or less!\033[0m" << std::endl;
		error::terminate("TOO MANY ARGUMENTS", ERROR_ARGUMENT_COUNT);
	}
	
	
	file::input = args.at(0); 
	file::output = args.at(1);
	
	if(file::input.length() > 3)
	{
		if( !(
			file::input[file::input.length()-3] == '.' &&
			file::input[file::input.length()-2] == 'j' &&
			file::input[file::input.length()-1] == 'l'
		) )
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
	
	if(contains(ops, 'd'))
		debug = true;
	
	file::read(); // Read file
	
	#define find(x) find(file::outputVector, x)
	
	file::asm_data = find("section .data") + 1;
	file::asm_bss  = find("section .bss")  + 1;
	file::asm_text = find("section .text") + 4;
	
	#undef find
	
	lexer::main(); // convert code into tokens
	parser::main(); // convert tokens into syntax tree
	if(debug)
		printAST(mainBranch);
	generator::main(); // generate assembly code out of syntax tree
	
	file::write(); // Writes to file
	
	std::string command = "nasm -f elf64 "; // compile assembly code
	command += file::output;
	command += ".asm";
	command += " && ";
	command += "ld -m elf_x86_64 -s -o "; // link assembly code
	command += file::output;
	command += " ";
	command += file::output;
	command += ".o";
	system(command.c_str());
	
	if(!contains(ops, 'k')) // remove asm file if needed
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
	
	std::cout << "\033[1;32mCompilation sucsessful!\033[0m" << std::endl; // compilation sucsessful!
	
	long end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	std::string ms = std::to_string((end - start) % 1000);
	int len = ms.length();
	for(int i = 4; i > len; i--)
		ms.insert(ms.begin(), '0');
	
	std::cout << "Compilation time: " << (end - start) / 1000 << "." << ms << " s" << std::endl;
	
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
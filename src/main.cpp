#include "jaclang.h"

// the main file, where the main loop is happening, and also where the file members are defined

unsigned int file::inputLineCount = 0; // number of lines in input file

bool debug = false; // if compilation is being debugged (default: false)

unsigned int file::asm_data; // integers for tracking sections in asm file
unsigned int file::asm_bss;  
unsigned int file::asm_text;
unsigned int file::asm_func;

std::string file::inputText; // input file
std::vector<std::string> file::outputVector = { // prefix for asm file
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
	""
	""
	};

// help text (if no argumets provided
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
"	uninstall  - remove jaclang"\

int main(int argc, char **argv)
{
	using namespace std::chrono;
	long start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); // get current time in miliseconds for timing compilation
	
	std::vector<std::string> args; // vector of command line arguments
	std::string ops;         // vector of command line options
	const char allowedOptions[] = { // all allowed options
		'd', // debug
		'k', // keep
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
			args.push_back(argv[i]); // append it to args
	}
	
	if(args.size() == 0) // if there are no arguments
	{
		std::cout << HELP_TEXT << std::endl; // print help text
		return 0;
	}
	
	else if(args.size() == 1) // if there is misc option
	{
		if(args.at(0) == "version") // check for every misc option
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
	else if (args.size() > 2) // if there is more args than 2
	{
		std::cout << "\033[1;31mMust input 2 arguments or less!\033[0m" << std::endl;
		error::terminate("TOO MANY ARGUMENTS", ERROR_ARGUMENT_COUNT);
	}
	
	if(args.at(0).length() > 3)
	{
		if( !(
			args.at(0)[args.at(0).length()-3] == '.' &&
			args.at(0)[args.at(0).length()-2] == 'j' &&
			args.at(0)[args.at(0).length()-1] == 'l'
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
	
	file::read(args.at(0)); // Read file
	
	#define find(x) find(file::outputVector, x)
	
	file::asm_data = find("section .data")     + 1; // locate each section
	file::asm_bss  = find("section .bss")      + 1;
	file::asm_text = find("section .text")     + 4;
	file::asm_func = file::outputVector.size() - 1;
	
	#undef find
	
	lexer::main(); // convert code into tokens
	parser::main(args); // convert tokens into syntax tree
	if(debug)
		printAST(mainBranch);
	generator::main(); // generate assembly code out of syntax tree
	
	file::write(args.at(1)); // Writes to file
	
	std::string command = OS_NASM;
	command += " -w-all -f elf64 "; // compile assembly code
	command += args.at(1);
	command += ".asm";
	command += " && ";
	command += "ld -m elf_x86_64 -s -o "; // link assembly code
	command += args.at(1);
	command += " ";
	command += args.at(1);
	command += ".o";
	system(command.c_str());
	
	if(!contains(ops, 'k')) // remove asm file if needed
	{
		command = "rm ";
		command += args.at(1);
		command += ".asm";
		system(command.c_str());
	}
	
	command = "rm "; // remove object file
	command += args.at(1);
	command += ".o";
	system(command.c_str());
	
	std::cout << "\033[1;32mCompilation sucsessful!\033[0m" << std::endl; // compilation sucsessful!
	
	long end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); // end timer
	std::string ms = std::to_string((end - start) % 1000); // get miliseconds time difference
	int len = ms.length(); // add zeros to have 4 char string for miliseconds
	for(int i = 4; i > len; i--)
		ms.insert(ms.begin(), '0'); // insert zeros
	
	std::cout << "Compilation time: " << (end - start) / 1000 << "." << ms << " s" << std::endl; // tell compilation time
	
	return 0; // exit sucess
}

void file::read(std::string text) // read file
{
	preprocessor::main(text); // call preprocessor
}

void file::write(std::string output) // write to file
{
	std::string command = "touch "; // create file (if not existing)
	command += output;
	command += ".asm";
	system(command.c_str()); // create file
	std::ofstream outputFileObj(output + ".asm"); // open file (or create)
	if (outputFileObj.is_open()) // if file was opened (or created)
		for (std::vector<std::string>::iterator t=file::outputVector.begin(); t!=file::outputVector.end(); ++t) // add line from vector 
			outputFileObj << *t << "\n"; // add new line so that the code wont be in the same line
	
	outputFileObj.close(); // close file, c++ code has been written
}

void file::append_data(std::string line)
{
	file::add(line, file::asm_data); // append text to data section
	file::asm_data++;
	file::asm_bss++;
	file::asm_text++;
	file::asm_func++;
}

void file::append_bss(std::string line)
{
	file::add(line, file::asm_bss); // append text to bss section
	file::asm_bss++;
	file::asm_text++;
	file::asm_func++;
}

void file::append_text(std::string line)
{
	file::add(line, file::asm_text); // append text to text section
	file::asm_text++;
	file::asm_func++;
}

void file::append_func(std::string line)
{
	file::add(line, file::asm_func); // append text to text section
	file::asm_func++;
}


void file::add(std::string line, int position)
{
	file::outputVector.insert(file::outputVector.begin() + position, line); // insert line of code into asm file
}

void file::add(std::string line)
{
	file::outputVector.push_back(line); // append line of code into asm file
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
	if(arg2 != "")
		expr += ", ";
	expr += arg2;
	file::append(expr);
}

void file::append(std::string line)
{
	if(generator::inFunction) // if code should be in function append it to function section
		file::append_func(line);
	else
		file::append_text(line);
}
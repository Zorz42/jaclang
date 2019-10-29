// this file preprocesses the input code.

#include "jaclang.h"

void preprocessor::main(std::string text)
{
	std::ifstream inputFileObj(text); // open file and store it in std::ifstream object
	if(!inputFileObj.is_open()) // if didnt open (file missing,...)
	{
		std::cout << "\033[1;31mFile does not exist!\033[0m" << std::endl;
		error::terminate("UNABLE TO OPEN FILE", ERROR_UNABLE_TO_OPEN_FILE);
	}
	
	bool multilineComment = false; // if in multiline comment
	
	//                              |||| string for iteration
	std::string line; //            VVVV
	while(std::getline(inputFileObj,line)) // iterate through lines of input file
	{
		// preprocessor
		bool lineComment = false;
		for(int i = 0; i < line.length(); i++)
		{
			if(i < line.length() - 3) // if not in the last 3 characters of line
				if(line[i] == ';') // if triple slash
					lineComment = true; // then there is comment
			if(i < line.length() - 2 && !lineComment) // prevent out of range
				if(line[i] == '/' && line[i + 1] == '*') // if multiline comment starts
					multilineComment = true;
			if(line[i - 2] == '*' && line[i - 1] == '/' && !lineComment) // if multiline comment ends
				multilineComment = false;
			
			if(!lineComment && !multilineComment) // character is not in comment
				file::inputText += line[i]; // adds line of code to the output
			
		}
		if(!multilineComment) // character is not in multiline comment
			file::inputText += '\n'; // adds line of code to the output
		file::inputLineCount++;  // adds one line count
	}
	
	file::inputText += " "; // So that lexer stores the last token for sure
	
	inputFileObj.close(); // close the file - file have been read
}
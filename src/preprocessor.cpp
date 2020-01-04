// this file will preprocess the input code.

#include "jaclang.h"

#include <fstream>  // open file

void preprocessor::main(const std::vector<std::string>& rawInputFile)
{
    bool inMultilineComment = false;
    bool lineComment = false;
    std::string line;
    for(const std::string& line : rawInputFile)
	{
		// preprocessor
		for(unsigned long i = 0; i < line.length(); i++)
		{
			if(i < line.length() - 3) // if not in the last 3 characters of line
				if(line[i] == ';')
					lineComment = true;
			if(i < line.length() - 2 && !lineComment) // prevent out of range
				if(line[i] == '/' && line[i + 1] == '*') // if multiline comment starts
					inMultilineComment = true;
			if(line[i - 2] == '*' && line[i - 1] == '/' && !lineComment) // if multiline comment ends
				inMultilineComment = false;
			
			if(!lineComment && !inMultilineComment) // character is not in comment
				file::inputText += line[i]; // adds line of code to the output
			
		}
		if(!inMultilineComment) // character is not in multiline comment
			file::inputText += '\n'; // adds line of code to the output
        lineComment = false;
	}
	
	file::inputText += " "; // So that lexer stores the last token for sure
}

// this file preprocesses the input code.

#include "jaclang.h"

void preprocessor::main(std::string text)
{
	std::ifstream inputFileObj(text); // Open file and store it in std::ifstream object
	if(!inputFileObj.is_open()) // If didnt open (file missing,...)
	{
		coutn << "Unable to open file!" << std::endl;
		return;
	}
	
	bool multilineComment = false;
	
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
			if(i < line.length() - 2 && !lineComment)
				if(line[i] == '/' && line[i + 1] == '*')
					multilineComment = true;
			if(line[i - 2] == '*' && line[i - 1] == '/' && !lineComment)
				multilineComment = false;
			
			if(!lineComment && !multilineComment) // character is not in comment
				file::inputText += line[i]; // Adds line of code to the output
			
		}
		if(!multilineComment) // character is not in multiline comment
			file::inputText += '\n'; // Adds line of code to the output
		file::inputLineCount++;  // Adds one line count
	}
	
	file::inputText += " "; // So that lexer stores the last token for sure
	
	inputFileObj.close(); // Close the file - file have been read
}
// this file parses equations

#include "jaclang.h"

#define current lexer::tokens.at(parser::tokCount)

void optimize(branch& currentBranch, const bool& nested);

branch parser::calculation(bool nested) // parse calculation
{
    std::vector<std::string> equationSymbols = {"+", "-", "*", "/"}; // valid operators in calculation
	branch currentBranch; // current branch in operation
	currentBranch.name = "calc"; // sets current branch to calculation
	if(current.text == "-")
	{
		appendBranch("0", currentBranch);
		appendBranch("-", currentBranch);
		parser::tokCount++;
	}
	while(parser::tokCount != lexer::tokens.size())
    {
		if(parser::e::functionCall(currentBranch));
		else if(current.type == TYPE_CONST) // if its constant
            appendBranch(current.text, currentBranch);
		else if(current.type == TYPE_INDENT)
            appendBranch(":" + current.text, currentBranch);
		else if(current.text == "(") // then its nested within the operation
        {
            parser::tokCount++;
            branch obj = parser::calculation(true); // make equation until ')'
            appendBranch(obj, currentBranch);
        }
		else
            error::syntaxError("Value expected");
        parser::tokCount++;
        if(parser::tokCount == lexer::tokens.size())
            break;
        if(parser::tokCount + 1 == lexer::tokens.size())
            error::syntaxError("File ends with a operator");
        if(find(equationSymbols, current.text) != equationSymbols.size()) // if there is valid operator
            appendBranch(current.text, currentBranch);
        else
            break;
		parser::tokCount++;
	}

	if(parser::tokCount != lexer::tokens.size())
        if(current.text != ")" && parser::tokCount + 1 != lexer::tokens.size())
            parser::tokCount--;
	
	#define curr(x) currentBranch.sub.at(x).name
	#define currentObj(x) currentBranch.sub.at(x)
	#define eraseEl(x) currentBranch.sub.erase(currentBranch.sub.begin() + x)
	
	// make that multiplication and division get calculated first
	for(unsigned long i = 2; i < currentBranch.sub.size(); i += 2)
	{
		if(curr(i) == "*" || curr(i) == "/")
		{
			branch obj;
			obj.name = "calc";
			appendBranch(currentObj(i - 1), obj);
			eraseEl(i - 1);
			i--;
			while(i < currentBranch.sub.size() && (curr(i) == "*" || curr(i) == "/"))
			{
				appendBranch(currentObj(i), obj);
				appendBranch(currentObj(i + 1), obj);
				
				eraseEl(i);
				eraseEl(i);
			}
			
			optimize(obj, true);
			
			currentBranch.sub.insert(currentBranch.sub.begin() + i, obj);
			i--;
		}
	}
	
	optimize(currentBranch, nested);
	
	return currentBranch;
}

void optimize(branch& currentBranch, const bool& nested)
{
    for(unsigned int i = 0; i + 2 <= currentBranch.sub.size(); i += 2)
	{
		if(isInt(currentBranch.sub.at(i).name) && isInt(currentBranch.sub.at(i + 2).name))
		{
			if(currentBranch.sub.at(i + 1).name == "+")
				currentBranch.sub.at(i).name = std::to_string(std::stoi(currentBranch.sub.at(i).name) + std::stoi(currentBranch.sub.at(i + 2).name));
			else if(currentBranch.sub.at(i + 1).name == "-")
				currentBranch.sub.at(i).name = std::to_string(std::stoi(currentBranch.sub.at(i).name) - std::stoi(currentBranch.sub.at(i + 2).name));
			else if(currentBranch.sub.at(i + 1).name == "*")
				currentBranch.sub.at(i).name = std::to_string(std::stoi(currentBranch.sub.at(i).name) * std::stoi(currentBranch.sub.at(i + 2).name));
			else if(currentBranch.sub.at(i + 1).name == "/")
				currentBranch.sub.at(i).name = std::to_string(std::stoi(currentBranch.sub.at(i).name) / std::stoi(currentBranch.sub.at(i + 2).name));
			currentBranch.sub.erase(currentBranch.sub.begin() + i + 1);
			currentBranch.sub.erase(currentBranch.sub.begin() + i + 1);
			i -= 2;
		}
	}
	if(currentBranch.sub.size() == 1 && nested && isInt(currentBranch.sub.at(0).name))
	{
		branch obj = currentBranch.sub.at(0);
		currentBranch = obj;
	}
}


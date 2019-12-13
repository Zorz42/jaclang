// this file parses equations

#include "jaclang.h"

#define current lexer::tokens.at(parser::tokCount)

branch optimize(branch currentBranch, bool nested);

void insertBranchAtBegin(std::string name, branch& target)
{
	branch obj;
	obj.name = std::move(name);
	target.sub.insert(target.sub.begin(), obj);
}
/*void insertBranchAtBegin(const branch& source, branch& target)
{
	target.sub.insert(target.sub.begin(), source);
}*/

branch parser::calculation(bool nested) // parse calculation
{
    std::vector<std::string> equationSymbols = {"+", "-", "*", "/"}; // valid operators in calculation
	bool timeForValue = true; // time for value is true following value, then is false following operator
	branch currentBranch; // current branch in operation
	currentBranch.name = "calc"; // sets current branch to calculation
	if(current.text == "-")
	{
		appendBranch("0", currentBranch);
		appendBranch("-", currentBranch);
		parser::tokCount++;
	}
	if(parser::e::functionCall(currentBranch))
	{
		insertBranchAtBegin("int", currentBranch);
		parser::tokCount++;
		timeForValue = false;
	}
	else if(current.type == TYPE_CONST || current.type == TYPE_INDENT) // if first value is number
	{
		if(contains(current.text, '.')) // if it has '.' in it then its float
			insertBranchAtBegin("float", currentBranch);
		else
			insertBranchAtBegin("int", currentBranch); // else its integer
	}
	else if(current.text == "(") // if its nested with () then its calculation within calculation
	{
		parser::tokCount++;
		branch obj = parser::calculation();
		parser::tokCount++;
		timeForValue = false;
		insertBranchAtBegin(obj.sub.at(0).name, currentBranch);
		appendBranch(obj, currentBranch);
	}
	else
        error::syntaxError("Value expected");


	while(true)
    {
	    if(parser::tokCount == lexer::tokens.size()) // Not crashing on unexpected EOF
	        break;
		if(parser::e::functionCall(currentBranch))
			timeForValue = false;
		else if(current.type == TYPE_CONST || current.type == TYPE_INDENT) // if its constant or string or variable
		{
			if(!timeForValue) // if isn't time for value
				break;
			std::string currentText = current.type == TYPE_INDENT ? ":" : "";
			currentText += current.text;
			appendBranch(currentText, currentBranch);
			timeForValue = false;
		}
		else if(find(equationSymbols, current.text) != equationSymbols.size())
		{ // if there is valid operator
			if(timeForValue) // if its time for value
				error::syntaxError("Value expected");
			appendBranch(current.text, currentBranch);
			timeForValue = true;
		}
		else if(current.text == "(") // then its nested within the operation
		{
			if(!timeForValue)
				break;
			timeForValue = false;
			parser::tokCount++;
			branch obj = parser::calculation(true); // make equation until ')'
			appendBranch(obj, currentBranch);
		}
		else if(current.type == TYPE_INDENT)
		{
			if(!timeForValue) // if isn't time for value
				error::syntaxError("Operator expected");
			appendBranch(current.text, currentBranch);
			timeForValue = false;
		}
		else
			break;
		
		if(parser::tokCount + 1 == lexer::tokens.size())
        {
            if (!timeForValue) {
                timeForValue = false;
                break;
            } // if calculation has come to the end of file without ending itself
            else
                error::syntaxError("Equation has no end");
        }
		parser::tokCount++;
	}
	if(timeForValue) // if it was a time for value
		error::syntaxError("Value expected");

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
			appendBranch("int", obj);
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
			
			obj = optimize(obj, true);
			
			currentBranch.sub.insert(currentBranch.sub.begin() + i, obj);
			i--;
		}
	}
	
	currentBranch = optimize(currentBranch, nested);
	
	return currentBranch;
}

branch optimize(branch currentBranch, bool nested)
{
	if(currentBranch.sub.size() == 2 && nested)
	{
		branch obj = currentBranch.sub.at(1);
		currentBranch = obj;
	}
	
	for(unsigned int i = 1; i + 2 <= currentBranch.sub.size(); i += 2)
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
	if(currentBranch.sub.size() == 2 && nested)
	{
		branch obj = currentBranch.sub.at(1);
		currentBranch = obj;
	}
	
	return currentBranch;
}


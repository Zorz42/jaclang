// this file parses equations

#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

std::vector<std::string> equationSymbols = {"+", "-", "*", "/"}; // valid operators in equation
branch optimize(branch currentBranch, bool nested);

void insertBranchAtBegin(std::string name, branch& target)
{
	branch obj;
	obj.name = name;
	target.sub.insert(target.sub.begin(), obj);
}
void insertBranchAtBegin(branch source, branch& target)
{
	target.sub.insert(target.sub.begin(), source);
}

branch parser::equation(std::string end, std::string end2, bool nested) // parse equation
{
	bool timeForValue = true; // time for value is true following value, then is false following operator
	branch currentBranch; // current branch in operation
	currentBranch.name = "/equation"; // sets current branch to equation
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
	else if(current.text == "(") // if its nested with () then its equation within equation
	{
		parser::tokCount++;
		branch obj = parser::equation(")");
		parser::tokCount++;
		timeForValue = false;
		insertBranchAtBegin(obj.sub.at(0).name, currentBranch);
		appendBranch(obj, currentBranch);
	}
	else
		error::syntaxError("Value expected");
	
	while(true)
	{
		if(parser::e::functionCall(currentBranch))
		{
			timeForValue = false;
		}
		else if(current.type == TYPE_CONST || current.type == TYPE_INDENT) // if its constant or string or variable
		{
			if(!timeForValue) // if isnt time for value
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
			branch obj = parser::equation(")", "\n", true); // make eqution until ')'
			appendBranch(obj, currentBranch);
		}
		else if(current.type == TYPE_INDENT)
		{
			if(!timeForValue) // if isnt time for value
				error::syntaxError("Operator expected");
			appendBranch(current.text, currentBranch);
			timeForValue = false;
		}
		else
			break;
		
		if(parser::tokCount + 1 == lexer::toks.size())
			if(timeForValue)
				error::syntaxError("Equation has no end"); // if equation has come to the end of file without ending itself
			else
			{
				timeForValue = false;
				break;
			}
		
		parser::tokCount++;
	}
	if(timeForValue) // if it was a time for value
		error::syntaxError("Value expected");
	
	if(current.text != ")" && parser::tokCount + 1 != lexer::toks.size())
		parser::tokCount--;
	
	#define current(x) currentBranch.sub.at(x).name
	#define currentObj(x) currentBranch.sub.at(x)
	#define eraseEl(x) currentBranch.sub.erase(currentBranch.sub.begin() + x)
	
	// make that multiplication and division get calculated first
	for(int i = 2; i < currentBranch.sub.size(); i += 2)
	{
		if(current(i) == "*" || current(i) == "/")
		{
			branch obj;
			obj.name = "/equation";
			appendBranch("int", obj);
			appendBranch(currentObj(i - 1), obj);
			eraseEl(i - 1);
			i--;
			while(i < currentBranch.sub.size() && (current(i) == "*" || current(i) == "/"))
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
	
	for(int i = 1; i + 2 <= currentBranch.sub.size(); i += 2)
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
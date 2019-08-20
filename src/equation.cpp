// this file parses equations

#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

std::string equationSymbols[4] = {"+", "-", "*", "/"}; // valid operators in equation

branch parser::equation(std::string end, std::string end2) // parse equation
{
	bool timeForValue = true; // time for value is true following value, then is false following operator
	branch currentBranch; // current branch in operation
	currentBranch.name = "/equation"; // sets current branch to equation
	if(current.type == TYPE_CONST) // if first value is number
	{
		if(contains(current.text, '.')) // if it has '.' in it then its float
			appendBranch("float", currentBranch);
		else
			appendBranch("int", currentBranch); // else its integer
	}
	else if(current.type == TYPE_STRING) // if its string
		appendBranch("string", currentBranch);
	else if(current.text == "(") // if its nested with () then its equation within equation
	{
		parser::tokCount++;
		branch obj = parser::equation(")");
		parser::tokCount++;
		timeForValue = false;
		appendBranch(obj.sub.at(0).name, currentBranch);
		appendBranch(obj, currentBranch);
	}
	else
		error::syntaxError("Value expected");
	
	while(current.text != end && current.text != end2 || timeForValue) // repeat until end token and it shouldnt be time for value eg. 10 + 10 +;
	{
		if(parser::tokCount + 1 == lexer::toks.size())
			error::syntaxError("Equation has no end"); // if equation has come to the end of file without ending itself
		
		else if(current.type == TYPE_CONST || current.type == TYPE_STRING) // if its constant or string
		{
			if(!timeForValue) // if isnt time for value
				error::syntaxError("Operator expected");
			appendBranch(current.text, currentBranch);
			timeForValue = false;
		}
		else if(std::find(std::begin(equationSymbols), std::end(equationSymbols), current.text) != std::end(equationSymbols))
		{ // if there is valid operator
			if(timeForValue) // if its time for value
				error::syntaxError("Value expected");
			appendBranch(current.text, currentBranch);
			timeForValue = true;
		}
		else if(current.text == "(") // then its nested within the operation
		{
			if(!timeForValue)
				error::syntaxError("Operator expected");
			timeForValue = false;
			parser::tokCount++;
			branch obj = parser::equation(")"); // make eqution until ')'
			appendBranch(obj, currentBranch);
		}
		else if(current.type == TYPE_INDENT)
		{
			if(!timeForValue) // if isnt time for value
				error::syntaxError("Operator expected");
			appendBranch(current.text, currentBranch);
			timeForValue = false;
		}
		else if(current.text != "\\n") // if token isn't newline, because operation can be in multiple lines
			error::syntaxError("Unexpected token");
		
		parser::tokCount++;
	}
	
	if(timeForValue) // if it was a time for value
		error::syntaxError("Value expected");
	
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
			
			currentBranch.sub.insert(currentBranch.sub.begin() + i, obj);
			i--;
		}
	}
	
	// optimisation
	if(currentBranch.sub.size() == 2) 
	{
		branch obj = currentBranch.sub.at(1);
		currentBranch = obj;
	} 
	
	return currentBranch;
}
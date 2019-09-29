// this generates system functions

#include "jaclang.h"

#define current currentBranchScope->sub.at(currentBranchScope->count) // function branch
#define currentName  currentBranchScope->sub.at(currentBranchScope->count).sub.at(0).name // name of function
#define currentName2 currentBranchScope->sub.at(currentBranchScope->count).sub.at(1).name

void f_asmtext();
void f_asmdata();
void f_asmbss();

void generator::e::systemFunctionCall() // system function: __test__
{
	if(currentName == "__asmtext__") // system functions
		f_asmtext();
	else if(currentName == "__asm__") // asm is alternative to asm text, because asmtext is the most common asm call
		f_asmtext();
	else if(currentName == "__asmdata__")
		f_asmdata();
	else if(currentName == "__asmbss__")
		f_asmbss();
	else
		error::treeError("Unknown system function call: " + currentName);
}

void generator::e::functionDeclaration()
{
	function obj;
	obj.name = currentName2;
	for(function i : generator::functionVector)
		if(i.name == obj.name)
			error::treeError("Function already declared!");
	generator::functionVector.push_back(obj);
	currentBranchScope->count++;
	std::vector<variable> prevStack = generator::stack;
	generator::stack = {};
	if(current.name != "scope")
		error::treeError("Expected scope after function declaration!");
	generator::inFunction = true;
	std::string line = obj.name;
	line += ".:";
	file::append(line);
	
	file::append_instruction("push", "rbp");
	file::append_instruction("mov", "rbp", "rsp");
	file::append("");
	
	branch* prevScope = currentBranchScope;
	currentBranchScope = &(current);
	generator::main();
	
	file::append("");
	file::append_instruction("pop", "rbp");
	file::append_instruction("ret");
	file::append("");
	
	generator::inFunction = false;
	currentBranchScope = prevScope;
	generator::stack = prevStack;
}

void generator::e::functionCall()
{
	
}

std::string generateAsmText() // generate text for inline assmebly [text]  ;;__asm__
{
	std::string text;
	text += current.sub.at(1).name;
	text.at(0) = '	';
	text += " ;;__asm__";
	return text;
}

void f_asmtext() // append to text section
{
	file::append_text(generateAsmText());
}

void f_asmdata() // append to data section
{
	file::append_data(generateAsmText());
}

void f_asmbss() // append to bss section
{
	file::append_bss(generateAsmText());
}
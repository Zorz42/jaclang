// this generates system functions

#include "jaclang.h"

void f_asmtext();
void f_asmdata();
void f_asmbss();

void generator::e::systemFunctionCall() // system function: __test__
{
	#define current mainBranch.sub.at(mainBranch.count) // function branch
	#define currentName mainBranch.sub.at(mainBranch.count).sub.at(0).name // name of function
	
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
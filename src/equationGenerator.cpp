// this generates tree equation into asm code

#include "jaclang.h"

int generator::currentRegister32 = 0;
std::vector<std::string> generator::availableRegisters32 =  // all registers that equations can use
{
	"ebx",
	"ecx",
	"esi",
	"edi",
	"r7d",
	"r8d",
	"r9d",
	"r10d",
	"r11d",
	"r12d",
	"r13d",
	"r14d",
	"r15d"
};

void generator::e::equation(branch& equation)
{
	if(equation.name == "/equation") // if branch is equation
	{
		if(equation.sub.at(0).name != "int") // for now equations only support int
			error::treeError("equation must be int");
		if(equation.sub.at(1).name == "functionCall")
		{
			bool funcExists = false;
			for(function iter : generator::functionVector)
				if(iter.name == equation.sub.at(1).sub.at(0).name)
				{
					funcExists = true;
					break;
				}
			if(!funcExists)
				error::treeError("Function does not exist!");
			file::append_instruction("call", equation.sub.at(1).sub.at(0).name + ".");
		}
		
		else if(equation.sub.at(1).name != "/equation")
		{
			std::string value = equation.sub.at(1).name;
			if(value.at(0) == ':')
			{
				value.erase(value.begin());
				bool varExists = false;
				for(variable iter : generator::stack) // go through stack
					if(iter.indent == value)
					{
						varExists = true;
						value = onStack(iter.position);
						break;
					}
				if(!varExists)
					error::treeError("Variable does not exist!");
			}
			file::append_instruction("mov", generator::availableRegister32(), value); // mov first value to register
		}
		else
		{
			generator::nextRegister();
			generator::e::equation(equation.sub.at(1));
			file::append_instruction("mov", generator::availableRegisters32.at(generator::currentRegister32 - 1), generator::availableRegister32());
			generator::currentRegister32--;
		}
		
		
		#define currentValue equation.sub.at(i).name
		#define currentOperator equation.sub.at(i - 1).name
		
		for(int i = 3; i <= equation.sub.size(); i += 2)
		{
			std::string currentValueAsm = currentValue;
			if(currentValueAsm.at(0) == ':')
			{
				currentValueAsm.erase(currentValueAsm.begin());
				bool varExists = false;
				for(variable iter : generator::stack) // go through stack
					if(iter.indent == currentValueAsm)
					{
						varExists = true;
						currentValueAsm = onStack(iter.position);
						break;
					}
				if(!varExists)
					error::treeError("Variable does not exist!");
			}
			if(currentValue.at(0) == '.')
				error::treeError("int cannot add string");
			else if(currentValue == "/equation")
			{
				generator::nextRegister();
				generator::e::equation(equation.sub.at(i));
				
				if(currentOperator == "+")
					file::append_instruction("add", generator::availableRegisters32.at(generator::currentRegister32 - 1), generator::availableRegister32());
				else if(currentOperator == "-")
					file::append_instruction("sub", generator::availableRegisters32.at(generator::currentRegister32 - 1), generator::availableRegister32());
				else if(currentOperator == "*")
				{
					file::append_instruction("mov", "eax", generator::availableRegister32());
					file::append_text("	imul " + generator::availableRegisters32.at(generator::currentRegister32 - 1));
					file::append_instruction("mov", generator::availableRegisters32.at(generator::currentRegister32 - 1), "eax");
				}
				else if(currentOperator == "/")
				{
					file::append_instruction("mov", "eax", generator::availableRegister32());
					file::append_text("	idiv " + generator::availableRegisters32.at(generator::currentRegister32 - 1));
					file::append_instruction("mov", generator::availableRegisters32.at(generator::currentRegister32 - 1), "eax");
				}
				else
					error::treeError("unrecognized operator");
				generator::currentRegister32--;
			}
			else if(currentOperator == "+")
				file::append_instruction("add", generator::availableRegister32(), currentValueAsm);
			else if(currentOperator == "-")
				file::append_instruction("sub", generator::availableRegister32(), currentValueAsm);
			else if(currentOperator == "*")
			{
				file::append_instruction("mov", "eax", currentValueAsm);
				file::append_text("	imul " + generator::availableRegister32());
				file::append_instruction("mov", generator::availableRegister32(), "eax");
			}
			else if(currentOperator == "/")
			{
				file::append_instruction("mov", "eax", generator::availableRegister32());
				generator::nextRegister();
				file::append_instruction("mov", generator::availableRegister32(), currentValueAsm);
				file::append_text("	idiv " + generator::availableRegister32());
				generator::currentRegister32--;
				file::append_instruction("mov", generator::availableRegister32(), "eax");
			}
			else
				error::treeError("unrecognized operator");
		}
	}
}

void generator::nextRegister()
{
	generator::currentRegister32++;
	if(generator::currentRegister32 == generator::availableRegisters32.size())
		error::treeError("register owerflow");
}

std::string generator::availableRegister32()
{
	return generator::availableRegisters32.at(generator::currentRegister32);
}
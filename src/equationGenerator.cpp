#include <jaclang.h>

// this generates tree equation into asm code

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

void generator::e::equation(branch equation)
{
	if(equation.name == "/equation") // if branch is equation
	{
		if(equation.sub.at(0).name != "int") // for now equations only support int
			error::treeError("equation must be int");
		
		if(equation.sub.at(1).name != "/equation")
			file::append_instruction("mov", generator::availableRegister32(), equation.sub.at(1).name); // mov first value to register
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
				file::append_instruction("add", generator::availableRegister32(), currentValue);
			else if(currentOperator == "-")
				file::append_instruction("sub", generator::availableRegister32(), currentValue);
			else if(currentOperator == "*")
			{
				file::append_instruction("mov", "eax", currentValue);
				file::append_text("	imul " + generator::availableRegister32());
				file::append_instruction("mov", generator::availableRegister32(), "eax");
			}
			else if(currentOperator == "/")
			{
				file::append_instruction("mov", "eax", generator::availableRegister32());
				generator::nextRegister();
				file::append_instruction("mov", generator::availableRegister32(), currentValue);
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
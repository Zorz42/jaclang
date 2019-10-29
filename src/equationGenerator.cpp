// this generates tree calculation into asm code

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

void generator::e::calculation(branch& calculation)
{
	if(calculation.name == "calc") // if branch is calculation
	{
		if(calculation.sub.at(0).name != "int") // for now equations only support int
			error::treeError("calculation must be int");
		if(calculation.sub.at(1).name == "functionCall") // check if its function call at the beginning
		{
			bool funcExists = false; // go through existing functions and check if it exists
			for(function iter : generator::functionVector)
				if(iter.name == calculation.sub.at(1).sub.at(0).name)
				{
					funcExists = true;
					break;
				}
			if(!funcExists)
				error::treeError("Function does not exist!");
			file::append_instruction("call", calculation.sub.at(1).sub.at(0).name + "."); // call function
		}
		
		else if(calculation.sub.at(1).name.at(0) == ':') // variables will have : at the beggining
		{
			std::string value = calculation.sub.at(1).name; // variable name
			
			value.erase(value.begin()); // remove :
			bool varExists = false; // go through stack and check if variable exists
			for(variable iter : generator::stack)
				if(iter.indent == value)
				{
					varExists = true;
					value = onStack(iter.position);
					break;
				}
			if(!varExists)
				error::treeError("Variable does not exist!");
				
			file::append_instruction("mov", generator::availableRegister32(), value); // mov first value to register
		}
		else
		{
			generator::nextRegister(); // else its just nested calculation
			generator::e::calculation(calculation.sub.at(1));
			file::append_instruction("mov", generator::availableRegisters32.at(generator::currentRegister32 - 1), generator::availableRegister32());
			generator::currentRegister32--;
		}
		
		
		#define currentValue calculation.sub.at(i).name
		#define currentOperator calculation.sub.at(i - 1).name
		
		for(int i = 3; i <= calculation.sub.size(); i += 2)
		{
			std::string currentValueAsm = currentValue;
			if(currentValueAsm.at(0) == ':') // if its variable
			{
				currentValueAsm.erase(currentValueAsm.begin()); // remove : at the beggining
				bool varExists = false; // check if variable exists
				for(variable iter : generator::stack)
					if(iter.indent == currentValueAsm)
					{
						varExists = true;
						currentValueAsm = onStack(iter.position); // if exists set current value to variable on stack
						break;
					}
				if(!varExists)
					error::treeError("Variable does not exist!");
			}
			if(currentValue.at(0) == '.') // if current value is string
				error::treeError("int cannot add string");
			else if(currentValue == "calc") // if value is calculation
			{
				generator::nextRegister();
				generator::e::calculation(calculation.sub.at(i));
				
				if(currentOperator == "+") // cases for opeartors
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
			else if(currentOperator == "+") // cases for operators default
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
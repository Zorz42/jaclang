// this generates tree calculation into asm code

#include <utility>

#include "jaclang.h"

unsigned int generator::currentRegister32 = 0;
std::vector<std::string> generator::availableRegisters32; // all registers that equations can use

void operator_add(const std::string& value);
void operator_sub(const std::string& value);
void operator_mul(const std::string& value);
void operator_div(const std::string& value);

std::string value_variable(std::string variableName);

void generator::e::calculation(branch& calculation)
{
	if(calculation.name == "calc") // if branch is calculation
	{
		if(calculation.sub.at(0).name != "int") // for now equations only support int
			error::treeError("calculation must be int");
		if(calculation.sub.at(1).name == "functionCall") // check if its function call at the beginning
			generator::e::functionCall(calculation.sub.at(1).sub.at(0).name);
		else if(calculation.sub.at(1).name.at(0) == ':') // variables will have : at the beginning
			file::append_instruction("mov", generator::availableRegister32(), value_variable(calculation.sub.at(1).name)); // mov first value to register
		else
		{
			generator::nextRegister(); // else its just nested calculation
			generator::e::calculation(calculation.sub.at(1));
			file::append_instruction("mov", generator::availableRegisters32.at(generator::currentRegister32 - 1), generator::availableRegister32());
			generator::prevRegister();
		}
		
		
		#define currentValue calculation.sub.at(i).name
		#define currentOperator calculation.sub.at(i - 1).name
		
		for(unsigned long i = 3; i <= calculation.sub.size(); i += 2)
		{
			std::string currentValueAsm = currentValue;
			if(currentValueAsm.at(0) == ':') // if its variable
				value_variable(currentValueAsm);
			if(currentValue.at(0) == '.') // if current value is string
				error::treeError("int cannot add string");
			else if(currentValue == "calc") // if value is calculation
            {
                generator::nextRegister();
                generator::e::calculation(calculation.sub.at(i));
                generator::prevRegister();

                currentValueAsm = generator::availableRegisters32.at(generator::currentRegister32 + 1);
            }

			if(currentValue == "functionCall") // check if its function call at the beginning
                generator::e::functionCall(calculation.sub.at(i).sub.at(0).name);
			else
            {
                if (currentOperator == "+") // cases for operators default
                    operator_add(currentValueAsm);
                else if (currentOperator == "-")
                    operator_sub(currentValueAsm);
                else if (currentOperator == "*")
                    operator_mul(currentValueAsm);
                else if (currentOperator == "/")
                    operator_div(currentValueAsm);
                else
                    error::treeError("unrecognized operator");
            }
		}
	}
}

void generator::nextRegister()
{
	generator::currentRegister32++;
	if(generator::currentRegister32 == generator::availableRegisters32.size())
		error::treeError("register overflow");
}

void generator::prevRegister()
{
    generator::currentRegister32--;
    if(generator::currentRegister32 == -1)
        error::treeError("register overflow");
}

std::string generator::availableRegister32()
{
	return generator::availableRegisters32.at(generator::currentRegister32);
}

void operator_add(const std::string& value)
{
    file::append_instruction("add", generator::availableRegister32(), value);
}

void operator_sub(const std::string& value)
{
    file::append_instruction("sub", generator::availableRegister32(), value);
}

void operator_mul(const std::string& value)
{
    file::append_instruction("mov", "eax", value);
    file::append_text("	imul " + generator::availableRegister32());
    file::append_instruction("mov", generator::availableRegister32(), "eax");
}
void operator_div(const std::string& value)
{
    file::append_instruction("mov", "eax", generator::availableRegister32());
    generator::nextRegister();
    if(generator::availableRegister32() != value)
        file::append_instruction("mov", generator::availableRegister32(), value);
    file::append_text("	idiv " + generator::availableRegister32());
    generator::prevRegister();
    file::append_instruction("mov", generator::availableRegister32(), "eax");
}

std::string value_variable(std::string variableName)
{
    std::string value = std::move(variableName); // variable name
    
    
    value.erase(value.begin()); // remove :
    bool varExists = false; // go through stack and check if variable exists
    for(const variable& iter : generator::stack)
        if(iter.indent == value)
        {
            varExists = true;
            value = onStack(iter.position);
            break;
        }
    if(!varExists)
    {
        std::string errorString = variableName;
        errorString += " does not exist";
        error::treeError(errorString);
    }
    return value;
}

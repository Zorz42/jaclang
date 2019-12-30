// this generates tree calculation into asm code

#include "jaclang.h"

unsigned int generator::currentRegister = 0;
std::vector<std::string> generator::availableRegisters[4]; // all registers that equations can use

void operator_add(const std::string& value);
void operator_sub(const std::string& value);
void operator_mul(const std::string& value);
void operator_div(const std::string& value);

int8_t currentValueAsmSize;

std::pair<std::string, std::vector<std::string>> getCalculationTypes(branch& calculation);

void generator::e::calculation(branch& calculation)
{
	if(calculation.name == "calc") // if branch is calculation
	{
	    // Getting types
        std::pair<std::string, std::vector<std::string>> calculationTypePair = getCalculationTypes(calculation);
	    std::vector<std::string>* calculationTypes = &calculationTypePair.second;
	    std::string* calculationType = &calculationTypePair.first;

	    // Actual calculation
		if(calculation.sub.at(0).name == "functionCall") // check if its function call at the beginning
			generator::e::functionCall(calculation.sub.at(0).sub.at(0).name);
		else if(calculation.sub.at(0).name.at(0) == ':') // variables will have : at the beginning
        {
            std::string value = calculation.sub.at(0).name; // variable name
            value.erase(value.begin()); // remove :
            variable curr = generator::get_variable(value);
            file::append_instruction("mov", generator::availableRegister(curr.size()),
                                     onStack(curr.position)); // mov first value to register
        }
		else if(calculation.sub.at(0).name == "calculation")
		{
			generator::nextRegister(); // its just nested calculation
			generator::e::calculation(calculation.sub.at(0));
			file::append_instruction("mov", generator::availableRegisters[2].at(generator::currentRegister - 1), generator::availableRegister(4));
			generator::prevRegister();
		}
		else // else its just constant
            file::append_instruction("mov", generator::availableRegister(4), calculation.sub.at(0).name);
        
		
		
		#define currentValue calculation.sub.at(i).name
		#define currentOperator calculation.sub.at(i - 1).name
		
		for(unsigned long i = 2; i <= calculation.sub.size(); i += 2)
		{
			std::string currentValueAsm = currentValue;
			currentValueAsmSize = 4; // default - int (4 bytes)
			if(currentValueAsm.at(0) == ':') // if its variable
            {
                std::string value = currentValueAsm; // variable name
                value.erase(value.begin()); // remove :
                variable curr = generator::get_variable(value);
                currentValueAsm = onStack(curr.position);
                currentValueAsmSize = curr.size();
            }

			if(currentValue == "calc") // if value is calculation
            {
                generator::nextRegister();
                generator::e::calculation(calculation.sub.at(i));
                generator::prevRegister();

                currentValueAsm = generator::availableRegisters[2].at(generator::currentRegister + 1);
            }

			else if(currentValue == "functionCall")
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
		delete calculationType;
		delete calculationTypes;
	}
}

void operator_add(const std::string& value)
{
    file::append_instruction("add", generator::availableRegister(currentValueAsmSize), value);
}

void operator_sub(const std::string& value)
{
    file::append_instruction("sub", generator::availableRegister(currentValueAsmSize), value);
}

void operator_mul(const std::string& value)
{
    file::append_instruction("mov", "eax", value);
    file::append_text("	imul " + generator::availableRegister(currentValueAsmSize));
    file::append_instruction("mov", generator::availableRegister(currentValueAsmSize), "eax");
}
void operator_div(const std::string& value)
{
    file::append_instruction("mov", "eax", generator::availableRegister(currentValueAsmSize));
    generator::nextRegister();
    if(generator::availableRegister(currentValueAsmSize) != value)
        file::append_instruction("mov", generator::availableRegister(currentValueAsmSize), value);
    file::append_text("	idiv " + generator::availableRegister(currentValueAsmSize));
    generator::prevRegister();
    file::append_instruction("mov", generator::availableRegister(currentValueAsmSize), "eax");
}

std::pair<std::string, std::vector<std::string>> getCalculationTypes(branch& calculation)
{
    std::pair<std::string, std::vector<std::string>> output;
    std::vector<std::string> calculationTypes;
    std::string calculationType;
#define current calculation.sub.at(i).name
    for(int i = 0; i < calculation.sub.size(); i += 2)
    {
        if(current == "calc")
            calculationTypes.push_back(getCalculationTypes(calculation.sub.at(i)).first);
        else if(current.at(0) == ':')
        {
            std::string variableName = current;
            variableName.erase(variableName.begin());
            variable currentVariable = generator::get_variable(variableName);
        }
        std::cout << current << std::endl;
    }
#undef current
    output.first = calculationType;
    output.second = calculationTypes;
    return output;
}
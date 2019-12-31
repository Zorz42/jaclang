// this generates tree calculation into asm code

#include "jaclang.h"

unsigned int generator::currentRegister = 0;
std::vector<std::string> generator::availableRegisters[4]; // all registers that equations can use

void operator_add(const std::string& value);
void operator_sub(const std::string& value);
void operator_mul(const std::string& value);
void operator_div(const std::string& value);

int8_t currentValueAsmSize;

bool operatorMatchMapHasKey(const std::string& key);
std::string getTypeMatch(const std::string& type1, const std::string& matchOperator, const std::string& type2);

std::string generator::e::calculation(branch& calculation)
{
    std::string currentValueType;
    std::string thisValueType;

    if(calculation.sub.at(0).name == "functionCall") // check if its function call at the beginning
    {
        generator::e::functionCall(calculation.sub.at(0).sub.at(0).name);
        currentValueType = "int";
    }
    else if(calculation.sub.at(0).name.at(0) == ':') // variables will have : at the beginning
    {
        std::string value = calculation.sub.at(0).name; // variable name
        value.erase(value.begin()); // remove :
        variable curr = generator::get_variable(value);
        file::append_instruction("mov", generator::availableRegister(curr.size()),
        onStack(curr.position)); // mov first value to register
        currentValueType = curr.type;
    }
    else if(calculation.sub.at(0).name == "calculation")
    {
        generator::nextRegister(); // its just nested calculation
        currentValueType = generator::e::calculation(calculation.sub.at(0));
        file::append_instruction("mov", generator::availableRegisters[3].at(generator::currentRegister - 1), generator::availableRegister(8));
        generator::prevRegister();
    }
    else // else its just constant
    {
        file::append_instruction("mov", generator::availableRegister(8), calculation.sub.at(0).name);
        currentValueType = "int";
    }

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
            thisValueType = curr.type;
        }
        else if(currentValue == "calc") // if value is calculation
        {
            generator::nextRegister();
            thisValueType = generator::e::calculation(calculation.sub.at(i));
            generator::prevRegister();

            currentValueAsm = generator::availableRegisters[2].at(generator::currentRegister + 1);
        }
        else if(currentValue == "functionCall")
        {
            generator::e::functionCall(calculation.sub.at(i).sub.at(0).name);
        }
        else
        {
            thisValueType = "int";
            if (currentOperator == "+") // default cases for operators
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
        currentValueType = getTypeMatch(currentValueType, currentOperator, thisValueType);
    }
    return currentValueType;
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

bool operatorMatchMapHasKey(const std::string& key)
{
    auto it = generator::operatorMatches.find(key);
    return it != generator::operatorMatches.end();
}

std::string getTypeMatch(const std::string& type1, const std::string& matchOperator, const std::string& type2)
{
    std::string key = type1 + matchOperator + type2;
    if(!operatorMatchMapHasKey(key))
        error::treeError("No match for operator '" + matchOperator + "' between '" + type1 + "' and '" + type2 + "'");
    return generator::operatorMatches[key];
}
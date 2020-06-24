enum Section {Section_Auto, Section_Data, Section_Bss, Section_Text, Section_Functions};

struct Instruction {
    std::string instruction, arg1, arg2;
    int size;
    Section section;
};

namespace asm_ {
    void main();

    void append_instruction(const std::string &inst, const std::string &arg1 = "", const std::string &arg2 = "", int size = 0, Section sect=Section_Auto);
    std::string onStack(long offset, bool positive=false);

    inline unsigned long current_scope_on_stack = 0;
    inline std::vector<Variable> stack;
    inline unsigned long stack_pointer = 0;
    inline unsigned long biggest_stack_pointer = 0;

    void incStackPointer(int value);
    void decStackPointer(int value);

    inline int8_t current_register = 0;
    inline std::vector<std::string> available_registers[4];

    void nextRegister();
    void prevRegister();

    void pushToStack(Variable source);

    std::string availableRegister(int8_t size, int8_t offset = 0);

    inline std::vector<Instruction> instructions;
}
